//  gcc -O3 -o avxpop avxpop.c -march=native -Wall -Wextra
#include "benchmark.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>

__m256i popcount_pshufb(__m256i v) {

  const __m256i lookup = _mm256_setr_epi8(
      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4,

      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4);

  const __m256i low_mask = _mm256_set1_epi8(0x0f);

  const __m256i lo = _mm256_and_si256(v, low_mask);
  const __m256i hi = _mm256_and_si256(_mm256_srli_epi16(v, 4), low_mask);
  const __m256i popcnt1 = _mm256_shuffle_epi8(lookup, lo);
  const __m256i popcnt2 = _mm256_shuffle_epi8(lookup, hi);

  return _mm256_sad_epu8(_mm256_add_epi8(popcnt1, popcnt2),
                         _mm256_setzero_si256());
}

__m256i popcount_pshufb32(__m256i v) {

  const __m256i lookup = _mm256_setr_epi8(
      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4,

      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4);

  const __m256i low_mask = _mm256_set1_epi8(0x0f);

  const __m256i lo = _mm256_and_si256(v, low_mask);
  const __m256i hi = _mm256_and_si256(_mm256_srli_epi16(v, 4), low_mask);
  const __m256i popcnt1 = _mm256_shuffle_epi8(lookup, lo);
  const __m256i popcnt2 = _mm256_shuffle_epi8(lookup, hi);

  __m256i sum8 = _mm256_add_epi8(popcnt1, popcnt2);
  return _mm256_srli_epi32(
      _mm256_mullo_epi32(sum8, _mm256_set1_epi32(0x01010101)), 24);
}
__m256i popcount_pshufb32b(__m256i v) {

  const __m256i lookup = _mm256_setr_epi8(
      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4,

      /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
      /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
      /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
      /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4);

  const __m256i low_mask = _mm256_set1_epi8(0x0f);

  const __m256i lo = _mm256_and_si256(v, low_mask);
  const __m256i hi = _mm256_and_si256(_mm256_srli_epi16(v, 4), low_mask);
  const __m256i popcnt1 = _mm256_shuffle_epi8(lookup, lo);
  const __m256i popcnt2 = _mm256_shuffle_epi8(lookup, hi);

  __m256i sum8 = _mm256_add_epi8(popcnt1, popcnt2);
  return _mm256_madd_epi16(_mm256_maddubs_epi16(sum8,_mm256_set1_epi8(1)),_mm256_set1_epi16(1));
}



uint64_t basic(uint64_t *a, size_t N) {
  uint64_t sum = 0;
  for (size_t i = 0; i < N; i++) {
    sum += _mm_popcnt_u64(a[i]);
  }
  return sum;
}

uint64_t shuft(uint64_t *a, size_t N) {
  assert(N / 4 * 4 == N);
  __m256i sum = _mm256_setzero_si256();
  for (size_t i = 0; i < N - 3; i += 4) {
    __m256i v = _mm256_lddqu_si256((const __m256i *)(a + i));
    sum = _mm256_add_epi64(sum, popcount_pshufb(v));
  }
  uint64_t buffer[4];
  _mm256_storeu_si256((__m256i *)buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3];
}

uint64_t shuft32(uint64_t *a, size_t N) {
  assert(N / 4 * 4 == N);
  __m256i sum = _mm256_setzero_si256();
  for (size_t i = 0; i < N - 3; i += 4) {
    __m256i v = _mm256_lddqu_si256((const __m256i *)(a + i));
    sum = _mm256_add_epi32(sum, popcount_pshufb32(v));
  }
  uint32_t buffer[8];
  _mm256_storeu_si256((__m256i *)buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7];
}

uint64_t shuft32b(uint64_t *a, size_t N) {
  assert(N / 4 * 4 == N);
  __m256i sum = _mm256_setzero_si256();
  for (size_t i = 0; i < N - 3; i += 4) {
    __m256i v = _mm256_lddqu_si256((const __m256i *)(a + i));
    sum = _mm256_add_epi32(sum, popcount_pshufb32b(v));
  }
  uint32_t buffer[8];
  _mm256_storeu_si256((__m256i *)buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7];
}

int main() {
  const size_t N = 1024 * 1024;
  uint64_t *a = malloc(N * sizeof(uint64_t));
  for (size_t k = 0; k < N; ++k) {
    a[k] = k;
  }
  uint64_t expected = basic(a, N);

  const int repeat = 500;
  BEST_TIME(basic(a, N), expected, , repeat, N, true);
  BEST_TIME(shuft(a, N), expected, , repeat, N, true);
  BEST_TIME(shuft32(a, N), expected, , repeat, N, true);
  BEST_TIME(shuft32b(a, N), expected, , repeat, N, true);

  free(a);
}
