#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>

#define STR_SIZE 16

typedef struct {
    atomic_int refs;
    int values[STR_SIZE];
} shared_array;

shared_array *str_new(void);
shared_array *str_retain(shared_array *o);
shared_array *update(size_t idx, int value, shared_array *o);
void obj_release(shared_array *o);

// create a new array that can be shared between threads.
// Returns NULL on allocation failure. The caller owns one reference.
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

/* Take an additional reference. The caller must already hold one, so the
   object cannot die under us and the increment publishes no data of its
   own: relaxed is enough. Ordering comes from whatever hands the pointer
   to the other thread, and from the release in obj_release. */
shared_array *str_retain(shared_array *o) {
    atomic_fetch_add_explicit(&o->refs, 1, memory_order_relaxed);
    return o;
}

/* Consumes the caller's reference to o and returns a reference to the
   updated array, which may or may not be o. Do not touch o afterwards.
   Returns NULL, keeping the caller's reference to o, if a copy was needed
   but could not be allocated. */
shared_array *update(size_t idx, int value, shared_array *o) {
    assert(idx < STR_SIZE);

    /* If we are the only owner, we can just update the value in place.
       Acquire so that we see the writes of every thread that dropped its
       reference before us. */
    if (atomic_load_explicit(&o->refs, memory_order_acquire) == 1) {
        o->values[idx] = value;
        return o;
    }

    /* Otherwise, we need to make a copy of the array and update that. */
    shared_array *new_o = str_new();
    if (new_o == NULL) {
        return NULL;
    }
    for (int i = 0; i < STR_SIZE; i++) {
        new_o->values[i] = o->values[i];
    }
    new_o->values[idx] = value;

    /* Release our reference to the old array. */
    obj_release(o);
    return new_o;
}

void obj_release(shared_array *o) {
    /* If this was not the last ref, we only published
       "I am done with my copy of the pointer." */
    int prev = atomic_fetch_sub_explicit(&o->refs, 1, memory_order_release);
    if (prev != 1)
        return;

    /* Last owner. Acquire so we see every write other threads
       made while they still held a ref. */
    atomic_thread_fence(memory_order_acquire);
    free(o);
}
