#ifndef MKLARQVIST_H
#define MKLARQVIST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

// from https://github.com/mklarqvist/FastFlagStats
void flag_stats_avx2(const uint16_t *data, uint32_t n, uint32_t *flags) {

  __m256i masks[16];
  __m256i counters[16];
  for (int i = 0; i < 16; ++i) {
    masks[i] = _mm256_set1_epi16(1 << i);
    counters[i] = _mm256_set1_epi16(0);
  }
  uint32_t out_counters[16] = {0};

  const __m256i hi_mask = _mm256_set1_epi32(0xFFFF0000);
  const __m256i lo_mask = _mm256_set1_epi32(0x0000FFFF);
  const __m256i *data_vectors = reinterpret_cast<const __m256i *>(data);
  const uint32_t n_cycles = n / 16;
  const uint32_t n_update_cycles = n_cycles / 65536;

#define UPDATE(idx)                                                            \
  counters[idx] = _mm256_add_epi16(                                            \
      counters[idx],                                                           \
      _mm256_srli_epi16(                                                       \
          _mm256_and_si256(_mm256_lddqu_si256(data_vectors + pos),             \
                           masks[idx]),                                        \
          idx))
#define ITERATION                                                              \
  {                                                                            \
    UPDATE(0);                                                                 \
    UPDATE(1);                                                                 \
    UPDATE(2);                                                                 \
    UPDATE(3);                                                                 \
    UPDATE(4);                                                                 \
    UPDATE(5);                                                                 \
    UPDATE(6);                                                                 \
    UPDATE(7);                                                                 \
    UPDATE(8);                                                                 \
    UPDATE(9);                                                                 \
    UPDATE(10);                                                                \
    UPDATE(11);                                                                \
    UPDATE(12);                                                                \
    UPDATE(13);                                                                \
    UPDATE(14);                                                                \
    UPDATE(15);                                                                \
    ++pos;                                                                     \
    ++k;                                                                       \
  }
  uint32_t pos = 0;
  for (uint32_t i = 0; i < n_update_cycles; ++i) { // each block of 2^16 values
    for (int k = 0; k < 65536;) // max sum of each 16-bit value in a register
      ITERATION                 // unrolled

          // Compute vector sum
          for (int k = 0; k < 16; ++k) { // each flag register
        // Accumulator
        // ((16-bit high & 16 high) >> 16) + (16-bit low & 16-low)
        __m256i x = _mm256_add_epi32(
            _mm256_srli_epi32(_mm256_and_si256(counters[k], hi_mask), 16),
            _mm256_and_si256(counters[k], lo_mask));
        __m256i t1 = _mm256_hadd_epi32(x, x);
        __m256i t2 = _mm256_hadd_epi32(t1, t1);
        __m128i t4 = _mm_add_epi32(_mm256_castsi256_si128(t2),
                                   _mm256_extractf128_si256(t2, 1));
        out_counters[k] += _mm_cvtsi128_si32(t4);

        counters[k] = _mm256_set1_epi16(0);
      }
  }

  // residual
  for (uint32_t i = pos * 16; i < n; ++i) {
    for (int j = 0; j < 16; ++j)
      out_counters[j] += ((data[i] & (1 << j)) >> j);
  }

  for (int i = 0; i < 16; ++i)
    flags[i] = out_counters[i];

#undef ITERATION
#undef UPDATE
}

// from https://github.com/mklarqvist/FastFlagStats
void flag_stats_avx2_naive_counter(const uint16_t *data, uint32_t n,
                                   uint32_t *flags) {
  __m256i masks[16];
  __m256i counters[16];
  for (int i = 0; i < 16; ++i) {
    masks[i] = _mm256_set1_epi16(1 << i);
    counters[i] = _mm256_set1_epi16(0);
  }
  uint32_t out_counters[16] = {0};

  const __m256i *data_vectors = reinterpret_cast<const __m256i *>(data);
  const uint32_t n_cycles = n / 16;
  const uint32_t n_update_cycles = n_cycles / 65536;

#define UPDATE(idx)                                                            \
  counters[idx] = _mm256_add_epi16(                                            \
      counters[idx],                                                           \
      _mm256_srli_epi16(                                                       \
          _mm256_and_si256(_mm256_lddqu_si256(data_vectors + pos),             \
                           masks[idx]),                                        \
          idx))

  uint32_t pos = 0;
  for (uint32_t i = 0; i < n_update_cycles; ++i) { // each block of 2^16 values
    for (int k = 0; k < 65536;
         ++pos, ++k) {             // max sum of each 16-bit value in a register
      for (int p = 0; p < 16; ++p) // Not unrolled
        UPDATE(p);
    }

    // Compute vector sum
    for (int k = 0; k < 16; ++k) { // each flag register
      // Naive counter
      uint16_t *d = reinterpret_cast<uint16_t *>(&counters[k]);
      for (int j = 0; j < 16; ++j) // each uint16_t in the register
        out_counters[k] += d[j];

      counters[k] = _mm256_set1_epi16(0);
    }
  }

  // residual
  for (uint32_t i = pos * 16; i < n; ++i) {
    for (int j = 0; j < 16; ++j)
      out_counters[j] += ((data[i] & (1 << j)) >> j);
  }

  for (int i = 0; i < 16; ++i)
    flags[i] = out_counters[i];

#undef UPDATE
}

// from https://github.com/mklarqvist/FastFlagStats
void flag_stats_avx2_single(const uint16_t *data, uint32_t n, uint32_t *flags) {
  __m256i counter = _mm256_set1_epi16(0);
  const __m256i one_mask = _mm256_set1_epi16(1);
  // set_epi is parameterized backwards (15->0)
  const __m256i masks = _mm256_set_epi16(
      -32768, 1 << 14, 1 << 13, 1 << 12, 1 << 11, 1 << 10, 1 << 9, 1 << 8,
      1 << 7, 1 << 6, 1 << 5, 1 << 4, 1 << 3, 1 << 2, 1 << 1, 1 << 0);
  uint32_t out_counters[16] = {0};
  const __m256i *data_vectors = reinterpret_cast<const __m256i *>(data);
  const uint32_t n_cycles = n / 16;
  const uint32_t n_update_cycles = n_cycles / 4096;

#define UPDATE(idx)                                                            \
  counter = _mm256_add_epi16(                                                  \
      counter, _mm256_and_si256(                                               \
                   _mm256_cmpeq_epi16(                                         \
                       _mm256_and_si256(                                       \
                           _mm256_set1_epi16(_mm256_extract_epi16(             \
                               _mm256_lddqu_si256(data_vectors + pos), idx)),  \
                           masks),                                             \
                       masks),                                                 \
                   one_mask));
#define BLOCK                                                                  \
  {                                                                            \
    UPDATE(0)                                                                  \
    UPDATE(1) UPDATE(2) UPDATE(3) UPDATE(4) UPDATE(5) UPDATE(6) UPDATE(7)      \
        UPDATE(8) UPDATE(9) UPDATE(10) UPDATE(11) UPDATE(12) UPDATE(13)        \
            UPDATE(14) UPDATE(15)                                              \
  }

  uint32_t pos = 0;
  for (uint32_t i = 0; i < n_update_cycles; ++i) { // each block of 65536 values
    for (int k = 0; k < 4096;
         ++k, ++pos) { // max sum of each 16-bit value in a register (65536/16)
      BLOCK
    }

    // Compute vector sum
    // D. Lemire: extract takes immediate
    out_counters[0] += _mm256_extract_epi16(counter, 0);
    out_counters[1] += _mm256_extract_epi16(counter, 1);
    out_counters[2] += _mm256_extract_epi16(counter, 2);
    out_counters[3] += _mm256_extract_epi16(counter, 3);
    out_counters[4] += _mm256_extract_epi16(counter, 4);
    out_counters[5] += _mm256_extract_epi16(counter, 5);
    out_counters[6] += _mm256_extract_epi16(counter, 6);
    out_counters[7] += _mm256_extract_epi16(counter, 7);
    out_counters[8] += _mm256_extract_epi16(counter, 8);
    out_counters[9] += _mm256_extract_epi16(counter, 9);
    out_counters[10] += _mm256_extract_epi16(counter, 10);
    out_counters[11] += _mm256_extract_epi16(counter, 11);
    out_counters[12] += _mm256_extract_epi16(counter, 12);
    out_counters[13] += _mm256_extract_epi16(counter, 13);
    out_counters[14] += _mm256_extract_epi16(counter, 14);
    out_counters[15] += _mm256_extract_epi16(counter, 15);
    counter = _mm256_set1_epi16(0);
  }

#undef UPDATE
#undef BLOCK

  // residual
  for (uint32_t i = pos * 16; i < n; ++i) {
    for (int j = 0; j < 16; ++j)
      out_counters[j] += ((data[i] & (1 << j)) >> j);
  }

  for (int i = 0; i < 16; ++i)
    flags[i] = out_counters[i];
}

#endif
