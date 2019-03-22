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

void scalar_morenaive(const uint16_t *data, size_t n, uint32_t *flags) {
  memset(flags, 0, 16 * sizeof(uint32_t));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += (data[i]  >> j) & 1;
    }
  }
}

void flag_stats_avx2(const uint16_t*  data, uint32_t n, uint32_t*  flags) {

    __m256i masks[16];
    __m256i counters[16];
    for(int i = 0; i < 16; ++i) {
        masks[i]    = _mm256_set1_epi16(1 << i);
        counters[i] = _mm256_set1_epi16(0);
    }
    uint32_t out_counters[16] = {0};

    const __m256i hi_mask = _mm256_set1_epi32(0xFFFF0000);
    const __m256i lo_mask = _mm256_set1_epi32(0x0000FFFF);
    const __m256i* data_vectors = reinterpret_cast<const __m256i*>(data);
    const uint32_t n_cycles = n / 16;
    const uint32_t n_update_cycles = n_cycles / 65536;

#define UPDATE(idx) counters[idx]  = _mm256_add_epi16(counters[idx],  _mm256_srli_epi16(_mm256_and_si256(data_vectors[pos], masks[idx]),  idx))
#define ITERATION  {                                   \
        UPDATE(0);  UPDATE(1);  UPDATE(2);  UPDATE(3); \
        UPDATE(4);  UPDATE(5);  UPDATE(6);  UPDATE(7); \
        UPDATE(8);  UPDATE(9);  UPDATE(10); UPDATE(11);\
        UPDATE(12); UPDATE(13); UPDATE(14); UPDATE(15);\
        ++pos; ++k;                                    \
}
    uint32_t pos = 0;
    for(int i = 0; i < n_update_cycles; ++i) { // each block of 2^16 values
        for(int k = 0; k < 65536; ) // max sum of each 16-bit value in a register
            ITERATION // unrolled

        // Compute vector sum
        for(int k = 0; k < 16; ++k) { // each flag register
            // Accumulator
            // ((16-bit high & 16 high) >> 16) + (16-bit low & 16-low)
            __m256i x = _mm256_add_epi32(
                           _mm256_srli_epi32(_mm256_and_si256(counters[k], hi_mask), 16),
                           _mm256_and_si256(counters[k], lo_mask));
            __m256i t1 = _mm256_hadd_epi32(x,x);
            __m256i t2 = _mm256_hadd_epi32(t1,t1);
            __m128i t4 = _mm_add_epi32(_mm256_castsi256_si128(t2),_mm256_extractf128_si256(t2,1));
            out_counters[k] += _mm_cvtsi128_si32(t4);

            /*
            // Naive counter
            uint16_t* d = reinterpret_cast<uint16_t*>(&counters[k]);
            for(int j = 0; j < 16; ++j) { // each uint16_t in the register
                out_counters[k] += d[j];
            }
            */

            counters[k] = _mm256_set1_epi16(0);
        }
    }

    // residual
    for(int i = pos*16; i < n; ++i) {
        for(int j = 0; j < 16; ++j)
            out_counters[j] += ((data[i] & (1 << j)) >> j);
    }


    for(int i = 0; i < 16; ++i) flags[i] = out_counters[i];

    //std::cerr << "simd=";
    //for(int i = 0; i < 16; ++i) std::cerr << " " << out_counters[i];
    //std::cerr << std::endl;

#undef ITERATION
#undef UPDATE


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
  printf("\n");

  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");
  BEST_TIME_NOCHECK(scalar_morenaive(array, len, counter), , repeat, len, true);
  printf("\n");

  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

  BEST_TIME_NOCHECK(fastavx2(array, len, counter), , repeat, len, true);
  printf("\n");

  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");

  BEST_TIME_NOCHECK(flag_stats_avx2(array, len, counter), , repeat, len, true);
  printf("\n");


  for (size_t i = 0; i < 16; i++) {
    printf("%u ", counter[i]);
  }
  printf("\n");
}
