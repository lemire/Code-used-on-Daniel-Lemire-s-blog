// gcc -march=native -std=c99 -O3 -o rng rng.c -Wall -Wextra

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <x86intrin.h>

typedef uint32_t value_t;

/*
Classic
*/

uint32_t xorshift128_x = 1, xorshift128_y = 2, xorshift128_z = 3, xorshift128_w = 4;

static inline uint32_t xorshift128(void) {
    uint32_t t = xorshift128_x;
    t ^= t << 11;
    t ^= t >> 8;
    xorshift128_x = xorshift128_y; xorshift128_y = xorshift128_z; xorshift128_z = xorshift128_w;
    xorshift128_w ^= xorshift128_w >> 19;
    xorshift128_w ^= t;
    return xorshift128_w;
}


uint64_t xorshift64star_x=1; /* The state must be seeded with a nonzero value. */

uint64_t xorshift64star(void) {
	xorshift64star_x ^= xorshift64star_x >> 12; // a
	xorshift64star_x ^= xorshift64star_x << 25; // b
	xorshift64star_x ^= xorshift64star_x >> 27; // c
	return xorshift64star_x * UINT64_C(2685821657736338717);
}

/**
BEGIN PCG code
*/

struct pcg_state_setseq_64 {    // Internals are *Private*.
    uint64_t state;             // RNG state.  All values are possible.
    uint64_t inc;               // Controls which RNG sequence (stream) is
    // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;
typedef __uint128_t pcg128_t;
struct pcg_state_setseq_128 {
    pcg128_t state;
    pcg128_t inc;
};
typedef struct pcg_state_setseq_128 pcg64_random_t;

static pcg32_random_t pcg32_global = { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL };

#define PCG_128BIT_CONSTANT(high,low) \
        ((((pcg128_t)high) << 64) + low)

#define PCG_DEFAULT_MULTIPLIER_128 \
        PCG_128BIT_CONSTANT(2549297995355413924ULL,4865540595714422341ULL)

#define PCG64_INITIALIZER                                       \
    { PCG_128BIT_CONSTANT(0x979c9a98d8462005ULL, 0x7d3e9cb6cfe0549bULL),       \
      PCG_128BIT_CONSTANT(0x0000000000000001ULL, 0xda3e39cb94b95bdbULL) }

static pcg64_random_t pcg64_global = PCG64_INITIALIZER;

static inline uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline uint64_t pcg64_random_r(struct pcg_state_setseq_128* rng) {
    // the 32-bit version uses the old state to generate the next value
    rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_128 + rng->inc;
    uint64_t xorshifted = ((uint64_t)(rng->state >> 64u)) ^ (uint64_t)rng->state;
    unsigned int rot = rng->state >> 122u;
    return (xorshifted >> rot) | (xorshifted << ((- rot) & 63));
}

static inline uint32_t pcg32_random() {
    return pcg32_random_r(&pcg32_global);
}

static inline uint64_t pcg64_random() {
    return pcg64_random_r(&pcg64_global);
}

/**
END PCG code
*/

/*
Vigna's
*/


uint64_t xorshift128plus_s[2]= {1,3};

//http://xorshift.di.unimi.it/xorshift128plus.c
uint64_t xorshift128plus(void) {
    uint64_t s1 = xorshift128plus_s[0];
    const uint64_t s0 = xorshift128plus_s[1];
    xorshift128plus_s[0] = s0;
    s1 ^= s1 << 23; // a
    xorshift128plus_s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
    return xorshift128plus_s[1] + s0;
}




void populateRandom_rand(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i!=0; i--) {
        answer[size-i] =   rand();
    }
}


void populateRandom_pcg32(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i!=0; i--) {
        answer[size-i] =   pcg32_random();
    }
}

void populateRandom_pcg64(uint32_t * answer, uint32_t size) {
    uint32_t  i=size;
    while (i>2) {
        * (uint64_t * )(answer + size - i) = pcg64_random();
        i-=2;
    }
    if(i!=0)
        answer[size-i] =   pcg32_random();
}
void populateRandom_xorshift128(uint32_t * answer, uint32_t size) {
  for (uint32_t  i=size; i!=0; i--) {
      answer[size-i] =   xorshift128();
  }
}

void populateRandom_xorshift128plus(uint32_t * answer, uint32_t size) {
    uint32_t  i=size;
    while (i>2) {
        * (uint64_t * )(answer + size - i) = xorshift128plus();
        i-=2;
    }
    if(i!=0)
        answer[size-i] =   (uint32_t) xorshift128plus();
}

void populateRandom_xorshift64star(uint32_t * answer, uint32_t size) {
    uint32_t  i=size;
    while (i>2) {
        * (uint64_t * )(answer + size - i) = xorshift64star();
        i-=2;
    }
    if(i!=0)
        answer[size-i] =   (uint32_t) xorshift64star();
}


#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)




/*
 * Prints the best number of operations per cycle where
 * test is the function call, answer is the expected answer generated by
 * test, repeat is the number of times we should repeat and size is the
 * number of operations represented by test.
 */
#define BEST_TIME(test, pre, repeat, size)                         \
        do {                                                              \
            printf("%s: ", #test);                                        \
            fflush(NULL);                                                 \
            uint64_t cycles_start, cycles_final, cycles_diff;             \
            uint64_t min_diff = (uint64_t)-1;                             \
            for (int i = 0; i < repeat; i++) {                            \
                pre;                                                       \
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                RDTSC_START(cycles_start);                                \
                test;                     \
                RDTSC_FINAL(cycles_final);                                \
                cycles_diff = (cycles_final - cycles_start);              \
                if (cycles_diff < min_diff) min_diff = cycles_diff;       \
            }                                                             \
            uint64_t S = size;                                            \
            float cycle_per_op = (min_diff) / (double)S;                  \
            printf(" %.2f cycles per operation", cycle_per_op);           \
            printf("\n");                                                 \
            fflush(NULL);                                                 \
 } while (0)


void demo(int size) {
    printf("Generating %d 32-bit random numbers \n",size);
    printf("Time reported in number of cycles per array element.\n");
    printf("We store values to an array of size = %lu kB.\n",size*sizeof(value_t)/(1024));
    int repeat = 500;
    uint32_t * prec = malloc(size * sizeof(value_t));
    printf("\nWe just generate the random numbers: \n");
    BEST_TIME(populateRandom_rand(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg32(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg64(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift128(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift128plus(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift64star(prec,size),, repeat, size);


    free(prec);
    printf("\n");
}

int main() {
    demo(5000);
    return 0;
}
