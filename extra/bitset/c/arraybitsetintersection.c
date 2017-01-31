/**
 gcc -O3 -o arraybitsetintersection arraybitsetintersection.c -std=c99
*/

#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint64_t bitset_contains(const uint64_t *bitset,
                                        uint16_t pos) {
    const uint64_t word = bitset[pos >> 6];
    return (word >> (pos & 63)) & 1;
}





#define ASM_INPLACESHIFT_RIGHT(srcReg, bitsReg)  \
    __asm volatile("shrx %1, %0, %0"             \
                   : "+r"(srcReg)                \
                   :            /* read/write */ \
                   "r"(bitsReg) /* read only */  \
                   )

static inline uint64_t asm_bitset_contains(const uint64_t *bitset,
                                        uint16_t pos) {
    uint64_t word = bitset[pos >> 6];
    const uint64_t p = pos;
    ASM_INPLACESHIFT_RIGHT(word, p);
    return word & 1;
}

static inline bool bool_asm_bitset_contains(const uint64_t *bitset,
                                        uint16_t pos) {
    uint64_t word = bitset[pos >> 6];
    const uint64_t p = pos;
    ASM_INPLACESHIFT_RIGHT(word, p);
    return word & 1;
}






size_t array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        // could probably be vectorized
        uint16_t key = src_1[i];
        // next bit could be branchless
        if (bitset_contains(src_2, key)) {
            dst[newcard++] = key;
        }
    }
    return newcard;
}

size_t branchless_array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        uint16_t key = src_1[i];
        dst[newcard] = key;
        newcard += bitset_contains(src_2, key);
   }
    return newcard;
}

size_t asm_branchless_array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        uint16_t key = src_1[i];
        dst[newcard] = key;
        newcard += asm_bitset_contains(src_2, key);
   }
    return newcard;
}


size_t bool_asm_branchless_array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        uint16_t key = src_1[i];
        dst[newcard] = key;
        newcard += bool_asm_bitset_contains(src_2, key);
   }
    return newcard;
}


size_t exist_branchless_array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        uint16_t key = src_1[i];
        dst[newcard] = key;
        newcard +=  bool_asm_bitset_contains(src_2, key);
   }
    return newcard;
}





size_t asm_array_bitset_intersection(const uint16_t *src_1, size_t origcard,
                                         const uint64_t *src_2,
                                         uint16_t *dst) {
    size_t newcard = 0;
    for (int i = 0; i < origcard; ++i) {
        // could probably be vectorized
        uint16_t key = src_1[i];
        // next bit could be branchless
        if (asm_bitset_contains(src_2, key)) {
            dst[newcard++] = key;
        }
    }
    return newcard;
}


void randomize(uint16_t * array, size_t N) {
  for(size_t k = 0; k < N; k++) {
    array[k] = rand() ;
  }
}

int main() {
  const int N = 1024;
  uint64_t * bitset = malloc(1024 * sizeof(uint64_t));
  for(size_t k = 0; k < 1024; k++) {
    bitset[k] = UINT64_C(0xaa55ff0000ff55aa);
  }
  uint16_t * array = malloc(sizeof(uint16_t) * N);
  randomize(array,N);
  for(size_t t = 0; t < 1000; t++) {
    randomize(array,N);
    uint16_t * buffer = malloc(sizeof(uint16_t) * N);
    size_t answer1 = array_bitset_intersection(array,N,bitset, buffer);
    size_t answer2 = branchless_array_bitset_intersection(array,N,bitset, buffer);
    size_t answer3 = asm_branchless_array_bitset_intersection(array,N,bitset, buffer);
    size_t answer4 = bool_asm_branchless_array_bitset_intersection(array,N,bitset, buffer);
    size_t answer5 = exist_branchless_array_bitset_intersection(array,N,bitset, buffer);
    size_t answer6 = asm_array_bitset_intersection(array,N,bitset, buffer);
    assert(answer1 == answer2);
    assert(answer2 == answer3);
    assert(answer3 == answer4);
    assert(answer4 == answer5);
    assert(answer5 == answer6);
    free(buffer);
  }
  const int repeat = 50;
  BEST_TIME_NOCHECK(array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  BEST_TIME_NOCHECK(branchless_array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  BEST_TIME_NOCHECK(asm_branchless_array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  BEST_TIME_NOCHECK(bool_asm_branchless_array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  BEST_TIME_NOCHECK(exist_branchless_array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  BEST_TIME_NOCHECK(asm_array_bitset_intersection(array,N,bitset,array), randomize(array,N), repeat, N, true);
  printf("going more predictible\n");
  for(size_t k = 0; k < 1024; k++) {
    bitset[k] = UINT64_C(0xaa55ffffffff55aa); // mostly just ones
  }
  BEST_TIME_NOCHECK(array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  BEST_TIME_NOCHECK(branchless_array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  BEST_TIME_NOCHECK(asm_branchless_array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  BEST_TIME_NOCHECK(bool_asm_branchless_array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  BEST_TIME_NOCHECK(exist_branchless_array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  BEST_TIME_NOCHECK(asm_array_bitset_intersection(array,N,bitset,array), , repeat, N, true);
  free(bitset);
  free(array);
}
