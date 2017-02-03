// gcc -O3 -o hadd hadd.c  -march=native
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "benchmark.h"
#include <x86intrin.h>

uint32_t hadd32(__m256i vector) {
  uint32_t buffer[sizeof(__m256i)/sizeof(uint32_t)];
 _mm256_store_si256((__m256i *)buffer , vector);
 return buffer[0]+buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5]+buffer[6]+buffer[7];
}

uint32_t _mm256_hadd2_epi32(__m256i a) {
    __m256i a_hi;
    a_hi = _mm256_permute2x128_si256(a, a, 1); 
    a = _mm256_hadd_epi32(a, a_hi);
    a = _mm256_hadd_epi32(a, a);
    a = _mm256_hadd_epi32(a, a);
    return _mm256_extract_epi32(a,0);
}

uint32_t long_hadd32(__m256i vector) {
   __m256i rotated = _mm256_permute2x128_si256(vector, vector, 1); 
  vector = _mm256_add_epi32(vector,rotated);
  rotated = _mm256_alignr_epi8(vector, vector, 2 *sizeof(uint32_t));
  vector = _mm256_add_epi32(vector,rotated);
  rotated = _mm256_alignr_epi8(vector, vector, sizeof(uint32_t));
  vector = _mm256_add_epi32(vector,rotated);
  return _mm256_extract_epi32(vector,0);
}


int main() {
  __m256i input = _mm256_set_epi32(10,2,30,4,5,6,7,8);
  printf("%d %d %d \n",hadd32(input),_mm256_hadd2_epi32(input),fast_hadd32(input) );
  const int N = 1;
  const int repeat = 500;
  const uint32_t expected = hadd32(input);
  BEST_TIME(hadd32(input),expected,, repeat, N, true);
 BEST_TIME(_mm256_hadd2_epi32(input),expected,, repeat, N, true);
  BEST_TIME(long_hadd32(input),expected,, repeat, N, true);
}
