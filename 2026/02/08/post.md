# The cost of a function call

When programming, we chain functions together. Function A calls function B. And so forth. 

You do not have to program this way, you could write an entire program using a single function. It would be a fun exercise to write a non-trivial program using a single function... as long as you delegate the code writing to AI because human beings quickly struggle with long functions.

A key compiler optimization is 'inlining': the compiler takes your function definition and it tries to substitute it at the call location. It is conceptually quite simple. Consider the following example where the function `add3` calls the function `add`.

```c
int add(int x, int y) {
    return x + y;
}

int add3(int x, int y, int z) {
    return add(add(x, y), z);
}
```

You can manually inline the call as follows.


```c
int add3(int x, int y, int z) {
    return x + y + z;
}
```


A function call is reasonably cheap performance-wise, but not free. If the function takes non-trivial parameters, you might need to save and restore them on the stack, so you get extra loads and stores. You need to jump into the function, and then jump out at the end. And depending on the function call convention on your system, and the type of instructions you are using, there are extra instructions at the beginning and at the end.

If a function is sufficiently simple, such as my `add` function, it should always be inlined when performance is critical. Let us examine a concrete example. Let me sum the integers in an array.

```cpp
for (int x : numbers) {
  sum = add(sum, x);
}
```

I am using my MacBook (M4 processor with LLVM).

| function |  ns/int |
|----------|---------|
| regular  |   0.7      |
| inline  |   0.03      |

Wow. The inline version is over 20 times faster.

Let us try to see what is happening. The call site of the 'add' function is just a straight loop with a call to the function.

```asm

ldr    w1, [x19], #0x4
bl     0x100021740    ; add(int, int)
cmp    x19, x20
b.ne   0x100001368    ; <+28>
```

The function itself is as cheap as it can be: just two instructions.

```asm

add    w0, w1, w0
ret
```

So, we spend 6 instructions for each addition. It takes about 3 cycles per addition.

What about the inline function?

```asm

ldp    q4, q5, [x12, #-0x20]
ldp    q6, q7, [x12], #0x40
add.4s v0, v4, v0
add.4s v1, v5, v1
add.4s v2, v6, v2
add.4s v3, v7, v3
subs   x13, x13, #0x10
b.ne   0x1000013fc    ; <+104>
```

It is entirely different. The compiler has converted the addition to advanced (SIMD) instructions processing blocks of 16 integers using 8 instructions. So we are down to half an instruction per integer (from 6 instructions). So we use 12 times fewer instructions. On top of having fewer instructions, the processor is able to retire more instructions per cycle, for a massive performance boost.

What if we prevented the compiler from using these fancy instructions while still inlining? We still get a significant performance boost (about 10x faster).


| function |  ns/int |
|----------|---------|
| regular  |   0.7      |
| inline  |   0.03      |
| inline (no SIMD)  |   0.07      |

Ok. But the `add` function is a bit extreme. We know it should always be inlined. What about something less trivial like a function that counts the number of spaces in a string.

```cpp
size_t count_spaces(std::string_view sv) {
    size_t count = 0;
    for (char c : sv) {
        if (c == ' ') ++count;
    }
    return count;
}
```

If the string is reasonably long, then the overhead of the function call should be negligible.
Let us pass a string of 1000 characters.


| function |  ns/string |
|----------|---------|
| regular  |   111      |
| inline  |   115      |

The inline version is not only not faster, but it is even slightly slower.  I am not sure why.

What if I use short strings (say between 0 and 6 characters)? Then the inline function is measurably faster.

| function |  ns/string |
|----------|---------|
| regular  |   1.6      |
| inline  |   1.0     |


Takeaways:

1. Short and simple functions should be inlined when possible if performance is a concern. The benefits can be impressive.
2. For functions that can be fast or slow, the decision as to whether to inline or not depends on the input. For string processing functions, the size of the string may determine whether inlining is necessary for best performance.


*Note*: [My source code is available](https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/tree/master/2026/02/08/benchmark).