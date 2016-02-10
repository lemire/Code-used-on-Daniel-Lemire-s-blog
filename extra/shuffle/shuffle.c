// gcc -march=native -std=c99 -O3 -o shuffle shuffle.c -Wall -Wextra

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <x86intrin.h>

typedef uint32_t value_t;

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

__attribute__((always_inline))
static inline uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

__attribute__((always_inline))
static inline uint64_t pcg64_random_r(struct pcg_state_setseq_128* rng) {
    // the 32-bit version uses the old state to generate the next value
    rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_128 + rng->inc;
    uint64_t xorshifted = ((uint64_t)(rng->state >> 64u)) ^ (uint64_t)rng->state;
    unsigned int rot = rng->state >> 122u;
    return (xorshifted >> rot) | (xorshifted << ((- rot) & 63));
}

__attribute__((always_inline))
static inline uint32_t pcg32_random() {
    return pcg32_random_r(&pcg32_global);
}

__attribute__((always_inline))
static inline uint64_t pcg64_random() {
    return pcg64_random_r(&pcg64_global);
}

/**
END PCG code
*/

/*
SIMD PCG
*/


uint64_t xorshift128plus_s[2]= {1,3};

//http://xorshift.di.unimi.it/xorshift128plus.c
__attribute__((always_inline))
static inline uint64_t xorshift128plus(void) {
    uint64_t s1 = xorshift128plus_s[0];
    const uint64_t s0 = xorshift128plus_s[1];
    xorshift128plus_s[0] = s0;
    s1 ^= s1 << 23; // a
    xorshift128plus_s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
    return xorshift128plus_s[1] + s0;
}



/**

Next we present the bounded RNG

*/

// as per the PCG implementation , uses two 32-bit divisions
static inline uint32_t pcg32_random_bounded(uint32_t bound)
{
    uint32_t threshold = -bound % bound;
    for (;;) {
        uint32_t r = pcg32_random();
        if (r >= threshold)
            return r % bound;
    }
}

// as per the Java implementation , uses one or more 32-bit divisions
static inline uint32_t java_random_bounded(uint32_t bound)
{
    uint32_t rkey = pcg32_random();
    uint32_t candidate = rkey % bound;
    while(rkey - candidate  > UINT32_MAX - bound + 1 ) { // will be predicted as false
        rkey = pcg32_random();
        candidate = rkey % bound;
    }
    return candidate;

}
// division-less bounded random number, uses 64-bit multiplication and shifts
static inline uint32_t pcg32_random_bounded_divisionless_fancy(uint32_t range) {
    uint64_t random32bit, multiresult;
    uint32_t leftover;
    uint32_t threshold;
    random32bit =  pcg32_random();
    if((range & (range - 1)) == 0) {
        return random32bit & (range - 1);
    }
    if(range >0x80000000) {// if range > 1<<31
        while(random32bit >= range) {
            random32bit =  pcg32_random();
        }
        return random32bit; // [0, range)
    }
    multiresult = random32bit * range;
    leftover = (uint32_t) multiresult;
    if(leftover < range ) {
        threshold = 0xFFFFFFFF % range ;
        while (leftover <= threshold) {
            random32bit =  pcg32_random();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
        }
    }
    return multiresult >> 32; // [0, range)
}

// this simplified version contains just one major branch/loop. For powers of two or large range, it is suboptimal.
static inline uint32_t pcg32_random_bounded_divisionless(uint32_t range) {
    uint64_t random32bit, multiresult;
    uint32_t leftover;
    uint32_t threshold;
    random32bit =  pcg32_random();
    multiresult = random32bit * range;
    leftover = (uint32_t) multiresult;
    if(leftover < range ) {
        threshold = -range % range ;
        while (leftover < threshold) {
            random32bit =  pcg32_random();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
        }
    }
    return multiresult >> 32; // [0, range)
}
static inline uint32_t xorshift128plus_random_bounded_divisionless(uint32_t range) {
    uint64_t random32bit, multiresult;
    uint32_t leftover;
    uint32_t threshold;
    random32bit = (uint32_t) xorshift128plus();
    multiresult = random32bit * range;
    leftover = (uint32_t) multiresult;
    if(leftover < range ) {
        threshold = -range % range ;
        while (leftover < threshold) {
            random32bit =  (uint32_t) xorshift128plus();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
        }
    }
    return multiresult >> 32; // [0, range)
}


// this simplified version contains just one major branch/loop. For powers of two or large range, it is suboptimal.
__attribute__((always_inline))
static inline void pcg32_random_bounded_divisionless_two_by_two(uint32_t range1, uint32_t range2, uint32_t *output1, uint32_t *output2) {
    uint64_t random64bit, random32bit, multiresult1, multiresult2;
    uint32_t leftover1, leftover2;
    uint32_t threshold1, threshold2;
    random64bit =  pcg64_random();
    // first part
    random32bit = random64bit & 0xFFFFFFFF;
    multiresult1 = random32bit * range1;
    leftover1 = (uint32_t) multiresult1;
    random32bit = random64bit >> 32;
    multiresult2 = random32bit * range2;
    leftover2 = (uint32_t) multiresult2;
    if((leftover1 < range1 )||(leftover2 < range2)) {
        if(leftover1 < range1 ) {
            threshold1 = -range1 % range1 ;
            while (leftover1 < threshold1) {
                random32bit =  pcg32_random();
                multiresult1 = random32bit * range1;
                leftover1 = (uint32_t) multiresult1;
            }
        }
        if(leftover2 < range2 ) {
            threshold2 = -range2 % range2 ;
            while (leftover2 < threshold2) {
                random32bit =  pcg32_random();
                multiresult2 = random32bit * range2;
                leftover2 = (uint32_t) multiresult2;
            }
        }
    }
    * output1 = multiresult1 >> 32; // [0, range1)
    * output2 = multiresult2 >> 32; // [0, range2)
}

__attribute__((always_inline))
static inline void xorshift128plus_random_bounded_divisionless_two_by_two(uint32_t range1, uint32_t range2, uint32_t *output1, uint32_t *output2) {
    uint64_t random64bit, random32bit, multiresult1, multiresult2;
    uint32_t leftover1, leftover2;
    uint32_t threshold;
    random64bit =  xorshift128plus();
    // first part
    random32bit = random64bit & 0xFFFFFFFF;
    multiresult1 = random32bit * range1;
    leftover1 = (uint32_t) multiresult1;
    random32bit = random64bit >> 32;
    multiresult2 = random32bit * range2;
    leftover2 = (uint32_t) multiresult2;
    if((leftover1 < range1 ) || (leftover2 < range2 )) {
        if(leftover1 < range1 ) {
            threshold = -range1 % range1 ;
            while (leftover1 < threshold) {
                random32bit =  (uint32_t) xorshift128plus();
                multiresult1 = random32bit * range1;
                leftover1 = (uint32_t) multiresult1;
            }
        }
        // second part
        if(leftover2 < range2 ) {
            threshold = -range2 % range2 ;
            while (leftover2 < threshold) {
                random32bit =  (uint32_t) xorshift128plus();
                multiresult2 = random32bit * range2;
                leftover2 = (uint32_t) multiresult2;
            }
        }
    }
    * output1 = multiresult1 >> 32; // [0, range1)
    * output2 = multiresult2 >> 32; // [0, range2)
}

#ifdef USE_EXPECT
#define COMPILER_EXPECT_FALSE(x) __builtin_expect((x), 0)
#define COMPILER_EXPECT_TRUE(x) __builtin_expect((x), 1)
#else
#define COMPILER_EXPECT_FALSE(x) (x)
#define COMPILER_EXPECT_TRUE(x) (x)
#endif

// this simplified version contains just one major branch/loop. For
//powers of two or large range, it is suboptimal.
static inline uint32_t pcg32_random_bounded_divisionless_expect(uint32_t range) {
    uint64_t random32bit, multiresult;
    uint32_t leftover;
    uint32_t threshold;
    random32bit =  pcg32_random();
    multiresult = random32bit * range;
    leftover = (uint32_t) multiresult;
    if(COMPILER_EXPECT_FALSE(leftover < range)) {
        threshold = -range % range ;
        while (COMPILER_EXPECT_FALSE(leftover < threshold)) {
            random32bit =  pcg32_random();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
        }
    }
    return multiresult >> 32; // [0, range)
}

// good old Fisher-Yates shuffle, shuffling an array of integers, uses the default pgc with a modulo (not fair!)
void  shuffle_broken_modulo(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random() % (i+1);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}



// good old Fisher-Yates shuffle, shuffling an array of integers, uses the default pgc ranged rng
void  shuffle_pcg(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded(i);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, uses java-like ranged rng
void  shuffle_pcg_java(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = java_random_bounded(i);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}
// good old Fisher-Yates shuffle, shuffling an array of integers, without division
void  shuffle_pcg_divisionless_fancy(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded_divisionless_fancy(i);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}

// good old Fisher-Yates shuffle, shuffling an array of integers, without division
void  shuffle_pcg_divisionless(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded_divisionless(i);
        value_t tmp = storage[i-1];// likely in cache
        value_t val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}
// good old Fisher-Yates shuffle, shuffling an array of integers, without division
void  shuffle_pcg_divisionless_two_by_two(value_t *storage, uint32_t size) {
    uint32_t i;
    uint32_t nextpos1, nextpos2;
    value_t tmp, val;
    for (i=size; i>2; i-=2) {
        pcg32_random_bounded_divisionless_two_by_two(i,i-1, &nextpos1, &nextpos2);
        tmp = storage[i-1];// likely in cache
        val = storage[nextpos1]; // could be costly
        storage[i - 1] = val;
        storage[nextpos1] = tmp; // you might have to read this store later
        //
        tmp = storage[i-2];// likely in cache
        val = storage[nextpos2]; // could be costly
        storage[i - 2] = val;
        storage[nextpos2] = tmp; // you might have to read this store later
    }
    if(i>1) {// could be optimized
        uint32_t nextpos = pcg32_random_bounded_divisionless(i);
        value_t tmp = storage[i-1];// likely in cache
        value_t val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}
void  shuffle_xorshift128plus_divisionless_two_by_two(value_t *storage, uint32_t size) {
    uint32_t i;
    uint32_t nextpos1, nextpos2;
    value_t tmp, val;
    for (i=size; i>2; i-=2) {
        xorshift128plus_random_bounded_divisionless_two_by_two(i,i-1, &nextpos1, &nextpos2);
        tmp = storage[i-1];// likely in cache
        val = storage[nextpos1]; // could be costly
        storage[i - 1] = val;
        storage[nextpos1] = tmp; // you might have to read this store later
        //
        tmp = storage[i-2];// likely in cache
        val = storage[nextpos2]; // could be costly
        storage[i - 2] = val;
        storage[nextpos2] = tmp; // you might have to read this store later
    }
    if(i>1) {// could be optimized
        uint32_t nextpos = xorshift128plus_random_bounded_divisionless(i);
        value_t tmp = storage[i-1];// likely in cache
        value_t val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}


void  shuffle_pcg_divisionless_expect(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded_divisionless_expect(i);
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}

void populateRandom_randunbounded(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   rand();
    }
}

void populateRandom_randmod(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   rand()%size;
    }
}

void populateRandom_pcgunbounded(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   pcg32_random();
    }
}

void populateRandom_pcg64unbounded(uint32_t * answer, uint32_t size) {
    uint32_t  i=size;
    while (i>2) {
        * (uint64_t * )(answer + size - i) = pcg64_random();
        i-=2;
    }
    if(i > 1)
        answer[size-i] =   pcg32_random();
}


void populateRandom_xorshift128plusunbounded(uint32_t * answer, uint32_t size) {
    uint32_t  i=size;
    while (i>2) {
        * (uint64_t * )(answer + size - i) = xorshift128plus();
        i-=2;
    }
    if(i > 1)
        answer[size-i] =   (uint32_t) xorshift128plus();
}

void populateRandom_pcg(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   pcg32_random_bounded(i);
    }
}
void populateRandom_java(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   java_random_bounded(i);
    }
}
void populateRandom_divisionless(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   pcg32_random_bounded_divisionless(i);
    }
}
void populateRandom_divisionless_fancy(uint32_t * answer, uint32_t size) {
    for (uint32_t  i=size; i>1; i--) {
        answer[size-i] =   pcg32_random_bounded_divisionless_fancy(i);
    }
}


uint32_t * precomputeRandom(uint32_t size) {
    uint32_t * answer = (uint32_t*) malloc(size * sizeof(uint32_t));
    populateRandom_divisionless(answer,size);
    return answer;
}


// good old Fisher-Yates shuffle, shuffling an array of integers, using pre-populated random numbers
void  shuffle_pre(value_t *storage, uint32_t size, uint32_t * randomsource) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = *randomsource;
        randomsource++;
        int tmp = storage[i-1];// likely in cache
        int val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
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


/**
Sorting stuff

*/

int qsort_compare_value_t(const void *a, const void *b) {
    return ( *(value_t *)a - *(value_t *)b );
}
value_t *create_sorted_array(size_t length) {
    value_t *array = malloc(length * sizeof(value_t));
    for (size_t i = 0; i < length; i++) array[i] = (value_t) pcg32_random();
    qsort(array, length, sizeof(*array), qsort_compare_value_t);
    return array;
}

value_t *create_random_array(size_t count) {
    value_t *targets = malloc(count * sizeof(value_t));
    for (size_t i = 0; i < count; i++) targets[i] = (value_t) (pcg32_random() & 0x7FFFFFF);
    return targets;
}




// flushes the array from cache
void array_cache_flush(value_t* B, int32_t length) {
    const int32_t CACHELINESIZE = 64;// 64 bytes per cache line
    for(int32_t  k = 0; k < length; k += CACHELINESIZE/sizeof(value_t)) {
        __builtin_ia32_clflush(B + k);
    }
}

// tries to put the array in cache
void array_cache_prefetch(value_t* B, int32_t length) {
    const int32_t CACHELINESIZE = 64;// 64 bytes per cache line
    for(int32_t  k = 0; k < length; k += CACHELINESIZE/sizeof(value_t)) {
        __builtin_prefetch(B + k);
    }
}



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



void print(value_t * orig, uint32_t size) {
    for(uint32_t k = 0; k < size; ++k)
        printf("%iu ",orig[k]);
}
int sortAndCompare(value_t * shuf, value_t * orig, uint32_t size) {
    qsort(shuf, size, sizeof(value_t), qsort_compare_value_t);
    qsort(orig, size, sizeof(value_t), qsort_compare_value_t);
    for(uint32_t k = 0; k < size; ++k)
        if(shuf[k] != orig[k]) {
            printf("[bug]\n");
            return -1;
        }
    return 0;
}

void demo(int size) {
    printf("Shuffling arrays of size %d \n",size);
    printf("Time reported in number of cycles per array element.\n");
    printf("Tests assume that array is in cache as much as possible.\n");
    int repeat = 500;
    value_t * testvalues = create_random_array(size);
    value_t * pristinecopy = malloc(size * sizeof(value_t));
    memcpy(pristinecopy,testvalues,sizeof(value_t) * size);
    uint32_t * prec = precomputeRandom(size);
    printf("\nFirst, we just generate the random numbers: \n");
    BEST_TIME(populateRandom_randunbounded(prec,size),, repeat, size);
    BEST_TIME(populateRandom_randmod(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcgunbounded(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg64unbounded(prec,size),, repeat, size);
    BEST_TIME(populateRandom_xorshift128plusunbounded(prec,size),, repeat, size);
    BEST_TIME(populateRandom_pcg(prec,size),, repeat, size);
    BEST_TIME(populateRandom_java(prec,size),, repeat, size);
    BEST_TIME(populateRandom_divisionless(prec,size),, repeat, size);
    BEST_TIME(populateRandom_divisionless_fancy(prec,size),, repeat, size);
    printf("\nNext we do the actual shuffle: \n");
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_java(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_divisionless(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_divisionless_two_by_two(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_xorshift128plus_divisionless_two_by_two(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_divisionless_expect(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_divisionless_fancy(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_broken_modulo(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pre(testvalues,size,prec), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    free(testvalues);
    free(pristinecopy);
    free(prec);
    printf("\n");
}

int main() {
    demo(10000);
    return 0;
}
