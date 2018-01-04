// gcc -O3 -o align align.c -mavx2  && ./align
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <immintrin.h> // avx2
#include "benchmark.h"

#include <stdint.h>
#include <stddef.h>

__attribute__((noinline)) void vecdaxpy(double *a, double *b, double s,
                                        size_t len) {
  size_t i = 0;
  assert(len % (sizeof(__m256d) / sizeof(double)) == 0);
  const __m256d vs = _mm256_set1_pd(s);
  for (; i + sizeof(__m256d) / sizeof(double) <= len;
       i += sizeof(__m256d) / sizeof(double)) {
    const __m256d va = _mm256_loadu_pd(a + i);
    const __m256d vb = _mm256_loadu_pd(b + i);
    const __m256d mults = _mm256_mul_pd(vb, vs);
    const __m256d vavb = _mm256_add_pd(va, mults);
    _mm256_storeu_pd(a + i, vavb);
  }
  assert(i == len);
}


void init(double *a, double *b, size_t N) {
  for (size_t i = 0; i < N; i++) {
    double A = i;
    double B = i + 2;
    memcpy(a + i, &A, sizeof(A));
    memcpy(b + i, &B, sizeof(B));
  }
}

void check(double *a, double s, int repeat, size_t N) {
  for (size_t i = 0; i < N; i++) {
    double A = i;
    double B = i + 2;
    for (int j = 0; j < repeat; j++) {
      A = A + B * s;
    }
    double RA;
    memcpy(&RA, a + i, sizeof(RA));
    assert(RA == A);
  }
}

// https://stackoverflow.com/questions/11277984/how-to-flush-the-cpu-cache-in-linux-from-a-c-program#11278030
void mem_flush(const void *p, unsigned int allocation_size) {
  const size_t cache_line = 64;
  const char *cp = (const char *)p;
  size_t i = 0;

  if (p == NULL || allocation_size <= 0)
    return;

  for (i = 0; i < allocation_size; i += cache_line) {
    asm volatile("clflush (%0)\n\t" : : "r"(&cp[i]) : "memory");
  }

  asm volatile("sfence\n\t" : : : "memory");
}

void demo(size_t N, bool flush) {
  printf("N = %zu \n", N);
  size_t farray1size = (2 * N + 1) * sizeof(double);
  int8_t *farray1 = (int8_t *)malloc(farray1size);
  memset(farray1, 0, farray1size); // fully clean
  const int repeat = 50;
  const double s = 1.2;
  if (flush) {
    double *a = (double *)(farray1);
    double *b = (double *)(farray1 + N * sizeof(double));
    init(a, b, N);
    BEST_TIME_NOCHECK(vecdaxpy(a, b, s, N), mem_flush(farray1, farray1size),
                      repeat, N, true);
    check(a, s, repeat, N);
  } else {
    double *a = (double *)(farray1);
    double *b = (double *)(farray1 + N * sizeof(double));
    init(a, b, N);
    BEST_TIME_NOCHECK(vecdaxpy(a, b, s, N), , repeat, N, true);
    check(a, s, repeat, N);
  }
  free(farray1);
}

unsigned long long alias(const void *inbyte) {
  return ((uintptr_t)(inbyte) & 4095) ;
}

void demooffset(size_t N, size_t maxoffset) {
  printf("N = %zu \n", N);
  size_t farray1size = (2 * N + 1 + maxoffset) * sizeof(double);
  int8_t *farray1 = (int8_t *)malloc(farray1size);
  memset(farray1, 0, farray1size); // fully clean
  const int repeat = 50;
  const double s = 1.2;
  for (size_t offset = 0; offset <= maxoffset; offset += 1) {
    printf("offset %zu -- \n", offset);
    memset(farray1, 0, farray1size); // fully clean
    double *a = (double *)(farray1 + (offset * sizeof(double)));
    double *b = (double *)(farray1 + (N + offset) * sizeof(double));
    init(a, b, N);
    BEST_TIME_NOCHECK(vecdaxpy(a, b, s, N), , repeat, N, true);
    check(a, s, repeat, N);
    BEST_TIME_NOCHECK(vecdaxpy(b, a, s, N), , repeat, N, true);
  }
  free(farray1);
}

void demooffset_explain(size_t N, size_t maxoffset) {
  printf("N = %zu \n", N);
  size_t farray1size = (2 * N + 1) * sizeof(double);
  int8_t *farray1 = (int8_t *)malloc(farray1size);
  for (size_t offset = 0; offset <= maxoffset; offset += 1) {
    printf("offset %zu -- \n", offset);
    double *a = (double *)(farray1 + (offset * sizeof(double)));
    double *b = (double *)(farray1 + (N + offset) * sizeof(double));
    for(size_t i = 0; i < 10; i++) {
      unsigned long long aa = alias(a+i*4);
      unsigned long long ab = alias(b+i*4);
      unsigned long long cacheas = aa / 64;
      unsigned long long cacheab = (aa + 31)/ 64;
      unsigned long long cachebs = ab / 64;
      unsigned long long cachebb = (ab + 31)/ 64;



      printf("load/store %d %d , load %d %d \n",cacheas, cacheab, cachebs, cachebb);
    }
   }
  free(farray1);
}


int main() {
  for (size_t i = 1000; i <= 1024; i += 4) {
    demo(i, false);
  }
  demooffset(500, 5);
  demooffset(1000, 5);
  demooffset(1008, 5);
  demooffset(1024, 5);
//  demooffset_explain(1000, 5);
//  demooffset_explain(1024, 5);
//  demooffset_explain(1015, 5);
    return 0;
}
