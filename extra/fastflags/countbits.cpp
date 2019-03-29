#include "benchmark.h"
#include "mklarqvist.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x86intrin.h>

void scalar_naive(const uint16_t *data, size_t n, uint32_t *flags) {
  memset(flags, 0, 16 * sizeof(uint32_t));
  for (uint32_t i = 0; i < n; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += ((data[i] & (1 << j)) >> j);
    }
  }
}

void scalar_morenaive(const uint16_t *data, size_t n, uint32_t *flags) {
  memset(flags, 0, 16 * sizeof(uint32_t));
  for (uint32_t i = 0; i < n; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += (data[i] >> j) & 1;
    }
  }
}

static inline void CSA(__m256i *h, __m256i *l, __m256i a, __m256i b,
                       __m256i c) {
  const __m256i u = _mm256_xor_si256(a, b);
  *h = _mm256_or_si256(_mm256_and_si256(a, b), _mm256_and_si256(u, c));
  *l = _mm256_xor_si256(u, c);
}

/**************************************
 * EXPERIMENTAL
 *************************************/
void purecircuit(uint16_t *array, size_t len, uint32_t *flags) {
  for (size_t i = 0; i < 16; i++)
    flags[i] = 0;
  // for the last 256 entries, we do it the scalar way.
  // maybe: move this to the end
  for (uint32_t i = len - (len % (16 * 16)); i < len; ++i) {
    for (int j = 0; j < 16; ++j) {
      flags[j] += ((array[i] & (1 << j)) >> j);
    }
  }
  const __m256i *data = (const __m256i *)array;
  size_t size = len / 16;
  __m256i ones = _mm256_setzero_si256();
  __m256i twos = _mm256_setzero_si256();
  __m256i fours = _mm256_setzero_si256();
  __m256i eights = _mm256_setzero_si256();
  __m256i sixteens = _mm256_setzero_si256();
  __m256i twosA, twosB, foursA, foursB, eightsA, eightsB;

  const uint64_t limit = size - size % 16;
  uint64_t i = 0;

  uint16_t buffer[16];

  // uint16_t x = 0;
  while (i < limit) {
    __m256i counter[16];
    for (size_t i = 0; i < 16; i++) {
      counter[i] = _mm256_setzero_si256();
    }
    size_t thislimit = limit;
    if (thislimit - i >= (1 << 16))
      thislimit = i + (1 << 16) - 1;
    for (; i < thislimit; i += 16) {
      CSA(&twosA, &ones, ones, _mm256_lddqu_si256(data + i),
          _mm256_lddqu_si256(data + i + 1));
      CSA(&twosB, &ones, ones, _mm256_lddqu_si256(data + i + 2),
          _mm256_lddqu_si256(data + i + 3));
      CSA(&foursA, &twos, twos, twosA, twosB);
      CSA(&twosA, &ones, ones, _mm256_lddqu_si256(data + i + 4),
          _mm256_lddqu_si256(data + i + 5));
      CSA(&twosB, &ones, ones, _mm256_lddqu_si256(data + i + 6),
          _mm256_lddqu_si256(data + i + 7));
      CSA(&foursB, &twos, twos, twosA, twosB);
      CSA(&eightsA, &fours, fours, foursA, foursB);
      CSA(&twosA, &ones, ones, _mm256_lddqu_si256(data + i + 8),
          _mm256_lddqu_si256(data + i + 9));
      CSA(&twosB, &ones, ones, _mm256_lddqu_si256(data + i + 10),
          _mm256_lddqu_si256(data + i + 11));
      CSA(&foursA, &twos, twos, twosA, twosB);
      CSA(&twosA, &ones, ones, _mm256_lddqu_si256(data + i + 12),
          _mm256_lddqu_si256(data + i + 13));
      CSA(&twosB, &ones, ones, _mm256_lddqu_si256(data + i + 14),
          _mm256_lddqu_si256(data + i + 15));
      CSA(&foursB, &twos, twos, twosA, twosB);
      CSA(&eightsB, &fours, fours, foursA, foursB);
      CSA(&sixteens, &eights, eights, eightsA, eightsB);
      for (size_t i = 0; i < 16; i++) {
        counter[i] = _mm256_add_epi16(
            counter[i], _mm256_and_si256(sixteens, _mm256_set1_epi16(1)));
        sixteens = _mm256_srli_epi16(sixteens, 1);
      }
    }
    for (size_t i = 0; i < 16; i++) {
      _mm256_storeu_si256((__m256i *)buffer, counter[i]);
      for (size_t z = 0; z < 16; z++) {
        flags[i] += buffer[z] * 16;
      }
    }
  }

  _mm256_storeu_si256((__m256i *)buffer, ones);
  for (size_t i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      flags[j] += ((buffer[i] & (1 << j)) >> j);
    }
  }

  _mm256_storeu_si256((__m256i *)buffer, twos);
  for (size_t i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      flags[j] += 2 * ((buffer[i] & (1 << j)) >> j);
    }
  }
  _mm256_storeu_si256((__m256i *)buffer, fours);
  for (size_t i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      flags[j] += 4 * ((buffer[i] & (1 << j)) >> j);
    }
  }
  _mm256_storeu_si256((__m256i *)buffer, eights);
  for (size_t i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      flags[j] += 8 * ((buffer[i] & (1 << j)) >> j);
    }
  }
}

void fastavx2(uint16_t *array, size_t len, uint32_t *flags) {
  for (size_t i = 0; i < 16; i++)
    flags[i] = 0;
  uint16_t buffer[16];
  __m256i bits = _mm256_set_epi16(-32768, 16384, 8192, 4096, 2048, 1024, 512,
                                  256, 128, 64, 32, 16, 8, 4, 2, 1);
  // we do the first part
  if (len < 64) {
    // don't bother with handcrafted SIMD
    scalar_naive(array, len, flags);
    return;
  }
  // handle the start (naively)
  __m256i count16 = _mm256_setzero_si256();

  {
    uint16_t startbuffer[32];
    memset(startbuffer, 0, 32 * 2);
    memcpy(startbuffer + 16, array, 16 * 2);
    for (size_t i = 1; i < 16; i++) {
      __m256i input = _mm256_loadu_si256((__m256i *)(startbuffer + i));
      __m256i m = _mm256_and_si256(input, bits);
      __m256i eq = _mm256_cmpeq_epi16(bits, m);
      count16 = _mm256_sub_epi16(count16, eq);
    }
  }
  {
    uint16_t startbuffer[32];
    memset(startbuffer, 0, 32 * 2);
    memcpy(startbuffer, array + len - 16, 16 * 2);
    for (size_t i = 1; i < 16; i++) {
      __m256i input = _mm256_loadu_si256((__m256i *)(startbuffer + i));
      __m256i m = _mm256_and_si256(input, bits);
      __m256i eq = _mm256_cmpeq_epi16(bits, m);
      count16 = _mm256_sub_epi16(count16, eq);
    }
  }
  _mm256_storeu_si256((__m256i *)buffer, count16);
  for (size_t k = 0; k < 16; k++) {
    flags[k] += buffer[k];
  }

  // main loop starts here
  for (size_t i = 0; i + 16 <= len;) {
    count16 = _mm256_setzero_si256();
    size_t j = 0;
    size_t maxj = 65535;
    if (maxj + i + 16 >= len)
      maxj = len - i - 15;
    for (; j < maxj; j++) {
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

void morefastavx2(uint16_t *array, size_t len, uint32_t *flags) {
  for (size_t i = 0; i < 16; i++)
    flags[i] = 0;
  uint16_t buffer[16];
  __m256i bits = _mm256_set_epi16(-32768, 16384, 8192, 4096, 2048, 1024, 512,
                                  256, 128, 64, 32, 16, 8, 4, 2, 1);
  // we do the first part
  if (len < 64) {
    // don't bother with handcrafted SIMD
    scalar_naive(array, len, flags);
    return;
  }
  // handle the start (naively)
  __m256i count16 = _mm256_setzero_si256();

  {
    uint16_t startbuffer[32];
    memset(startbuffer, 0, 32 * 2);
    memcpy(startbuffer + 16, array, 16 * 2);
    for (size_t i = 1; i < 16; i++) {
      __m256i input = _mm256_loadu_si256((__m256i *)(startbuffer + i));
      __m256i m = _mm256_and_si256(input, bits);
      __m256i eq = _mm256_cmpeq_epi16(bits, m);
      count16 = _mm256_sub_epi16(count16, eq);
    }
  }
  {
    uint16_t startbuffer[32];
    memset(startbuffer, 0, 32 * 2);
    memcpy(startbuffer, array + len - 16, 16 * 2);
    for (size_t i = 1; i < 16; i++) {
      __m256i input = _mm256_loadu_si256((__m256i *)(startbuffer + i));
      __m256i m = _mm256_and_si256(input, bits);
      __m256i eq = _mm256_cmpeq_epi16(bits, m);
      count16 = _mm256_sub_epi16(count16, eq);
    }
  }
  _mm256_storeu_si256((__m256i *)buffer, count16);
  for (size_t k = 0; k < 16; k++) {
    flags[k] += buffer[k];
  }

  // main loop starts here
  for (size_t i = 0; i + 16 <= len;) {
    count16 = _mm256_setzero_si256();
    size_t j = 0;
    size_t maxj = 65535;
    if (maxj + i + 16 >= len)
      maxj = len - i - 15;
    if (maxj > 8) {
      for (; j < maxj - 7; j += 8) {
        __m256i input1 = _mm256_loadu_si256((__m256i *)(array + i + j));
        __m256i m1 = _mm256_and_si256(input1, bits);
        __m256i eq1 = _mm256_cmpeq_epi16(bits, m1);
        count16 = _mm256_sub_epi16(count16, eq1);
        __m256i input2 = _mm256_loadu_si256((__m256i *)(array + i + j + 1));
        __m256i m2 = _mm256_and_si256(input2, bits);
        __m256i eq2 = _mm256_cmpeq_epi16(bits, m2);
        count16 = _mm256_sub_epi16(count16, eq2);
        __m256i input3 = _mm256_loadu_si256((__m256i *)(array + i + j + 2));
        __m256i m3 = _mm256_and_si256(input3, bits);
        __m256i eq3 = _mm256_cmpeq_epi16(bits, m3);
        count16 = _mm256_sub_epi16(count16, eq3);
        __m256i input4 = _mm256_loadu_si256((__m256i *)(array + i + j + 3));
        __m256i m4 = _mm256_and_si256(input4, bits);
        __m256i eq4 = _mm256_cmpeq_epi16(bits, m4);
        count16 = _mm256_sub_epi16(count16, eq4);
        __m256i input5 = _mm256_loadu_si256((__m256i *)(array + i + j + 4));
        __m256i m5 = _mm256_and_si256(input5, bits);
        __m256i eq5 = _mm256_cmpeq_epi16(bits, m5);
        count16 = _mm256_sub_epi16(count16, eq5);
        __m256i input6 = _mm256_loadu_si256((__m256i *)(array + i + j + 5));
        __m256i m6 = _mm256_and_si256(input6, bits);
        __m256i eq6 = _mm256_cmpeq_epi16(bits, m6);
        count16 = _mm256_sub_epi16(count16, eq6);
        __m256i input7 = _mm256_loadu_si256((__m256i *)(array + i + j + 6));
        __m256i m7 = _mm256_and_si256(input7, bits);
        __m256i eq7 = _mm256_cmpeq_epi16(bits, m7);
        count16 = _mm256_sub_epi16(count16, eq7);
        __m256i input8 = _mm256_loadu_si256((__m256i *)(array + i + j + 7));
        __m256i m8 = _mm256_and_si256(input8, bits);
        __m256i eq8 = _mm256_cmpeq_epi16(bits, m8);
        count16 = _mm256_sub_epi16(count16, eq8);
      }
    }

    if (maxj > 4) {
      for (; j + 3 < maxj; j += 4) {
        __m256i input1 = _mm256_loadu_si256((__m256i *)(array + i + j));
        __m256i m1 = _mm256_and_si256(input1, bits);
        __m256i eq1 = _mm256_cmpeq_epi16(bits, m1);
        count16 = _mm256_sub_epi16(count16, eq1);
        __m256i input2 = _mm256_loadu_si256((__m256i *)(array + i + j + 1));
        __m256i m2 = _mm256_and_si256(input2, bits);
        __m256i eq2 = _mm256_cmpeq_epi16(bits, m2);
        count16 = _mm256_sub_epi16(count16, eq2);
        __m256i input3 = _mm256_loadu_si256((__m256i *)(array + i + j + 2));
        __m256i m3 = _mm256_and_si256(input3, bits);
        __m256i eq3 = _mm256_cmpeq_epi16(bits, m3);
        count16 = _mm256_sub_epi16(count16, eq3);
        __m256i input4 = _mm256_loadu_si256((__m256i *)(array + i + j + 3));
        __m256i m4 = _mm256_and_si256(input4, bits);
        __m256i eq4 = _mm256_cmpeq_epi16(bits, m4);
        count16 = _mm256_sub_epi16(count16, eq4);
      }
    }
    for (; j < maxj; j++) {
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

// fixme
void fastavx2mula(uint16_t *array, size_t len, uint32_t *flags) {
  for (size_t i = 0; i < 16; i++)
    flags[i] = 0;
  for (size_t i = 0; i + 32 <= len; i += 32) {
    __m256i v0 = _mm256_loadu_si256((__m256i *)(array + i));
    __m256i v1 = _mm256_loadu_si256((__m256i *)(array + i + 16));

    __m256i input0 =
        _mm256_or_si256(_mm256_and_si256(v0, _mm256_set1_epi16(0x00ff)),
                        _mm256_slli_epi16(v1, 8));
    __m256i input1 =
        _mm256_or_si256(_mm256_and_si256(v0, _mm256_set1_epi16(0xff00)),
                        _mm256_srli_epi16(v1, 8));
    for (int i = 0; i < 8; i++) {
      flags[7 - i] += __builtin_popcount(_mm256_movemask_epi8(input0));
      flags[15 - i] += __builtin_popcount(_mm256_movemask_epi8(input1));
      input0 = _mm256_add_epi8(input0, input0);
      input1 = _mm256_add_epi8(input1, input1);
    }
  }
}

void demo(size_t len) {
  printf("\n Using array size = %zu \n", len);
  uint32_t counter[16];
  uint32_t truecounter[16];

  uint16_t *array = (uint16_t *)malloc(len * sizeof(uint16_t));
  memset(array, 0, len * sizeof(uint16_t));
  for (size_t i = 0; i < len; i++) {
    array[i] = i & 0xFFFF;
  }
  scalar_naive(array, len, truecounter);
  for (size_t i = 0; i < 16; i++) {
    printf("%u ", truecounter[i]);
  }
  printf("\n");
  int repeat = 1;

  fastavx2(array, len, counter);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("fastavx2 is buggy\n");
      break;
    }
  }
  repeat = 10;
  BEST_TIME_NOCHECK(scalar_naive(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("scalar_naive is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(scalar_morenaive(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("fastavx2 is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(fastavx2(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("fastavx2 is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(morefastavx2(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("morefastavx2 is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(flag_stats_avx2(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("flag_stats_avx2 is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(flag_stats_avx2_naive_counter(array, len, counter), ,
                    repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("flag_stats_avx2_naive_counter is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(flag_stats_avx2_single(array, len, counter), , repeat, len,
                    true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("flag_stats_avx2_single is buggy\n");
      break;
    }
  }
  BEST_TIME_NOCHECK(purecircuit(array, len, counter), , repeat, len, true);

  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("purecircuit is buggy\n");
      break;
    }
  }

  BEST_TIME_NOCHECK(fastavx2mula(array, len, counter), , repeat, len, true);
  for (size_t i = 0; i < 16; i++) {
    if (counter[i] != truecounter[i]) {
      printf("fastavx2mula is buggy\n");
      break;
    }
  }
  free(array);
}

int main() {
  demo(64 * 64 * 64 * 64);
  return EXIT_SUCCESS;
}
