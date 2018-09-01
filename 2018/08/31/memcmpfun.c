#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

#include "benchmark.h"


int avx_memeq(const char * s1, const char *s2, size_t length) {
  size_t i = 0;
  if(length >= 32) {
    for(; i <= length - 32; i += 32) {
      __m256i x1 = _mm256_lddqu_si256((const __m256i *)(s1 + i));
      __m256i x2 = _mm256_lddqu_si256((const __m256i *)(s2 + i));
      if(_mm256_testc_si256(x1,x2) == 0) return 0;
    }
  }
  if(length - i >= 16) {
      __m128i x1 = _mm_lddqu_si128((const __m128i *)(s1 + i));
      __m128i x2 = _mm_lddqu_si128((const __m128i *)(s2 + i));
      if(_mm_testc_si128(x1,x2) == 0) return 0;
      i += 16;
  }
  if(length - i >= 8) {
    uint64_t x1;
    uint64_t x2;
    memcpy(&x1, s1 + i, sizeof(x1));
    memcpy(&x2, s2 + i, sizeof(x2));
    if(x1 != x2) return 0;
    i += 8;
  }
  for(; i < length; i ++) {
    if(s1[i] != s2[i]) return 0;
  }
  return 1;
}

int avx_memcmp(const char * s1, const char *s2, size_t length) {
  size_t i = 0;
  if(length >= 32) {
    for(; i <= length - 32; i += 32) {
      __m256i x1 = _mm256_lddqu_si256((const __m256i *)(s1 + i));
      __m256i x2 = _mm256_lddqu_si256((const __m256i *)(s2 + i));
      if(_mm256_testc_si256(x1,x2) == 0) {
         return memcmp(s1 + i, s2 + i, 32);
      }
    }
  }
  if(length - i >= 16) {
      __m128i x1 = _mm_lddqu_si128((const __m128i *)(s1 + i));
      __m128i x2 = _mm_lddqu_si128((const __m128i *)(s2 + i));
      if(_mm_testc_si128(x1,x2) == 0) {
         return memcmp(s1 + i, s2 + i, 32);
      }
      i += 16;
  }
  if(length - i >= 8) {
    uint64_t x1;
    uint64_t x2;
    memcpy(&x1, s1 + i, sizeof(x1));
    memcpy(&x2, s2 + i, sizeof(x2));
    if(x1 != x2) {
       return memcmp(s1 + i, s2 + i, 8);
    }
    i += 8;
  }
  for(; i < length; i ++) {
    if(s1[i] != s2[i]) return s1[i] - s2[i];
  }
  return 0;
}


int simplememeq(const char * s1, const char *s2, size_t length) {
  size_t i = 0;
  for(; i < length; i ++) {
    if(s1[i] != s2[i]) return 0;
  }
  return 1;
}


typedef struct pcg_state_setseq_64 { // Internals are *Private*.
  uint64_t state;                    // RNG state.  All values are possible.
  uint64_t inc;                      // Controls which RNG sequence (stream) is
                                     // selected. Must *always* be odd.
} pcg32_random_t;


pcg32_random_t pcg;

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * 6364136223846793005ULL + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


uint32_t *create_random_array(size_t count) {
  uint32_t *targets;
  int r = posix_memalign((void **)&targets, 64, count * 64);
  if (r != 0) {
    printf("oh!\n\n");
    return NULL;
  }
  for (size_t i = 0; i < 16 * count; i++)
    targets[i] = pcg32_random_r(&pcg);
  return targets;
}


size_t mass_comparison(const char * bigarray1, const char * bigarray2,  size_t N, size_t strlen) {
  _mm256_zeroupper();
  size_t count = 0;
  for(size_t i = 0; i < N; i++) {
    const char * s1 = bigarray1 + i * strlen;
    const char * s2 = bigarray2 + i * strlen;
    count += (memcmp(s1, s2, strlen) == 0) ? 1 : 0;
  }
  return count;
}



size_t avxmemcmp_mass_comparison(const char * bigarray1, const char * bigarray2,  size_t N, size_t strlen) {
  _mm256_zeroupper();
  size_t count = 0;
  for(size_t i = 0; i < N; i++) {
    const char * s1 = bigarray1 + i * strlen;
    const char * s2 = bigarray2 + i * strlen;
    count += (avx_memcmp(s1, s2, strlen) == 0) ? 1 : 0;
  }
  return count;
}

size_t avx_mass_comparison(const char * bigarray1, const char * bigarray2,  size_t N, size_t strlen) {
  _mm256_zeroupper();
  size_t count = 0;
  for(size_t i = 0; i < N; i++) {
    const char * s1 = bigarray1 + i * strlen;
    const char * s2 = bigarray2 + i * strlen;
    count += avx_memeq(s1, s2, strlen);
  }
  return count;
}


size_t simple_mass_comparison(const char * bigarray1, const char * bigarray2,  size_t N, size_t strlen) {
  _mm256_zeroupper();
  size_t count = 0;
  for(size_t i = 0; i < N; i++) {
    const char * s1 = bigarray1 + i * strlen;
    const char * s2 = bigarray2 + i * strlen;
    count += simplememeq(s1, s2, strlen);
  }
  return count;
}



void demo(size_t N, size_t strlen) {
  printf("strlen %zu \n", strlen);
  char * bigarray1 = (char *)create_random_array(N  * strlen + 1);
  char * bigarray2 = (char *)create_random_array(N * strlen + 1);
  for(size_t i = 0; i < N; i+=(rand() % 4)) {
    memcpy(bigarray2 + i  * strlen, bigarray1 + i * strlen, strlen);
    assert(avx_memeq(bigarray1 + i * strlen, bigarray2 + i * strlen, strlen)  == 1);
    assert(memcmp(bigarray1 + i * strlen, bigarray2 + i * strlen, strlen) == 0);
  }
  for(size_t i = 0; i < N; i++) {
    assert(avx_memeq(bigarray1 + i * strlen, bigarray2 + i * strlen, strlen)  == ((memcmp(bigarray1 + i * strlen, bigarray2 + i * strlen, strlen) == 0) ? 1 : 0));
  }
  size_t expected = mass_comparison(bigarray1, bigarray2, N, strlen);
  printf("Out of %zu strings, exactly %zu are identical.\n", N, expected);

  int repeat = 5;
  bool verbose = true;

  BEST_TIME(mass_comparison(bigarray1, bigarray2, N, strlen), expected,
            , repeat, N, N * strlen, verbose);
  BEST_TIME(simple_mass_comparison(bigarray1, bigarray2, N, strlen), expected,
            , repeat, N, N * strlen, verbose);
  BEST_TIME(avx_mass_comparison(bigarray1, bigarray2, N, strlen), expected,
            , repeat, N, N * strlen, verbose);
  BEST_TIME(avxmemcmp_mass_comparison(bigarray1, bigarray2, N, strlen), expected,
            , repeat, N, N * strlen, verbose);
  free(bigarray1);
  free(bigarray2);



  printf("\n");
}

int main() {
  pcg.inc = 1;
  demo(1000,1024 * 32);
  for(size_t i = 10; i < 256; i++)
    demo(1000, i);
  return EXIT_SUCCESS;
}
