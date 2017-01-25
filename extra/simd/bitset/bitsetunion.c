// gcc -O3 -o bitsetunion bitsetunion.c  -march=native
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "benchmark.h"
#include <x86intrin.h>

#pragma GCC optimize( "no-tree-vectorize" )
void or_novect(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
  #pragma clang loop vectorize(disable)
  for(size_t i = 0; i < len; i++) {
    a[i] |= b[i];
  }
}
#pragma GCC reset_options


#pragma GCC optimize("tree-vectorize")
void or(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
  #pragma clang loop vectorize(enable)
  #pragma GCC ivdep
  for(size_t i = 0; i < len; i++) {
    a[i] |= b[i];
  }
}
#pragma GCC reset_options


void vor(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
   for(size_t i = 0 ; i + 4 <= len ; i +=4) {
     __m256i va = _mm256_loadu_si256((const __m256i *) (a + i));
     __m256i vb = _mm256_loadu_si256((const __m256i *) (b + i));
     va = _mm256_or_si256(va,vb);
     _mm256_storeu_si256((__m256i *)(a + i),va);
  }
}

void vor4(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
   __m256i va, vb;
   for(size_t i = 0 ; i + 4*4 <= len ; i += 4*4) {
     va = _mm256_loadu_si256((const __m256i *) (a + i));
     vb = _mm256_loadu_si256((const __m256i *) (b + i));
     va = _mm256_or_si256(va,vb);
     _mm256_storeu_si256((__m256i *)(a + i),va);
     va = _mm256_loadu_si256((const __m256i *) (a + i + 4));
     vb = _mm256_loadu_si256((const __m256i *) (b + i + 4));
     va = _mm256_or_si256(va,vb);
     _mm256_storeu_si256((__m256i *)(a + i + 4),va);
     va = _mm256_loadu_si256((const __m256i *) (a + i + 8));
     vb = _mm256_loadu_si256((const __m256i *) (b + i + 8));
     va = _mm256_or_si256(va,vb);
     _mm256_storeu_si256((__m256i *)(a + i + 8),va);
     va = _mm256_loadu_si256((const __m256i *) (a + i + 12));
     vb = _mm256_loadu_si256((const __m256i *) (b + i + 12));
     va = _mm256_or_si256(va,vb);
     _mm256_storeu_si256((__m256i *)(a + i + 12),va);
  }
}



int main() {
  const size_t N = 1024;
  uint64_t * a = malloc(N * sizeof(uint64_t));
  uint64_t * b = malloc(N * sizeof(uint64_t));
  for (size_t k = 0; k < N; ++k) {
    a[k] = k;
    b[k] = k * 100001;
  }
  const int repeat = 50;
  BEST_TIME_NOCHECK(or_novect(a,b,N),, repeat, N, true);
  BEST_TIME_NOCHECK(or(a,b,N),, repeat, N, true);
  BEST_TIME_NOCHECK(vor(a,b,N),, repeat, N, true);
  BEST_TIME_NOCHECK(vor4(a,b,N),, repeat, N, true);
}
