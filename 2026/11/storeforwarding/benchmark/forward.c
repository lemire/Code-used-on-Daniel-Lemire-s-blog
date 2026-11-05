#include "forward.h"

#include <string.h>

#if defined(__clang__)
#define NOUNROLL _Pragma("clang loop unroll(disable)")
#else
#define NOUNROLL _Pragma("GCC unroll 0")
#endif

// The empty asm keeps the compiler from turning `x += 1` into `x += n`.
// It emits no instruction. This is the cost of the loop with no memory in it.
uint64_t lat_reg(uint64_t x, int n) {
    NOUNROLL
    for (int i = 0; i < n; i++) {
        __asm__ volatile("" : "+r"(x));
        x += 1;
    }
    return x;
}

// Store 8, load the same 8. Volatile, because otherwise the compiler deletes
// the reload and the loop: it can see that the load returns x.
uint64_t lat_same(unsigned char *p, uint64_t x, int n) {
    volatile uint64_t *w = (volatile uint64_t *)p;
    NOUNROLL
    for (int i = 0; i < n; i++) {
        *w = x;
        x = *w;
    }
    return x;
}

// Store 8, load the low 4. The load is contained in the store.
uint64_t lat_low32(unsigned char *p, uint64_t x, int n) {
    volatile uint64_t *w = (volatile uint64_t *)p;
    volatile uint32_t *q = (volatile uint32_t *)p;
    NOUNROLL
    for (int i = 0; i < n; i++) {
        *w = x;
        x = *q;
    }
    return x;
}

// Store 8, load 4 bytes at +4. Still contained, not at the same address.
uint64_t lat_hi32(unsigned char *p, uint64_t x, int n) {
    volatile uint64_t *w = (volatile uint64_t *)p;
    volatile uint32_t *q = (volatile uint32_t *)(p + 4);
    NOUNROLL
    for (int i = 0; i < n; i++) {
        *w = x;
        x = *q;
    }
    return x;
}

// Store 4, load 8. The load is wider than the store.
uint64_t lat_u32(unsigned char *p, uint64_t x, int n) {
    NOUNROLL
    for (int i = 0; i < n; i++) {
        uint32_t lo = (uint32_t)x;
        memcpy(p, &lo, 4);
        uint64_t w;
        memcpy(&w, p, 8);
        x = w;
    }
    return x;
}

// Store 1, load 8.
uint64_t lat_byte(unsigned char *p, uint64_t x, int n) {
    NOUNROLL
    for (int i = 0; i < n; i++) {
        unsigned char b = (unsigned char)x;
        memcpy(p, &b, 1);
        uint64_t w;
        memcpy(&w, p, 8);
        x = w;
    }
    return x;
}

// Two 4-byte stores, then one 8-byte load. Volatile so the compiler does not
// merge the stores into one 8-byte store and delete the load. The shift
// splits x into the two halves. It is one extra arithmetic instruction.
uint64_t lat_two(unsigned char *p, uint64_t x, int n) {
    volatile uint32_t *q = (volatile uint32_t *)p;
    volatile uint64_t *w = (volatile uint64_t *)p;
    NOUNROLL
    for (int i = 0; i < n; i++) {
        q[0] = (uint32_t)x;
        q[1] = (uint32_t)(x >> 32);
        x = *w;
    }
    return x;
}

// Store 8 at p, load 8 at p+4. Partial overlap.
uint64_t lat_overlap4(unsigned char *p, uint64_t x, int n) {
    NOUNROLL
    for (int i = 0; i < n; i++) {
        memcpy(p, &x, 8);
        uint64_t w;
        memcpy(&w, p + 4, 8);
        x = w;
    }
    return x;
}

// Store 8 at p, load 8 at p+1.
uint64_t lat_overlap1(unsigned char *p, uint64_t x, int n) {
    NOUNROLL
    for (int i = 0; i < n; i++) {
        memcpy(p, &x, 8);
        uint64_t w;
        memcpy(&w, p + 1, 8);
        x = w;
    }
    return x;
}

// Store 8 at p, load 8 at p+64. No overlap. The load should not wait for
// the store. Volatile so the store is not deleted.
uint64_t lat_plus64(unsigned char *p, uint64_t x, int n) {
    volatile uint64_t *a = (volatile uint64_t *)p;
    volatile uint64_t *b = (volatile uint64_t *)(p + 64);
    NOUNROLL
    for (int i = 0; i < n; i++) {
        *a = x;
        x = *b;
    }
    return x;
}

// Same as lat_plus64, but the load is 4096 bytes later: same offset within
// the page. On some Intel processors that false match also makes the load
// wait for the store.
uint64_t lat_plus4k(unsigned char *p, uint64_t x, int n) {
    volatile uint64_t *a = (volatile uint64_t *)p;
    volatile uint64_t *b = (volatile uint64_t *)(p + 4096);
    NOUNROLL
    for (int i = 0; i < n; i++) {
        *a = x;
        x = *b;
    }
    return x;
}

uint64_t thr_flip_byte(uint64_t *a, size_t n) {
    uint64_t h = 0;
    NOUNROLL
    for (size_t i = 0; i < n; i++) {
        ((unsigned char *)&a[i])[0] ^= 1;
        h += a[i];
    }
    return h;
}

uint64_t thr_flip_word(uint64_t *a, size_t n) {
    uint64_t h = 0;
    NOUNROLL
    for (size_t i = 0; i < n; i++) {
        uint64_t x = a[i];
        x ^= 1;
        a[i] = x;
        h += x;
    }
    return h;
}

uint64_t thr_join_mem(unsigned char *tmp, const uint32_t *a, const uint32_t *b,
                      size_t n) {
    uint64_t h = 0;
    NOUNROLL
    for (size_t i = 0; i < n; i++) {
        memcpy(tmp, &a[i], 4);
        memcpy(tmp + 4, &b[i], 4);
        uint64_t w;
        memcpy(&w, tmp, 8);
        h ^= w;
    }
    return h;
}

uint64_t thr_join_reg(const uint32_t *a, const uint32_t *b, size_t n) {
    uint64_t h = 0;
    NOUNROLL
    for (size_t i = 0; i < n; i++) {
        uint64_t w = (uint64_t)a[i] | ((uint64_t)b[i] << 32);
        h ^= w;
    }
    return h;
}
