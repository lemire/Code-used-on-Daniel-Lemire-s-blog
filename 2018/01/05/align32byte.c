// gcc -O3 -o align32byte align32byte.c -mavx2  && ./align32byte
#include "benchmark.h"
#include <assert.h>
#include <immintrin.h> // avx2
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>

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

__attribute__((noinline)) void rvecdaxpy(double *a, double *b, double s,
                                         size_t len) {
  int i = len - sizeof(__m256d) / sizeof(double);
  assert(len % (sizeof(__m256d) / sizeof(double)) == 0);
  const __m256d vs = _mm256_set1_pd(s);
  for (; i >= 0; i -= sizeof(__m256d) / sizeof(double)) {
    const __m256d va = _mm256_loadu_pd(a + i);
    const __m256d vb = _mm256_loadu_pd(b + i);
    const __m256d mults = _mm256_mul_pd(vb, vs);
    const __m256d vavb = _mm256_add_pd(va, mults);
    _mm256_storeu_pd(a + i, vavb);
  }
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

// portable version of  posix_memalign
static inline void *aligned_malloc(size_t alignment, size_t size) {
    void *p;
#ifdef _MSC_VER
    p = _aligned_malloc(size, alignment);
#elif defined(__MINGW32__) || defined(__MINGW64__)
    p = __mingw_aligned_malloc(size, alignment);
#else
    // somehow, if this is used before including "x86intrin.h", it creates an
    // implicit defined warning.
    if (posix_memalign(&p, alignment, size) != 0) return NULL;
#endif
    return p;
}
unsigned long long alias(const void *inbyte) {
  return ((uintptr_t)(inbyte)&4095);
}

void demo() {
  size_t farray1size = 4 * 4096; // 4 x 4kB (more than we need)
  int8_t *farray1 = (int8_t *)aligned_malloc(32, farray1size);
  if(farray1 == NULL) {
    printf("aborting\n");
    return;
  }
  const size_t N = 256;
  memset(farray1, 0, farray1size); // fully clean
  const int repeat = 500;
  const double s = 1.2;
  for(int offset = 0; offset < 32; offset += 1) {
    printf("offset: %d bytes\n",offset);
    double *a = (double *)(farray1);
    double *b = (double *)(farray1 + 4096 - offset);
    init(a, b, N);
    BEST_TIME_NOCHECK(vecdaxpy(a, b, s, N), , repeat, N, true);
    check(a, s, repeat, N);
  }
  for(int offset = 32; offset <= 32 * 32; offset += 32) {
    printf("offset: %d bytes (%d 32-byte)\n",offset, offset/32);
    double *a = (double *)(farray1);
    double *b = (double *)(farray1 + 4096 - offset);
    init(a, b, N);
    BEST_TIME_NOCHECK(vecdaxpy(a, b, s, N), , repeat, N, true);
    check(a, s, repeat, N);
  }
  free(farray1);
}

int main() {
  demo();
  return 0;
}
