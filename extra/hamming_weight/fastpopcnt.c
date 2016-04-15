#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include <x86intrin.h>



// straight out of wikipedia
static inline uint64_t scalar_hamming_weight(uint64_t x) {
    x -= (x >> 1) & 0x5555555555555555;
    x = ((x>>2)&0x3333333333333333) + (x&0x3333333333333333);
    x += x >> 4;
    x &= 0x0f0f0f0f0f0f0f0f;
    x *= 0x0101010101010101;
    return x >> 56;
}


/**
 * Attributed to Cédric Lauradoux
 */
int lauradoux_bitset64_weight(const uint64_t *fp, int size) {
    const uint64_t m1  = UINT64_C(0x5555555555555555);
    const uint64_t m2  = UINT64_C(0x3333333333333333);
    const uint64_t m4  = UINT64_C(0x0F0F0F0F0F0F0F0F);
    const uint64_t m8  = UINT64_C(0x00FF00FF00FF00FF);
    const uint64_t m16 = UINT64_C(0x0000FFFF0000FFFF);
    const uint64_t h01 = UINT64_C(0x0101010101010101);

    uint64_t count1, count2, half1, half2, acc;
    uint64_t x;
    int i, j;
    int limit = size - size % 12;
    int bit_count = 0;
    for (i = 0; i < limit; i += 12, fp += 12) {
        acc = 0;
        for (j = 0; j < 12; j += 3) {
            count1  =  fp[j + 0];
            count2  =  fp[j + 1];
            half1   =  fp[j + 2];
            half2   =  fp[j + 2];
            half1  &=  m1;
            half2   = (half2  >> 1) & m1;
            count1 -= (count1 >> 1) & m1;
            count2 -= (count2 >> 1) & m1;
            count1 +=  half1;
            count2 +=  half2;
            count1  = (count1 & m2) + ((count1 >> 2) & m2);
            count1 += (count2 & m2) + ((count2 >> 2) & m2);
            acc    += (count1 & m4) + ((count1 >> 4) & m4);
        }
        acc = (acc & m8) + ((acc >>  8)  & m8);
        acc = (acc       +  (acc >> 16)) & m16;
        acc =  acc       +  (acc >> 32);
        bit_count += (int) acc;
    }
    for (i = 0; i < size - limit; i++) {
        x = fp[i];
        x =  x       - ((x >> 1)  & m1);
        x = (x & m2) + ((x >> 2)  & m2);
        x = (x       +  (x >> 4)) & m4;
        bit_count += (int) ((x * h01) >> 56);
    }
    return bit_count;
}



// compute the Hamming weight of the an array of 64-bit words using a scalar Hamming weight function
int scalar_bitset64_weight(const uint64_t * input, size_t length) {
    int card = 0;
    for(size_t k = 0; k < length; k++) {
        card += scalar_hamming_weight(input[k]);
    }
    return card;
}

// compute the Hamming weight of an array of 64-bit words using the popcnt instruction
int popcnt_bitset64_weight(const uint64_t * input, size_t length) {
    int card = 0;
    for(size_t k = 0; k < length; k++) {
        card += _mm_popcnt_u64(input[k]);
    }
    return card;
}


// weights of the 8-bit values
static uint8_t small_table[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};

// compute the Hamming weight of an array of 8-bit words using the small table look-ups
int table_bitset8_weight(const uint8_t * input, size_t length) {
    int card = 0;
    for(size_t k = 0; k < length; k++) {
        card += small_table[input[k]];
    }
    return card;
}

#include "bigtable.c"


// compute the Hamming weight of an array of 16-bit words using the big table look-ups
int table_bitset16_weight(const uint16_t * input, size_t length) {
    int card = 0;
    for(size_t k = 0; k < length; k++) {
        card += big_table[input[k]];
    }
    return card;
}


// compute the Hamming weight of an array of 64-bit words using unrolled popcnt instructions
int unrolled_popcnt_bitset64_weight(const uint64_t * input, size_t length) {
    int card = 0;
    for(size_t k = 0; k < length; k+=4) {
        card += _mm_popcnt_u64(input[k]);
        card += _mm_popcnt_u64(input[k+1]);
        card += _mm_popcnt_u64(input[k+2]);
        card += _mm_popcnt_u64(input[k+3]);
    }
    return card;
}

// compute the Hamming weight of an array of 64-bit words using AVX2 instructions
int avx2_bitset64_weight(const uint64_t * array, size_t length) {
    // these are precomputed hamming weights (weight(0), weight(1)...)
    const __m256i shuf =
        _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 0, 1,
                         1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    const __m256i mask = _mm256_set1_epi8(0x0f);  // low 4 bits of each byte
    __m256i total = _mm256_setzero_si256();
    __m256i zero = _mm256_setzero_si256();
    const int inner = 4;  // length of the inner loop, could go up to 8 safely
    const int outer = length * sizeof(uint64_t) /
                      (sizeof(__m256i) * inner);  // length of outer loop
    for (int k = 0; k < outer; k++) {
        __m256i innertotal = _mm256_setzero_si256();
        for (int i = 0; i < inner; ++i) {
            __m256i ymm1 =
                _mm256_lddqu_si256((const __m256i *)array + k * inner + i);
            __m256i ymm2 =
                _mm256_srli_epi32(ymm1, 4);  // shift right, shiftingin zeroes
            ymm1 = _mm256_and_si256(ymm1, mask);  // contains even 4 bits
            ymm2 = _mm256_and_si256(ymm2, mask);  // contains odd 4 bits
            ymm1 = _mm256_shuffle_epi8(
                       shuf, ymm1);  // use table look-up to sum the 4 bits
            ymm2 = _mm256_shuffle_epi8(shuf, ymm2);
            innertotal = _mm256_add_epi8(innertotal, ymm1);  // inner total
            // values in each
            // byte are bounded
            // by 8 * inner
            innertotal = _mm256_add_epi8(innertotal, ymm2);  // inner total
            // values in each
            // byte are bounded
            // by 8 * inner
        }
        innertotal = _mm256_sad_epu8(zero, innertotal);  // produces 4 64-bit
        // counters (having
        // values in [0,8 *
        // inner * 4])
        total = _mm256_add_epi64(
                    total,
                    innertotal);  // add the 4 64-bit counters to previous counter
    }
    int leftoverwords =  length % (inner * sizeof(__m256i) / sizeof(uint64_t));
    int leftover = popcnt_bitset64_weight(array + length - leftoverwords , leftoverwords);
    return leftover + _mm256_extract_epi64(total, 0) + _mm256_extract_epi64(total, 1) +
           _mm256_extract_epi64(total, 2) + _mm256_extract_epi64(total, 3);
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
#define BEST_TIME(test, expected, pre, repeat, size)                         \
        do {                                                              \
            printf("%s: ", #test);                                        \
            fflush(NULL);                                                 \
            uint64_t cycles_start, cycles_final, cycles_diff;             \
            uint64_t min_diff = (uint64_t)-1;                             \
            for (int i = 0; i < repeat; i++) {                            \
                pre;                                                       \
                __asm volatile("" ::: /* pretend to clobber */ "memory"); \
                RDTSC_START(cycles_start);                                \
                if(test != expected) {printf("not expected (%d , %d )",test,expected);break;}                     \
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
    printf("size = %d words or %lu bytes \n",size,  size*sizeof(uint64_t));
    int repeat = 500;
    uint64_t * prec = malloc(size * sizeof(uint64_t));
    for(int k = 0; k < size; ++k) prec[k] = -k;
    int expected = scalar_bitset64_weight(prec,size);

    BEST_TIME(lauradoux_bitset64_weight(prec,size),expected,, repeat, size);
    BEST_TIME(scalar_bitset64_weight(prec,size),expected,, repeat, size);
    BEST_TIME(popcnt_bitset64_weight(prec,size),expected,, repeat, size);
    BEST_TIME(table_bitset8_weight((uint8_t*)prec,size*8),expected,, repeat, size);
    BEST_TIME(table_bitset16_weight((uint16_t*)prec,size*4),expected,, repeat, size);
    BEST_TIME(unrolled_popcnt_bitset64_weight(prec,size),expected,, repeat, size);
    BEST_TIME(avx2_bitset64_weight(prec,size),expected,, repeat, size);

    free(prec);
    printf("\n");
}

int main() {
    demo(16);
    demo(32);
    demo(64);
    demo(128);

    demo(5000);
    return 0;
}
