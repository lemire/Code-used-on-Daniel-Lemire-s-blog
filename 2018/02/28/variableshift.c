// gcc -O3 -o variableshift variableshift.c  -march=native
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <math.h>

__attribute__ ((noinline))
size_t scalarshift(uint32_t *array, size_t length, int shiftamount) {
  for (size_t k = 0; k < length; ++k) {
    array[k] = array[k] >> shiftamount;
  }
  return 0;
}

#define SHIFTAMOUNT 3
__attribute__ ((noinline))
size_t vectorshift(uint32_t *array, size_t length, int shiftamount) {
  size_t k = 0;
  __m256i * a = (__m256i *) array;
  for (; k  < length / 8 ; k ++, a++) {
    __m256i v = _mm256_loadu_si256(a);
    v = _mm256_srli_epi32(v,SHIFTAMOUNT);
     _mm256_storeu_si256(a,v);
  }
  k *= 8;
  for (; k < length; ++k) {
    array[k] = array[k] >> SHIFTAMOUNT;
  }
  return 0;
}
__attribute__ ((noinline))
size_t vectorshift_unrolled(uint32_t *array, size_t length, int shiftamount) {
  size_t k = 0;
  __m256i * a = (__m256i *) array;
  for (; k +3  < length / 8 ; k +=4, a+=4) {
    __m256i v1 = _mm256_loadu_si256(a);
    __m256i v2 = _mm256_loadu_si256(a + 1);
    __m256i v3 = _mm256_loadu_si256(a + 2);
    __m256i v4 = _mm256_loadu_si256(a + 3);

    v1 = _mm256_srli_epi32(v1,SHIFTAMOUNT);
    v2 = _mm256_srli_epi32(v2,SHIFTAMOUNT);
    v3 = _mm256_srli_epi32(v3,SHIFTAMOUNT);
    v4 = _mm256_srli_epi32(v4,SHIFTAMOUNT);

     _mm256_storeu_si256(a,v1);
     _mm256_storeu_si256(a + 1,v2);
     _mm256_storeu_si256(a + 2,v3);
     _mm256_storeu_si256(a + 3,v4);
  }

  for (; k  < length / 8 ; k ++, a++) {
    array[k] = array[k] >> shiftamount;
    __m256i v = _mm256_loadu_si256(a);
    v = _mm256_srli_epi32(v,SHIFTAMOUNT);
     _mm256_storeu_si256(a,v);
  }
  k *= 8;
  for (; k < length; ++k) {
    array[k] = array[k] >> SHIFTAMOUNT;
  }
  return 0;
}

#undef SHIFTAMOUNT


__attribute__ ((noinline))
size_t varvectorshift(uint32_t *array, size_t length, int shiftamount) {
  size_t k = 0;
  __m256i * a = (__m256i *) array;
  __m128i s = _mm_set1_epi32(shiftamount);
  for (; k  < length / 8 ; k ++, a++) {
    __m256i v = _mm256_loadu_si256(a);
    v = _mm256_srl_epi32(v,s);
     _mm256_storeu_si256(a,v);
  }
  k *= 8;
  for (; k < length; ++k) {
    array[k] = array[k] >> shiftamount;
  }
  return 0;
}
size_t varvectorshift_unrolled(uint32_t *array, size_t length, int shiftamount) {
  size_t k = 0;
  __m256i * a = (__m256i *) array;
  __m128i s = _mm_set1_epi32(shiftamount);
  for (; k + 3 < length / 8 ; k +=4, a+=4) {
    __m256i v1 = _mm256_loadu_si256(a);
    __m256i v2 = _mm256_loadu_si256(a + 1);
    __m256i v3 = _mm256_loadu_si256(a + 2);
    __m256i v4 = _mm256_loadu_si256(a + 3);

    v1 = _mm256_srl_epi32(v1,s);
    v2 = _mm256_srl_epi32(v2,s);
    v3 = _mm256_srl_epi32(v3,s);
    v4 = _mm256_srl_epi32(v4,s);

     _mm256_storeu_si256(a,v1);
     _mm256_storeu_si256(a + 1,v2);
     _mm256_storeu_si256(a + 2,v3);
     _mm256_storeu_si256(a + 3,v4);

  }
  for (; k  < length / 8 ; k ++, a++) {
    __m256i v = _mm256_loadu_si256(a);
    v = _mm256_srl_epi32(v,s);
     _mm256_storeu_si256(a,v);
  }
  k *= 8;
  for (; k < length; ++k) {
    array[k] = array[k] >> shiftamount;
  }
  return 0;
}


void randominit(uint32_t *array, size_t length) {
  for (size_t k = 0; k < length; ++k) {
    array[k] = rand();
  }
}

void array_shifting() {
  printf("[array shifting]\n");
  int repeat = 5;
  size_t N = 1000;
  uint32_t *array = malloc(N * sizeof(uint32_t));
  randominit(array, N);
  const int shiftamount = 3;
  BEST_TIME(scalarshift(array, N, shiftamount), 0, randominit(array, N), repeat,
      N, true);
  BEST_TIME(vectorshift(array, N, shiftamount), 0, randominit(array, N), repeat,
      N, true);
  BEST_TIME(vectorshift_unrolled(array, N, shiftamount), 0, randominit(array, N), repeat,
      N, true);

  BEST_TIME(varvectorshift(array, N, shiftamount), 0, randominit(array, N), repeat,
      N, true);
  BEST_TIME(varvectorshift_unrolled(array, N, shiftamount), 0, randominit(array, N), repeat,
      N, true);

  free(array);
}

int main() {
  array_shifting();
  return EXIT_SUCCESS;
}
