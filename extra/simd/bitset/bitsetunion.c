// gcc -O3 -o bitsetunion bitsetunion.c  -mavx2
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
static inline void or_novect(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
  #pragma clang loop vectorize(disable)
  for(size_t i = 0; i < len; i++) {
    a[i] |= b[i];
  }
}
#pragma GCC optimize(pop)

#pragma GCC optimize("tree-vectorize")
static inline void or(uint64_t * __restrict__ a, const uint64_t * __restrict__ b, size_t len) {
  #pragma clang loop vectorize(enable)
  #pragma GCC ivdep
  for(size_t i = 0; i < len; i++) {
    a[i] |= b[i];
  }
}
#pragma GCC optimize(pop) 

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
}
