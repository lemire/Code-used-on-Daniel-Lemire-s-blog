// gcc -march=native -std=c99 -O3 -o shuffle shuffle.c -Wall -Wextra

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <x86intrin.h>

typedef uint32_t value_t;

struct pcg_state_setseq_64 {    // Internals are *Private*.
    uint64_t state;             // RNG state.  All values are possible.
    uint64_t inc;               // Controls which RNG sequence (stream) is
    // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;


static pcg32_random_t pcg32_global = { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL };


static inline uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline uint32_t pcg32_random() {
    return pcg32_random_r(&pcg32_global);
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
void  shuffle_pcg_divisionless(value_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded_divisionless(i);
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
    BEST_TIME(populateRandom_pcg(prec,size),, repeat, size);
    BEST_TIME(populateRandom_java(prec,size),, repeat, size);
    BEST_TIME(populateRandom_divisionless(prec,size),, repeat, size);
    printf("\nNext we do the actual shuffle: \n");
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_java(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pcg_divisionless(testvalues,size), array_cache_prefetch(testvalues,size), repeat, size);
    if(sortAndCompare(testvalues, pristinecopy, size)!=0) return;
    BEST_TIME(shuffle_pre(testvalues,size,prec), array_cache_prefetch(testvalues,size), repeat, size);
    free(testvalues);
    free(prec);
    free(pristinecopy);
    printf("\n");
}

int main() {
    demo(10000);
    return 0;
}
