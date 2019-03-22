#include <stdio.h>
#include <string.h>
#include <x86intrin.h>
#include "benchmark.h"

void scalar_naive(const uint16_t *data, size_t n, uint32_t *flags) {
  memset(flags, 0, 16 * sizeof(uint32_t));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += ((data[i] & (1 << j)) >> j);
    }
  }
}

// fixme
void fastavx2(uint16_t *array, size_t len, uint32_t *flags) {
  for (size_t i = 0; i < 16; i++)
    flags[i] = 0;
  uint16_t buffer[16];
  __m256i bits = _mm256_set_epi16(-32768, 16384, 8192, 4096, 2048, 1024, 512,
                                  256, 128, 64, 32, 16, 8, 4, 2, 1);
  for (size_t i = 0; i + 32 <= len;) {
    __m256i count16 = _mm256_setzero_si256();
    size_t j = 0;
    size_t maxj = i + 65535 + 32 <= len ? 65535 : len - i;
    for (; j + 3 < maxj; j += 4) {
      __m256i input1 = _mm256_loadu_si256((__m256i *)(array + i + j));
      __m256i m1 = _mm256_and_si256(input1, bits);
      __m256i eq1 = _mm256_cmpeq_epi16(bits, m1);
      count16 = _mm256_sub_epi16(count16, eq1);
      __m256i input2 = _mm256_loadu_si256((__m256i *)(array + i + j + 1));
      __m256i m2 = _mm256_and_si256(input2, bits);
      __m256i eq2 = _mm256_cmpeq_epi16(bits, m2);
      count16 = _mm256_sub_epi16(count16, eq1);
      __m256i input3 = _mm256_loadu_si256((__m256i *)(array + i + j + 2));
      __m256i m3 = _mm256_and_si256(input3, bits);
      __m256i eq3 = _mm256_cmpeq_epi16(bits, m3);
      count16 = _mm256_sub_epi16(count16, eq3);
      __m256i input4 = _mm256_loadu_si256((__m256i *)(array + i + j + 3));
      __m256i m4 = _mm256_and_si256(input4, bits);
      __m256i eq4 = _mm256_cmpeq_epi16(bits, m4);
      count16 = _mm256_sub_epi16(count16, eq4);

    }
    for (; j < maxj; j += 1) {
      __m256i input = _mm256_loadu_si256((__m256i *)(array + i + j));
      __m256i m = _mm256_and_si256(input, bits);
      __m256i eq = _mm256_cmpeq_epi16(bits, m);
      count16 = _mm256_sub_epi16(count16, eq);
    }
    i += j;
    _mm256_storeu_si256((__m256i *)buffer, count16);
    for (size_t k = 0; k < 16; k++) {
      flags[k] += buffer[k];
    }
  }
}

int main() {
  uint32_t counter[16];
  size_t len = 1000000;
  uint16_t *array = (uint16_t *)malloc(len * sizeof(uint16_t));
  memset(array, 0, len * sizeof(uint16_t));
  for (size_t i = 32; i + 32 < len; i++) {
    array[i] = i & 0xFFF;
  }
  scalar_naive(array, len, counter);

  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

  fastavx2(array, len, counter);

  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

  int repeat = 10;
  BEST_TIME_NOCHECK(scalar_naive(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

  BEST_TIME_NOCHECK(fastavx2(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

}
