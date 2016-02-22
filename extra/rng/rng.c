// gcc -mavx2 -march=native -std=c99 -O3 -o rng rng.c -Wall -Wextra

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
    xorshift128_x = xorshift128_y;
    xorshift128_y = xorshift128_z;
    xorshift128_z = xorshift128_w;
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
/**
XOR shift 128
**/

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


void xorshift128plus_jump() {
    static const uint64_t JUMP[] = { 0x8a5cd789635d2dff, 0x121fd2155c472f96 };
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    for(unsigned int i = 0; i < sizeof (JUMP) / sizeof (*JUMP); i++)
        for(int b = 0; b < 64; b++) {
            if (JUMP[i] & 1ULL << b) {
                s0 ^= xorshift128plus_s[0];
                s1 ^= xorshift128plus_s[1];
            }
            xorshift128plus();
        }

    xorshift128plus_s[0] = s0;
    xorshift128plus_s[1] = s1;
}

/**
End of Vigna's
**/

/**
Vectorized version of Vigna's xorshift128plus
**/

/*

If I have seed x, and then with every random number generated 
the seed becomes x = f(x). So two applications give you x= f(f(x)).

Let f(f(x)) := f^(2) (x).  That is, I define the "power" of f to be 
a repeated application of f. E.g., f^(1) = f.

Vigna provides us with a function to compute f^(2^64).

Given an initial seed s, I can initialize my vector as

s,  f^(2^64)(s), f^(2 * 2^64)(s), f^(3 * 2^64)(s),...

Then on the next iteration I will have...

f(s),  f^(2^64+1)(s), f^(2 * 2^64+1)(s), f^(3 * 2^64+1)(s),...
  
and so forth.

This gives me a period of 2^64. I could increase this further by 2^64 is 
a huge number that should be fine for all applications.
*/

// used to initiate keys
// todo: streamline
//http://xorshift.di.unimi.it/xorshift128plus.c
void xorshift128plus_onkeys(uint64_t * ps0, uint64_t * ps1) {
    uint64_t s1 = *ps0;
    const uint64_t s0 = *ps1;
    *ps0 = s0;
    s1 ^= s1 << 23; // a
    *ps1 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
}

// used to initiate keys
// todo: streamline
void xorshift128plus_jump_onkeys( uint64_t  in1,  uint64_t  in2, uint64_t * output1, uint64_t * output2) {
    static const uint64_t JUMP[] = { 0x8a5cd789635d2dff, 0x121fd2155c472f96 };
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    for(unsigned int i = 0; i < sizeof (JUMP) / sizeof (*JUMP); i++)
        for(int b = 0; b < 64; b++) {
            if (JUMP[i] & 1ULL << b) {
                s0 ^= in1;
                s1 ^= in2;
            }
            xorshift128plus_onkeys(&in1,&in2);
        }
    output1[0] = s0;
    output2[0] = s1;
}



__m256i avx_xorshift128plus_s0;
__m256i avx_xorshift128plus_s1;

// call this once with non-zero values
void avx_xorshift128plus_init(uint64_t key1, uint64_t key2) {
    // this function could be streamlined quite a bit
    uint64_t S0[4];
    uint64_t S1[4];
    if((key1 == 0) || (key2 == 0)) printf("using zero keys?\n");
    S0[0] = key1;
    S1[0] = key2;
    xorshift128plus_jump_onkeys(*S0, *S1, S0+1, S1+1);
    xorshift128plus_jump_onkeys(*(S0+1), *(S1+1), S0+2, S1+2);
    xorshift128plus_jump_onkeys(*(S0+2), *(S1+2), S0+3, S1+3);
    avx_xorshift128plus_s0 = _mm256_loadu_si256((const __m256i*)S0);
    avx_xorshift128plus_s1 = _mm256_loadu_si256((const __m256i*)S1);
}

__m256i avx_xorshift128plus(void) {
    // we follow as closely as possible Vigna's code at http://xorshift.di.unimi.it/xorshift128plus.c
    __m256i s1 = avx_xorshift128plus_s0;
    const __m256i s0 = avx_xorshift128plus_s1;
    avx_xorshift128plus_s0 = avx_xorshift128plus_s1;
    s1 = _mm256_xor_si256(avx_xorshift128plus_s1,_mm256_slli_epi64(avx_xorshift128plus_s1,23));
    //s1 ^= s1 << 23; // a
    avx_xorshift128plus_s1 = _mm256_xor_si256(_mm256_xor_si256(_mm256_xor_si256(s1,s0),_mm256_srli_epi64(s1,18)),_mm256_srli_epi64(s0,5));
    //xorshift128plus_s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
    return _mm256_add_epi64(avx_xorshift128plus_s1 ,s0);
}


/**
end of vectorized version
**/

/**

Fog's MWC

*/

uint32_t fogmwc_iw;
uint32_t fogmwc_buffer1[16];
__m256i fogmwc_next() {
// Factors for multiply-with-carry
    static const uint32_t factors[16] = {
        4294963023, 0, 3947008974, 0, 4162943475, 0, 2654432763, 0,
        3874257210, 0, 2936881968, 0, 4294957665, 0, 2811536238, 0
    };
    __m256i x, f;
    __m256i y;
    x = _mm256_loadu_si256((const __m256i * )(fogmwc_buffer1 + fogmwc_iw));
    f = _mm256_loadu_si256((const __m256i * )(factors + fogmwc_iw));
    y = _mm256_mul_epu32(x, f);
    y = _mm256_add_epi64(y,_mm256_srli_epi64(y,32));//shift could be replaced by shuffle? Fog uses shift
    _mm256_storeu_si256((__m256i * )(fogmwc_buffer1 + fogmwc_iw),y);
    y = _mm256_xor_si256(y, _mm256_slli_epi64(y,30));
    y = _mm256_xor_si256(y, _mm256_srli_epi64(y,35));
    y = _mm256_xor_si256(y, _mm256_slli_epi64(y,13));
    fogmwc_iw = (fogmwc_iw + 8) & 15;
    return y;
}


void formwc_init(int seed) {
    unsigned int i;
    uint32_t tmp = seed;
    fogmwc_iw = 0;
    for (i = 0; i < 16; i++) {
        tmp = fogmwc_buffer1[i] = 1566083941u * (tmp ^ (tmp >> 27)) + i;
    }
    for (i = 0; i < 4 * 64 / sizeof(__m256i); i++)  fogmwc_next();
}


/**

End of Fog's MWC

*/


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


void populateRandom_avx_xorshift128plus(uint32_t * answer, uint32_t size) {
    uint32_t  i=0;
    const uint32_t block = sizeof(__m256i)/sizeof(uint32_t);//8
    while (i+block<=size) {
        _mm256_storeu_si256((__m256i * )(answer + i),avx_xorshift128plus());
        i += block;
    }
    if(i != size) {
        uint32_t buffer[sizeof(__m256i)/sizeof(uint32_t)];
        _mm256_storeu_si256((__m256i * )buffer,avx_xorshift128plus());
        memcpy(answer + i,buffer,sizeof(uint32_t) * (size - i));
    }
}

void populateRandom_fogmwc(uint32_t * answer, uint32_t size) {
    uint32_t  i=0;
    const uint32_t block = sizeof(__m256i)/sizeof(uint32_t);//8
    while (i+block<=size) {
        _mm256_storeu_si256((__m256i * )(answer + i),fogmwc_next());
        i += block;
    }
    if(i != size) {
        uint32_t buffer[sizeof(__m256i)/sizeof(uint32_t)];
        _mm256_storeu_si256((__m256i * )buffer,fogmwc_next());
        memcpy(answer + i,buffer,sizeof(uint32_t) * (size - i));
    }
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
    avx_xorshift128plus_init(1,2);
    formwc_init(1);
    printf("\nWe just generate the random numbers: \n");
    BEST_TIME(populateRandom_rand(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg32(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg64(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift128(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift128plus(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift64star(prec,size),, repeat, size);
    BEST_TIME(populateRandom_avx_xorshift128plus(prec,size),, repeat, size);
    BEST_TIME(populateRandom_fogmwc(prec,size),, repeat, size);

    free(prec);
    printf("\n");
}

int main() {
    demo(5000);
    return 0;
}
