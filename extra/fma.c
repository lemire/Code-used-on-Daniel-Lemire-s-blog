// gcc -fno-inline -std=gnu99 -Wall -O3 -g -march=native  -mavx fma.c -o fma

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <math.h>

#ifndef SIZE
#define SIZE 4096
#endif

#ifndef REPEAT
#define REPEAT 100000
#endif

#define RDTSC_START(cycles)                                     \
    do {                                                        \
        register unsigned cyc_high, cyc_low;                    \
        __asm volatile("cpuid\n\t"                              \
                     "rdtsc\n\t"                                \
                     "mov %%edx, %0\n\t"                        \
                     "mov %%eax, %1\n\t"                        \
                     : "=r" (cyc_high), "=r" (cyc_low)          \
                     :: "%rax", "%rbx", "%rcx", "%rdx");        \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;        \
    } while (0)

#define RDTSC_FINAL(cycles)                                     \
    do {                                                        \
        register unsigned cyc_high, cyc_low;                    \
        __asm volatile("rdtscp\n\t"                             \
                     "mov %%edx, %0\n\t"                        \
                     "mov %%eax, %1\n\t"                        \
                     "cpuid\n\t"                                \
                     : "=r" (cyc_high), "=r" (cyc_low)          \
                     :: "%rax", "%rbx", "%rcx", "%rdx");        \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;        \
    } while(0)


#define BEST_TIME(test, answer)                                         \
    do {                                                                \
        printf("%s: ", #test);                                          \
        fflush(NULL);                                                   \
        uint64_t cycles_start, cycles_final, cycles_diff;               \
        uint64_t min_diff = (uint64_t) -1;                              \
        int wrong_answer = 0;                                           \
        for (int i = 0; i < REPEAT; i++) {                              \
            __asm volatile (""::: /* pretend to clobber */ "memory");   \
            RDTSC_START(cycles_start);                                  \
            float result = test;                                        \
            RDTSC_FINAL(cycles_final);                                  \
            if (result != answer) wrong_answer = 1;                     \
            cycles_diff = (cycles_final - cycles_start);                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;         \
        }                                                               \
        float floats_per_cycle = (SIZE) / (float) min_diff;             \
        printf(" %.2f floats/cycle", floats_per_cycle);                 \
        if (wrong_answer) printf(" [ERROR]");                           \
        printf("\n");                                                   \
        fflush(NULL);                                                   \
    } while (0)


#define VEC_LOAD_OFFSET_BASE(load, offset, base)                        \
    __asm volatile ("vmovups %c1(%2), %0":                              \
                    "=x" (load):  /* xmm or ymm destination register */ \
                    "i" (offset), /* constant array offset in bytes */  \
                    "r" (base)    /* read only memory location */       \
                    )

#define VEC_LOAD_OFFSET_BASE_CL(load, offset, base)                        \
    __asm volatile ("vlddqu %c1(%2), %0":                              \
                    "=x" (load):  /* xmm or ymm destination register */ \
                    "i" (offset), /* constant array offset in bytes */  \
                    "r" (base)    /* read only memory location */       \
                    )
                    


#define VEC_FMA_SUM_MULT_OFFSET_BASE(sum, mult, offset, base)           \
   __asm volatile ("vfmadd231ps %c2(%3), %1, %0":                       \
                   "+x" (sum):   /* sum = sum + (mult * [mem]) */       \
                   "x" (mult),   /* xmm or ymm vec of floats */         \
                   "i" (offset), /* constant array offset in bytes */   \
                   "r" (base)    /* read only memory location */        \
                   )

typedef __m256 ymm_t;
typedef __m128 xmm_t;

float fma_scalar(float *array1, float *array2, size_t size) {
    float total = 0.0;
    for (size_t i = 0; i < size; i++) {
        float sum = array1[i] * array2[i];
        total += sum;
    }
    return total;
}

float fma_xmm(float *array1, float *array2, size_t size) {

    xmm_t sum1 = {0};
    xmm_t sum2 = {0};
    xmm_t sum3 = {0};
    xmm_t sum4 = {0};

    if (size % 16 != 0) return NAN;
    float *end2 = array2 + size;
    while (array2 < end2) {
        xmm_t mult1, mult2, mult3, mult4;
        VEC_LOAD_OFFSET_BASE(mult1, 0, array1);
        VEC_LOAD_OFFSET_BASE(mult2, 16, array1);
        VEC_LOAD_OFFSET_BASE(mult3, 32, array1);
        VEC_LOAD_OFFSET_BASE(mult4, 48, array1);

        VEC_FMA_SUM_MULT_OFFSET_BASE(sum1, mult1, 0, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum2, mult2, 16, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum3, mult3, 32, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum4, mult4, 48, array2);

        array1 += 16;
        array2 += 16;
    }

    sum1 = _mm_add_ps(sum1, sum2);
    sum3 = _mm_add_ps(sum3, sum4);
    sum1 = _mm_add_ps(sum1, sum3);
    xmm_t r2 = _mm_hadd_ps(sum1, sum1);
    xmm_t r3 = _mm_hadd_ps(r2, r2);
    float total = _mm_cvtss_f32(r3);

    return total;
}

float fma_ymm(float *array1, float *array2, size_t size) {

    ymm_t sum1 = {0, 0};
    ymm_t sum2 = {0, 0};
    ymm_t sum3 = {0, 0};
    ymm_t sum4 = {0, 0};

    if (size % 32 != 0) return NAN;
    float *end2 = array2 + size;
    while (array2 < end2) {
        ymm_t mult1, mult2, mult3, mult4;
        VEC_LOAD_OFFSET_BASE(mult1, 0, array1);
        VEC_LOAD_OFFSET_BASE(mult2, 32, array1);
        VEC_LOAD_OFFSET_BASE(mult3, 64, array1);
        VEC_LOAD_OFFSET_BASE(mult4, 96, array1);

        VEC_FMA_SUM_MULT_OFFSET_BASE(sum1, mult1, 0, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum2, mult2, 32, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum3, mult3, 64, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum4, mult4, 96, array2);

        array1 += 32;
        array2 += 32;
    }

    sum1 = _mm256_add_ps(sum1, sum2);
    sum3 = _mm256_add_ps(sum3, sum4);
    sum1 = _mm256_add_ps(sum1, sum3);
    ymm_t r2 = _mm256_hadd_ps(sum1, sum1);
    ymm_t r3 = _mm256_hadd_ps(r2, r2);
    ymm_t r4 = _mm256_hadd_ps(r3, r3);
    float total = _mm_cvtss_f32(_mm256_extractf128_ps(r4,0));

    return total;
}

float fma_ymm2(float *array1, float *array2, size_t size) {

    ymm_t sum1 = {0, 0};
    ymm_t sum2 = {0, 0};
    ymm_t sum3 = {0, 0};
    ymm_t sum4 = {0, 0};
    ymm_t sum5 = {0, 0};
    ymm_t sum6 = {0, 0};
    ymm_t sum7 = {0, 0};
    ymm_t sum8 = {0, 0};


    if (size % 64 != 0) return NAN;
    float *end2 = array2 + size;
    while (array2 < end2) {
        ymm_t mult1, mult2, mult3, mult4, mult5, mult6, mult7, mult8;
        VEC_LOAD_OFFSET_BASE(mult1, 0, array1);
        VEC_LOAD_OFFSET_BASE(mult2, 32, array1);
        VEC_LOAD_OFFSET_BASE(mult3, 64, array1);
        VEC_LOAD_OFFSET_BASE(mult4, 96, array1);
        VEC_LOAD_OFFSET_BASE(mult5, 128, array1);
        VEC_LOAD_OFFSET_BASE(mult6, 160, array1);
        VEC_LOAD_OFFSET_BASE(mult7, 192, array1);
        VEC_LOAD_OFFSET_BASE(mult8, 224, array1);


        VEC_FMA_SUM_MULT_OFFSET_BASE(sum1, mult1, 0, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum2, mult2, 32, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum3, mult3, 64, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum4, mult4, 96, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum5, mult5, 128, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum6, mult6, 160, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum7, mult7, 192, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum8, mult8, 224, array2);


        array1 += 64;
        array2 += 64;
    }

    sum1 = _mm256_add_ps(sum1, sum2);
    sum3 = _mm256_add_ps(sum3, sum4);
    sum5 = _mm256_add_ps(sum5, sum6);
    sum7 = _mm256_add_ps(sum7, sum8);
    sum1 = _mm256_add_ps(sum1, sum3);
    sum5 = _mm256_add_ps(sum5, sum7);
    sum1 = _mm256_add_ps(sum1, sum5);
    ymm_t r2 = _mm256_hadd_ps(sum1, sum1);
    ymm_t r3 = _mm256_hadd_ps(r2, r2);
    ymm_t r4 = _mm256_hadd_ps(r3, r3);
    float total = _mm_cvtss_f32(_mm256_extractf128_ps(r4,0));

    return total;
}



float fma_xmm_cl(float *array1, float *array2, size_t size) {

    xmm_t sum1 = {0};
    xmm_t sum2 = {0};
    xmm_t sum3 = {0};
    xmm_t sum4 = {0};

    if (size % 16 != 0) return NAN;
    float *end2 = array2 + size;
    while (array2 < end2) {
        xmm_t mult1, mult2, mult3, mult4;
        VEC_LOAD_OFFSET_BASE_CL(mult1, 0, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult2, 16, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult3, 32, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult4, 48, array1);

        VEC_FMA_SUM_MULT_OFFSET_BASE(sum1, mult1, 0, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum2, mult2, 16, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum3, mult3, 32, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum4, mult4, 48, array2);

        array1 += 16;
        array2 += 16;
    }

    sum1 = _mm_add_ps(sum1, sum2);
    sum3 = _mm_add_ps(sum3, sum4);
    sum1 = _mm_add_ps(sum1, sum3);
    xmm_t r2 = _mm_hadd_ps(sum1, sum1);
    xmm_t r3 = _mm_hadd_ps(r2, r2);
    float total = _mm_cvtss_f32(r3);

    return total;
}

float fma_ymm_cl(float *array1, float *array2, size_t size) {

    ymm_t sum1 = {0, 0};
    ymm_t sum2 = {0, 0};
    ymm_t sum3 = {0, 0};
    ymm_t sum4 = {0, 0};

    if (size % 32 != 0) return NAN;
    float *end2 = array2 + size;
    while (array2 < end2) {
        ymm_t mult1, mult2, mult3, mult4;
        VEC_LOAD_OFFSET_BASE_CL(mult1, 0, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult2, 32, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult3, 64, array1);
        VEC_LOAD_OFFSET_BASE_CL(mult4, 96, array1);

        VEC_FMA_SUM_MULT_OFFSET_BASE(sum1, mult1, 0, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum2, mult2, 32, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum3, mult3, 64, array2);
        VEC_FMA_SUM_MULT_OFFSET_BASE(sum4, mult4, 96, array2);

        array1 += 32;
        array2 += 32;
    }

    sum1 = _mm256_add_ps(sum1, sum2);
    sum3 = _mm256_add_ps(sum3, sum4);
    sum1 = _mm256_add_ps(sum1, sum3);
    ymm_t r2 = _mm256_hadd_ps(sum1, sum1);
    ymm_t r3 = _mm256_hadd_ps(r2, r2);
    ymm_t r4 = _mm256_hadd_ps(r3, r3);
    float total = _mm_cvtss_f32(_mm256_extractf128_ps(r4,0));

    return total;
}

// modified from http://stackoverflow.com/questions/196329/osx-lacks-memalign
void *aligned_malloc(int align , size_t size ) {
    void *mem = malloc( size + align + sizeof(void*) );
    char *amem = ((char*)mem) + sizeof(void*);
    amem += align - ((uintptr_t)amem & (align - 1));
    ((void**)amem)[-1] = mem;
    return amem;
}

// we try to make sure that the alignment is no better than what is requested
void *offsetted_aligned_malloc(int align , size_t size ) {
    void *mem = malloc( size + 2 * align + sizeof(void*) );
    char *amem = ((char*)mem) + sizeof(void*);
    amem += align - ((uintptr_t)amem & (align - 1));
    if(((uintptr_t)amem & (2*align - 1)) == 0) amem += align; // could be branchless
    ((void**)amem)[-1] = mem;
    return amem;
}

void aligned_free( void *mem ) {
    free( ((void**)mem)[-1] );
}

void demo(int align) {
    printf("Testing with SIZE=%d...\n", SIZE);
    printf("Testing with align=%d...\n", align);
     size_t size = SIZE;

    float *array1 = offsetted_aligned_malloc(align, SIZE * sizeof(float) + align);
    float *array2 = offsetted_aligned_malloc(align, SIZE * sizeof(float) + align);
    if(((uintptr_t)array1 & (align - 1)) != 0) {
        printf("insufficient align 1\n");
    }
    if(((uintptr_t)array2 & (align - 1)) != 0) {
        printf("insufficient align 2\n");
    }
    if(((uintptr_t)array1 & (2*align - 1)) == 0) {
        printf("Too much alignment 1\n");
    }
    if(((uintptr_t)array2 & (2*align - 1)) == 0) {
        printf("Too much alignment 2\n");
    }
    for (size_t i = 0; i < size; i++) {
        array1[i] = 1.0;
        array2[i] = 2.0;
    }
    float answer = fma_scalar(array1, array2, size);

    BEST_TIME(fma_scalar(array1, array2, size), answer);
    BEST_TIME(fma_xmm(array1, array2, size), answer);
    BEST_TIME(fma_ymm(array1, array2, size), answer);
    BEST_TIME(fma_ymm2(array1, array2, size), answer);
    BEST_TIME(fma_xmm_cl(array1, array2, size), answer);
    BEST_TIME(fma_ymm_cl(array1, array2, size), answer);

    aligned_free(array1);
    aligned_free(array2);
}

int main(int argc, char **argv) {
  demo(1);
  demo(8);
  demo(sizeof(xmm_t));
  demo(sizeof(ymm_t));
  demo(2*sizeof(ymm_t));
}
