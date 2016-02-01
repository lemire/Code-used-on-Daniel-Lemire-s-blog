// gcc -mavx2 -std=c99 -O3 -o searchtinyarray searchtinyarray.c -Wall -Wextra

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
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
    for (size_t i = 0; i < count; i++) targets[i] = (value_t) pcg32_random();
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

// could be faster, but we just want it to be correct
int32_t ilog2(size_t lenarray)  {
    int32_t low = 0;
    int32_t high = (int32_t) lenarray - 1;
    int32_t counter = 0;
    while( low <= high) {
        int32_t middleIndex = (low+high) >> 1;
        high = middleIndex - 1;
        ++counter;
    }
    return counter;
}
int32_t __attribute__ ((noinline)) search(uint32_t * array, uint32_t length, uint32_t target )  {
    for(int k = 0; k < 8; ++k)
        if(array[k] == target) return k;
    return -1;
}

int32_t __attribute__ ((noinline)) unrolled_search(uint32_t * array, uint32_t length, uint32_t target )  {
    if(array[0] == target) return 0;
    if(array[1] == target) return 1;
    if(array[2] == target) return 2;
    if(array[3] == target) return 3;
    if(array[4] == target) return 4;
    if(array[5] == target) return 5;
    if(array[6] == target) return 6;
    if(array[7] == target) return 7;
    return -1;
}


int32_t __attribute__ ((noinline)) simd_search(uint32_t * array, uint32_t length, uint32_t target )  {
    __m256i varray = _mm256_lddqu_si256((const __m256i *)array);
    __m256i vtarget = _mm256_set1_epi32(target);
    __m256i m = _mm256_cmpeq_epi32(varray,vtarget);
    int32_t bits = _mm256_movemask_epi8(m);
    return (__builtin_ffs(bits) >> 2) - 1;
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
 * This is like BEST_TIME except that ... it runs functions "test" using the
 * first parameter "base" and various parameters from "testvalues" (there
 * are nbrtestvalues), calling pre on base between tests
 */
#define BEST_TIME_PRE_ARRAY(base, length, test, pre,   testvalues, nbrtestvalues, cycle_per_op, bogus)        \
    do {                                                                                \
        fflush(NULL);                                                                   \
        uint64_t cycles_start, cycles_final, cycles_diff;                               \
        uint64_t min_diff = (uint64_t)-1;                                               \
        int sum = 0;                                                                    \
        for (size_t j = 0; j < nbrtestvalues; j++) {                                    \
            pre(base,length);                                                           \
            pre(base,length);                                                           \
            __asm volatile("" ::: /* pretend to clobber */ "memory");                   \
            RDTSC_START(cycles_start);                                                  \
            bogus += test(base,length,testvalues[j]);                                            \
            RDTSC_FINAL(cycles_final);                                                  \
            cycles_diff = (cycles_final - cycles_start);                                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;                         \
            sum += cycles_diff;                                                           \
        }                                                                               \
        uint64_t S = nbrtestvalues;                                                     \
        cycle_per_op = sum / (double)S;                                                 \
    } while (0)

#define ASSERT_PRE_ARRAY(base, length, test,   testvalues, nbrtestvalues)               \
    do {                                                                                \
        int error = 0;                                                                  \
        for (size_t j = 0; j < nbrtestvalues; j++) {                                    \
            int32_t re = test(base,length,testvalues[j]);                               \
            if(re >= 0) {                                                               \
              if(base[re] != testvalues[j]) {                                           \
                error=1; break;                                                         \
              }                                                                         \
            } else {                                                                    \
               int32_t ip = -re - 1;                                                    \
               if(ip < (int32_t) length) {                                              \
                 if(base[ip] <= testvalues[j]) {                                        \
                   printf(" Index returned is %d which points at %d for target %d. ",ip,base[ip],testvalues[j]);\
                   error = 3;                                                           \
                   break;                                                               \
                 }                                                                      \
               }                                                                        \
               if( ip > 0 ){                                                            \
                 if(base[ip - 1] >= testvalues[j]) {                                    \
                   printf(" Index returned is %d which points at %d for target %d. ",ip,base[ip],testvalues[j]);\
                  printf(" Previous index is %d which points at %d for target %d. ",ip-1,base[ip-1],testvalues[j]);\
                   error = 4;                                                           \
                   break;                                                               \
                 }                                                                      \
               }                                                                        \
            }                                                                           \
        }                                                                               \
        if(error != 0) printf("[%s error: %d]\n",#test,error);                     \
     } while (0)






void demo() {
    size_t nbrtestvalues = 10000;
    value_t * testvalues = create_random_array(nbrtestvalues);
    printf("# searching in unsorted arrays of 8 32-bit integers\n");
    int32_t bogus = 0;
    size_t N = sizeof(__m256i)/sizeof(uint32_t);
    value_t * source = create_random_array(N);
    float cposearch, cpousearch, cposimdsearch;
    BEST_TIME_PRE_ARRAY(source, N, search, array_cache_prefetch,   testvalues, nbrtestvalues, cposearch, bogus);
 

    BEST_TIME_PRE_ARRAY(source, N, unrolled_search, array_cache_prefetch,   testvalues, nbrtestvalues, cpousearch, bogus);
    BEST_TIME_PRE_ARRAY(source, N, simd_search, array_cache_prefetch,   testvalues, nbrtestvalues, cposimdsearch, bogus);
    printf("times (clock per value) scalar = %2.f unrolled scalar =  %2.f   AVX2  = %.2f \n",cposearch, cpousearch,cposimdsearch);
    free(source);
    printf("\n");
    printf("bogus = %d \n",bogus);
    free(testvalues);
}
int main() {
    demo();
    return 0;
}
