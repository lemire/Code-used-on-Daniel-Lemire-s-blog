#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h> // use GCC or clang, please

#include "benchmark.h"
typedef struct pcg_state_setseq_64 { // Internals are *Private*.
  uint64_t state;                    // RNG state.  All values are possible.
  uint64_t inc;                      // Controls which RNG sequence (stream) is
                                     // selected. Must *always* be odd.
} pcg32_random_t;

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void shuffle_pcg32(uint32_t *storage, uint32_t size, pcg32_random_t *rng) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = (pcg32_random_r(rng) * (uint64_t)i) >> 32;
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

int qsort_compare_uint32_t(const void *a, const void *b) {
  return (*(uint32_t *)a - *(uint32_t *)b);
}

uint32_t *create_sorted_array(size_t length, pcg32_random_t *rng) {
  uint32_t *array = (uint32_t *)malloc(length * sizeof(uint32_t));
  for (size_t i = 0; i < length; i++)
    array[i] = (uint32_t)pcg32_random_r(rng);
  qsort(array, length, sizeof(*array), qsort_compare_uint32_t);
  return array;
}

uint32_t *create_random_array(size_t count, pcg32_random_t *rng) {
  uint32_t *targets = (uint32_t *)malloc(count * sizeof(uint32_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = (uint32_t)(pcg32_random_r(rng) & 0x7FFFFFF);
  return targets;
}

#if defined(__AVX512F__) && defined(__AVX512DQ__)
#define AVX512PCG
typedef struct avx512_pcg_state_setseq_64 { // Internals are *Private*.
  __m512i state;      // (8x64bits) RNG state.  All values are possible.
  __m512i inc;        // (8x64bits)Controls which RNG sequences (stream) is
                      // selected. Must *always* be odd. You probably want
                      // distinct sequences
  __m512i multiplier; // set to _mm512_set1_epi64(0x5851f42d4c957f2d);
} avx512_pcg32_random_t;

static inline __m256i avx512_pcg32_random_r(avx512_pcg32_random_t *rng) {
  __m512i oldstate = rng->state;
  rng->state = _mm512_add_epi64(_mm512_mullo_epi64(rng->multiplier, rng->state),
                                rng->inc);
  __m512i xorshifted = _mm512_srli_epi64(
      _mm512_xor_epi64(_mm512_srli_epi64(oldstate, 18), oldstate), 27);
  __m512i rot = _mm512_srli_epi64(oldstate, 59);
  return _mm512_cvtepi64_epi32(_mm512_rorv_epi32(xorshifted, rot));
}

void shuffle_simdpcg32(uint32_t *storage, uint32_t size,
                       avx512_pcg32_random_t *rng) {
  uint32_t i = size;
  assert((size % 16) == 0); // simplification
  uint32_t rndbuffer32[8];
  // general case
  for (; i > 0xFFFF; i -= 8) {
    __m256i nextpos = avx512_pcg32_random_r(rng);
    _mm256_storeu_si256((__m256i *)rndbuffer32, nextpos);
    for (int j = 0; j < 8; j++) {
      uint32_t nextpos = (rndbuffer32[j] * (uint64_t)(i - j)) >> 32;
      uint32_t tmp = storage[i - j - 1];
      uint32_t val = storage[nextpos];
      storage[i - j - 1] = val;
      storage[nextpos] = tmp;
    }
  }
  // switching to 16-bit case
  uint16_t rndbuffer16[16];
  __m256i offsets = _mm256_set_epi16(i - 15, i - 14, i - 13, i - 12, i - 11,
                                     i - 10, i - 9, i - 8, i - 7, i - 6, i - 5,
                                     i - 4, i - 3, i - 2, i - 1, i);
  for (; i >= 16; i -= 16) {
    __m256i nextpos = avx512_pcg32_random_r(rng);
    nextpos = _mm256_mulhi_epu16(nextpos, offsets);
    offsets = _mm256_sub_epi16(offsets, _mm256_set1_epi16(16));
    _mm256_storeu_si256((__m256i *)rndbuffer16, nextpos);
    for (int j = 0; j < 16; j++) {
      uint32_t nextpos = rndbuffer16[j];
      uint32_t tmp = storage[i - j - 1];
      uint32_t val = storage[nextpos];
      storage[i - j - 1] = val;
      storage[nextpos] = tmp;
    }
  }
}

#endif

void demo(size_t size) {
  size = (size + 31) / 32 * 32;
  printf("number = %zu.\n", size);
  pcg32_random_t key = {.state = 324, .inc = 4444};

  uint32_t *testvalues = create_random_array(size, &key);
  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);
  int repeat = 50;
  bool verbose = true;

#ifdef AVX512PCG
  avx512_pcg32_random_t key512 = {};
  key512.state = _mm512_set1_epi64(1111);
  key512.inc = _mm512_set_epi64(15, 13, 11, 9, 7, 5, 3, 1);
  key512.multiplier = _mm512_set1_epi64(0x5851f42d4c957f2d);
  BEST_TIME_NOCHECK(shuffle_simdpcg32(testvalues, size, &key512), , repeat,
                    size, verbose);

#endif
  BEST_TIME_NOCHECK(shuffle_pcg32(testvalues, size, &key), , repeat, size,
                    verbose);

  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

int main() {
  demo(512);
  demo(256);
  return EXIT_SUCCESS;
}
