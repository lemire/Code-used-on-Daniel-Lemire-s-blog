# A quick overview of atomics in C

If you write in C, by default, you use a single thread. Extra cores do not help until you create more threads.
However, if you include the header `<threads.h>`, you can pass a function to `thrd_create`, and wait for it with `thrd_join`.

```c
#include <threads.h>
#include <stdio.h>

int worker(void *arg) {
    printf("hello from thread %d\n", *(int *)arg);
    return 0;
}

int main(void) {
    thrd_t t;
    int id = 1;
    thrd_create(&t, worker, &id);
    thrd_join(t, NULL);
}
```

Be warned that C11 threads are an optional feature. If the macro `__STDC_NO_THREADS__` is defined, you do not have them. Apple's C library has never shipped `<threads.h>`, so the program above does not compile on macOS, and glibc only added it in version 2.28 (2018). On such systems you fall back on POSIX threads (`pthread_create`, `pthread_join`).

Once you have more than one thread, they may share memory. If two threads access the same non-atomic variable with no ordering between them, and at least one of them writes, the C language calls that a data race. In other words, it is unsafe. 

If you have a variable and it is effectively constant, then it is fine to share it. But as soon as anyone changes it, then it might get corrupted. If it is not guarded somewhat, you are in trouble.

To be clear, that is what the C programming language says. I don't mean that it will happen on your machine.

To get a better behavior, we can use atomic variables. In C, you have the `<stdatomic.h>` header.

An atomic integer is never garbage. You always read a value that was once written.

In practice, on most computers you might use today, aligned 8-, 16-, 32- and 64-bit loads and stores are atomic. The C language does not care about that, so if you don't specifically require atomicity, you might get in trouble with your C compiler.

The next funny problem is that instructions can be reordered. When you write:

```
x = 2
y = 3
```

This may not happen in this sequence. The variable `y` might be set before the variable `x`. You may wonder why this is allowed at all. The fundamental reason is that our processors are quite complex. They have layers of buffers and they can execute multiple instructions at once. They can issue several memory loads or stores at once.

By default, in C, atomic accesses are all ordered. It is as if there is an oracle that watches all threads and comes up with a consistent story where everything is in order. This can be expensive, so we prefer not to do it that way.

At the other extreme is the relaxed model: your reads and stores are not garbage, and a given atomic still has one modification order (you will not see 1 and then 0 if the counter only went from 0 to 1), but there is no ordering with respect to other memory.

So we use something intermediate, the release and acquire semantics. They are ordering barriers. A strict barrier would be 'everything before me really happens before me, and everything after me really happens after me'. (Where 'really happens' refers to visible effects, the hardware and compiler are allowed to cheat as long as you don't catch them.) It is a bit too strong. So we split it in two parts: release and acquire. Intuitively, release means 'if you see me, you see all the stuff before me'. Acquire means 'I take that package, and everything I do after this load really happens after it'.

Consider the case where you have a resource (such as a block of allocated memory). You share this resource, but count how many people have access to it. When the counter goes to zero, you free the resource.

One thread could do...

```
access resource
decrement counter // I won't need it anymore
```

You see these operations happening one after the other, but they may not execute this way. It is possible that they overlap, or even that the decrement occurs before the access. It is entirely safe in a single threaded context.

Anyhow, so the following could happen

```
decrement counter // I won't need it anymore
access resource
```



But what if you have a second thread that does:


```
access resource
decrement counter // I won't need it anymore
if (counter is zero)
  free(resource)
```

You could have this interplay:


```
[thread2] access resource
[thread2] decrement counter
[thread1] decrement counter
[thread2] free(resource)
[thread1] access resource
```
 
That would be a bug.

So what you first do is make the decrement a 'release access' which means that operations that come before it cannot be reordered after it. So if you do it this way...

```
access resource
decrement counter using release
```

Then it is not possible that we 'see' the operations as if they happened in the reverse order.

But then we have a second problem. Release is enough for this thread: we cannot still be using the resource after we drop it. It does not tell the last owner that everyone else is finished. The last decrement is itself a release, so it does not observe the other threads' releases. Without an acquire, that last thread can call `free` while another thread's earlier access is not yet done.

```
[thread2] access resource
[thread2] decrement counter using release
[thread1] decrement counter using release
[thread1] free(resource)
```

Thread 2 did its access before its release decrement, but thread 1 never acquired, so it is not required to see that access as finished before `free`.

So we need the counterpart to a release, an acquire. The last owner acquires before it frees, and that pairs with everyone else's release:

```
access resource
decrement counter with release
if (counter is zero)
  acquire barrier // see that everyone else is done
  free(resource)
```


Alternatively, you could do this.

```
access resource
decrement counter with release and acquire
if (counter is zero)
  free(resource)
```

The two are equivalent, but they are not necessarily equally cheap. 



So let us consider a nice example. Let us build a small array that several threads can share. If you are the only owner, you overwrite an element in place. If not, you copy, then you update the copy. That is called copy-on-write. It is a really nice idea that you will find in many important systems.

We start with the type.

```c
#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>

#define STR_SIZE 16

typedef struct {
    atomic_int refs;
    int values[STR_SIZE];
} shared_array;
```

The payload is a plain `int` array. Only `refs` is atomic. That is deliberate. We never write `values` while another thread might be reading them.

We create an instance like so.

```c
shared_array *str_new(void) {
    shared_array *o = malloc(sizeof *o);
    if (o == NULL) {
        return NULL;
    }
    atomic_init(&o->refs, 1);
    for (int i = 0; i < STR_SIZE; i++) {
        o->values[i] = 0;
    }
    return o;
}
```

The `atomic_init` is not an atomic access in the memory-model sense. Nobody else has the pointer yet, so there is no other thread to race with. The caller owns one reference. It is just how we initialize an `atomic_int`.

Here is how we might naively release an instance.

```c
// not real code
void obj_release(shared_array *o) {
    auto ref = o->refs;
    o->refs -= 1;
    if (ref != 1)
        return;
    // we are the last copy
    free(o);
}
```

What is the problem with this code?

The load and the decrement are two operations. Two threads can both read 2, both subtract, the counter hits zero, and nobody frees: the resource leaks. Write the check the other way around, decrementing first and then testing whether the counter is zero, as in the pseudocode above, and you get the mirror-image bug instead: with `refs` at 2, one thread decrements to 1, the other decrements to 0, both then read 0, and both call `free`. You need one atomic subtract that hands you the previous value: only the thread that saw 1 was last.

So you could try

```c
void obj_release(shared_array *o) {
    if (atomic_fetch_sub_explicit(&o->refs, 1, memory_order_relaxed) != 1)
        return;
    free(o);
}
```

But suppose you have two owners, so that `refs` is 2. And you have two threads doing

```c
(void)o->values[4];
obj_release(o);
```

One of them will call `free`, but the order could be

```c
[thread1] o->values[4];
[thread2] atomic_fetch_sub_explicit(&o->refs, 1, memory_order_relaxed)
[thread1] atomic_fetch_sub_explicit(&o->refs, 1, memory_order_relaxed)
[thread1] free(o);
[thread2] o->values[4];
```

It is a bit confusing because things are not happening in order within thread 2:

```c
[thread2] atomic_fetch_sub_explicit(&o->refs, 1, memory_order_relaxed)
[thread2] o->values[4];
```

But this is allowed. 

So what we can do is put a release on the `atomic_fetch_sub_explicit` and then an acquire right before the `free`.

```c
void obj_release(shared_array *o) {
    if (atomic_fetch_sub_explicit(&o->refs, 1, memory_order_release) != 1)
        return;
    atomic_thread_fence(memory_order_acquire);
    free(o);
}
```

The release on every decrement means "I am done with the payload." The acquire fence, only on the last owner, means "I have seen that everyone else is done." Then `free` is safe.

That release does double duty, as we are about to see. It is also what lets the last remaining owner write to the payload in place.



If a thread wants another reference to the same instance, it only needs a relaxed access.

```c
shared_array *str_retain(shared_array *o) {
    atomic_fetch_add_explicit(&o->refs, 1, memory_order_relaxed);
    return o;
}
```

Why relaxed? Because the caller already holds a reference, so the object cannot be freed under us: the last owner would need our reference to be gone first. 


We can now write `update`. It consumes the caller's reference and returns a reference to the array that contains the new value, which may or may not be the same object. After you call it, you must not touch the pointer you passed in. There is one exception: if a copy was needed and the allocation failed, it returns `NULL` and leaves the caller's reference to `o` untouched, so you still own it and must still release it.

```c
shared_array *update(size_t idx, int value, shared_array *o) {
    assert(idx < STR_SIZE);

    if (atomic_load_explicit(&o->refs, memory_order_acquire) == 1) {
        o->values[idx] = value;
        return o;
    }

    shared_array *new_o = str_new();
    if (new_o == NULL) {
        return NULL;
    }
    for (int i = 0; i < STR_SIZE; i++) {
        new_o->values[i] = o->values[i];
    }
    new_o->values[idx] = value;
    obj_release(o);
    return new_o;
}
```

If the load reads 1, we are the only owner. No other thread holds a reference, so we can write `values[idx]` in place.

The load is an acquire. When the load reads 1, it may read the value written by the release decrement of the last *other* owner to drop out. Everything that thread did with `values` happens before our write. Nothing in our code appearing after such as `o->values[idx] = value` may move before it. No other thread still holds a reference, so the write does not race with a concurrent reader. Later, after a retain, other threads can see it.

On x64, acquire and release are effectively free at the CPU: ordinary loads already behave like acquire, ordinary stores like release. You still have to write them in C, or the compiler may reorder the payload accesses. ARM has a  weaker memory model so the acquire/release require different instructions (`ldapr`, `ldaddl`) which may incur a small perforamnce hit.

[The code is available.](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/09/09)
