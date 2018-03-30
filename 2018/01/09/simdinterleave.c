// credit : Geoff Langdale
// gcc -O3 -o simdinterleave simdinterleave.c -march=native  && ./simdinterleave
#include "benchmark.h"
#include <assert.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>

static inline uint64_t interleave_uint32_with_zeros(uint32_t input) {
  uint64_t word = input;
  word = (word ^ (word << 16)) & 0x0000ffff0000ffff;
  word = (word ^ (word << 8)) & 0x00ff00ff00ff00ff;
  word = (word ^ (word << 4)) & 0x0f0f0f0f0f0f0f0f;
  word = (word ^ (word << 2)) & 0x3333333333333333;
  word = (word ^ (word << 1)) & 0x5555555555555555;
  return word;
}

static inline uint32_t deinterleave_lowuint32(uint64_t word) {
  word &= 0x5555555555555555;
  word = (word ^ (word >> 1)) & 0x3333333333333333;
  word = (word ^ (word >> 2)) & 0x0f0f0f0f0f0f0f0f;
  word = (word ^ (word >> 4)) & 0x00ff00ff00ff00ff;
  word = (word ^ (word >> 8)) & 0x0000ffff0000ffff;
  word = (word ^ (word >> 16)) & 0x00000000ffffffff;
  return (uint32_t)word;
}

typedef struct {
  uint32_t x;
  uint32_t y;
} uint32_2;

uint64_t interleave(uint32_2 input) {
  return interleave_uint32_with_zeros(input.x) |
         (interleave_uint32_with_zeros(input.y) << 1);
}

void interleave_array(uint32_2 *input, size_t length, uint64_t *out) {
  for (size_t i = 0; i < length; i++) {
    out[i] = interleave(input[i]);
  }
}

_Static_assert(sizeof(uint32_2) == sizeof(uint64_t), "assert1");

static inline __m256i interleave_uint8_with_zeros_avx(__m256i word) {
  const __m256i m3 = _mm256_set1_epi64x(0x0f0f0f0f0f0f0f0f);
  const __m256i m4 = _mm256_set1_epi64x(0x3333333333333333);
  const __m256i m5 = _mm256_set1_epi64x(0x5555555555555555);
  word = _mm256_xor_si256(word, _mm256_slli_epi16(word, 4));
  word = _mm256_and_si256(word, m3);
  word = _mm256_xor_si256(word, _mm256_slli_epi16(word, 2));
  word = _mm256_and_si256(word, m4);
  word = _mm256_xor_si256(word, _mm256_slli_epi16(word, 1));
  word = _mm256_and_si256(word, m5);
  return word;
}

static inline __m256i interleave_uint8_with_zeros_avx_lut(__m256i word) {
  const __m256i m = _mm256_set_epi8(85, 84, 81, 80, 69, 68, 65, 64, 21, 20, 17,
                                    16, 5, 4, 1, 0, 85, 84, 81, 80, 69, 68, 65,
                                    64, 21, 20, 17, 16, 5, 4, 1, 0);
  __m256i lownibbles =
      _mm256_shuffle_epi8(m, _mm256_and_si256(word, _mm256_set1_epi8(0xf)));
  __m256i highnibbles = _mm256_and_si256(word, _mm256_set1_epi8(0xf0));
  highnibbles = _mm256_srli_epi16(highnibbles, 4);
  highnibbles = _mm256_shuffle_epi8(m, highnibbles);
  highnibbles = _mm256_slli_epi16(highnibbles, 8);
  return _mm256_or_si256(lownibbles, highnibbles);
}

static inline __m256i interleave_uint8_with_zeros_avx_lut1(__m256i word) {
  const __m256i m = _mm256_set_epi8(
      (char)170, (char)168, (char)162, (char)160, (char)138, (char)136,
      (char)130, (char)128, 42, 40, 34, 32, 10, 8, 2, 0, (char)170, (char)168,
      (char)162, (char)160, (char)138, (char)136, (char)130, (char)128, 42, 40,
      34, 32, 10, 8, 2, 0);
  __m256i lownibbles =
      _mm256_shuffle_epi8(m, _mm256_and_si256(word, _mm256_set1_epi8(0xf)));
  __m256i highnibbles = _mm256_and_si256(word, _mm256_set1_epi8(0xf0));
  highnibbles = _mm256_srli_epi16(highnibbles, 4);
  highnibbles = _mm256_shuffle_epi8(m, highnibbles);
  highnibbles = _mm256_slli_epi16(highnibbles, 8);
  return _mm256_or_si256(lownibbles, highnibbles);
}

static inline __m256i interleave_zeroes_with_uint8_avx(__m256i word) {
  const __m256i m3 = _mm256_set1_epi64x(0xf0f0f0f0f0f0f0f0);
  const __m256i m4 = _mm256_set1_epi64x(0xCCCCCCCCCCCCCCCC);
  const __m256i m5 = _mm256_set1_epi64x(0xAAAAAAAAAAAAAAAA);
  word = _mm256_xor_si256(word, _mm256_srli_epi16(word, 4));
  word = _mm256_and_si256(word, m3);
  word = _mm256_xor_si256(word, _mm256_srli_epi16(word, 2));
  word = _mm256_and_si256(word, m4);
  word = _mm256_xor_si256(word, _mm256_srli_epi16(word, 1));
  word = _mm256_and_si256(word, m5);
  return word;
}

static inline void interleave_avx2(uint32_2 *input, uint64_t *out) {
  __m256i xy = _mm256_lddqu_si256((const __m256i *)input);
  __m256i justx = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(-1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1, -1,
                          0, -1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1,
                          -1, 0));
  __m256i justy = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6, -1, 5, -1,
                          4, -1, 15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6, -1,
                          5, -1, 4, -1));
  justx = interleave_uint8_with_zeros_avx(justx);
  justy = interleave_zeroes_with_uint8_avx(justy);
  __m256i answer = _mm256_or_si256(justx, justy);
  _mm256_storeu_si256((__m256i *)out, answer);
}

static inline void interleave_avx2_fast(uint32_2 *input, uint64_t *out) {
  __m256i xy = _mm256_lddqu_si256((const __m256i *)input);
  __m256i justx = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(-1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1, -1,
                          0, -1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1,
                          -1, 0));
  __m256i justy = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(-1, 15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6, -1, 5,
                          -1, 4, -1, 15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6,
                          -1, 5, -1, 4));
  justx = interleave_uint8_with_zeros_avx_lut(justx);
  justy = interleave_uint8_with_zeros_avx_lut1(justy);
  __m256i answer = _mm256_or_si256(justx, justy);
  _mm256_storeu_si256((__m256i *)out, answer);
}

static inline void interleave_avx2_short(uint32_2 *input, uint64_t *out) {
  __m256i xy = _mm256_lddqu_si256((const __m256i *)input);
  __m256i justx = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(-1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1, -1,
                          0, -1, 11, -1, 10, -1, 9, -1, 8, -1, 3, -1, 2, -1, 1,
                          -1, 0));
  __m256i justy = _mm256_shuffle_epi8(
      xy, _mm256_set_epi8(-1, 15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6, -1, 5,
                          -1, 4, -1, 15, -1, 14, -1, 13, -1, 12, -1, 7, -1, 6,
                          -1, 5, -1, 4));
  justx = interleave_uint8_with_zeros_avx(justx);
  justy = interleave_uint8_with_zeros_avx(justy);
  __m256i answer = _mm256_or_si256(justx, _mm256_slli_epi16(justy, 1));
  _mm256_storeu_si256((__m256i *)out, answer);
}

void interleave_avx_array(uint32_2 *input, size_t length, uint64_t *out) {
  size_t i = 0;
  for (; i + 3 < length; i += 4) {
    interleave_avx2(input + i, out + i);
  }

  for (; i < length; i++) {
    out[i] = interleave(input[i]);
  }
}
void interleave_avx_fast_array(uint32_2 *input, size_t length, uint64_t *out) {
  size_t i = 0;
  for (; i + 3 < length; i += 4) {
    interleave_avx2_fast(input + i, out + i);
  }

  for (; i < length; i++) {
    out[i] = interleave(input[i]);
  }
}

void interleave_avx_short_array(uint32_2 *input, size_t length, uint64_t *out) {
  size_t i = 0;
  for (; i + 3 < length; i += 4) {
    interleave_avx2_short(input + i, out + i);
  }

  for (; i < length; i++) {
    out[i] = interleave(input[i]);
  }
}

uint32_2 deinterleave(uint64_t input) {
  uint32_2 answer;
  answer.x = deinterleave_lowuint32(input);
  answer.y = deinterleave_lowuint32(input >> 1);
  return answer;
}

void deinterleave_array(uint64_t *input, size_t length, uint32_2 *out) {
  for (size_t i = 0; i < length; i++) {
    out[i] = deinterleave(input[i]);
  }
}

static inline uint64_t interleave_uint32_with_zeros_pdep(uint32_t input) {
  return _pdep_u64(input, 0x5555555555555555);
}

static inline uint32_t deinterleave_lowuint32_pext(uint64_t word) {
  return (uint32_t)_pext_u64(word, 0x5555555555555555);
}

uint64_t interleave_pdep(uint32_2 input) {
  return _pdep_u64(input.x, 0x5555555555555555) |
         _pdep_u64(input.y, 0xaaaaaaaaaaaaaaaa);
}

void interleave_array_pdep(uint32_2 *input, size_t length, uint64_t *out) {
  for (size_t i = 0; i < length; i++) {
    out[i] = interleave_pdep(input[i]);
  }
}

uint32_2 deinterleave_pext(uint64_t input) {
  uint32_2 answer;
  answer.x = _pext_u64(input, 0x5555555555555555);
  answer.y = _pext_u64(input, 0xaaaaaaaaaaaaaaaa);
  return answer;
}

void deinterleave_array_pext(uint64_t *input, size_t length, uint32_2 *out) {
  for (size_t i = 0; i < length; i++) {
    out[i] = deinterleave_pext(input[i]);
  }
}

void demo(size_t N) {
  printf("N = %zu \n", N);
  uint32_2 *array = (uint32_2 *)malloc(N * sizeof(uint32_2));
  uint64_t *storedarray = (uint64_t *)malloc(N * sizeof(uint64_t));
  for (uint32_t k = 0; k < N; k++) {
    array[k].x = k;
    array[k].y = k + 1;
  }
  int repeat = 5;
  BEST_TIME_NOCHECK(interleave_array(array, N, storedarray), , repeat, N, true);
  BEST_TIME_NOCHECK(deinterleave_array(storedarray, N, array), , repeat, N,
                    true);
  for (uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k + 1);
  }

  BEST_TIME_NOCHECK(interleave_array_pdep(array, N, storedarray), , repeat, N,
                    true);
  BEST_TIME_NOCHECK(deinterleave_array_pext(storedarray, N, array), , repeat, N,
                    true);
  for (uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k + 1);
  }
  BEST_TIME_NOCHECK(interleave_avx_array(array, N, storedarray), , repeat, N,
                    true);
  BEST_TIME_NOCHECK(deinterleave_array(storedarray, N, array), , repeat, N,
                    true);
  for (uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k + 1);
  }

  BEST_TIME_NOCHECK(interleave_avx_short_array(array, N, storedarray), , repeat,
                    N, true);
  BEST_TIME_NOCHECK(deinterleave_array(storedarray, N, array), , repeat, N,
                    true);
  for (uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k + 1);
  }
  BEST_TIME_NOCHECK(interleave_avx_fast_array(array, N, storedarray), , repeat,
                    N, true);
  BEST_TIME_NOCHECK(deinterleave_array(storedarray, N, array), , repeat, N,
                    true);
  for (uint32_t k = 0; k < N; k++) {
    assert(array[k].x == k);
    assert(array[k].y == k + 1);
  }

  free(array);
  free(storedarray);
}
int main() {
  demo(1000);
  return 0;
}
