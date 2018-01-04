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

 __attribute__ ((noinline))
void vecdsum(double * a, double * b, double s, size_t len) {
    size_t i = 0;
    assert(len % 4 == 0);
    const __m256d vs = _mm256_set1_pd(s);
    for (; i + sizeof(__m256d) / sizeof(double) <= len ; i += sizeof(__m256d) / sizeof(double)) {
      const __m256d va = _mm256_loadu_pd(a + i);
      const __m256d vb = _mm256_loadu_pd(b + i);
      const __m256d adds = _mm256_add_pd(vb,vs);
      const __m256d vavb = _mm256_add_pd(va,adds);
      _mm256_storeu_pd(a,vavb);
    }
}

 __attribute__ ((noinline))
void vecdaxpy(double * a, double * b,  size_t len) {
    size_t i = 0;
    assert(len % 4 == 0);
    for (; i + sizeof(__m256d) / sizeof(double) <= len ; i += sizeof(__m256d) / sizeof(double)) {
      const __m256d va = _mm256_loadu_pd(a + i);
      const __m256d vb = _mm256_loadu_pd(b + i);
      const __m256d mults = _mm256_mul_pd(vb,va);
      const __m256d vavb = _mm256_add_pd(va,mults);
      _mm256_storeu_pd(a,vavb);
    }
}

 __attribute__ ((noinline))
void vecdaxpybb(double * a, double * b,  size_t len) {
    size_t i = 0;
    assert(len % 4 == 0);
    for (; i + sizeof(__m256d) / sizeof(double) <= len ; i += sizeof(__m256d) / sizeof(double)) {
      const __m256d va = _mm256_loadu_pd(a + i);
      const __m256d vb = _mm256_loadu_pd(b + i);
      const __m256d mults = _mm256_mul_pd(vb,vb);
      const __m256d vavb = _mm256_add_pd(va,mults);
      _mm256_storeu_pd(a,vavb);
    }
}


 __attribute__ ((noinline))
void vecdaxy(double * a, double * b, double s, size_t len) {
    size_t i = 0;
    const __m256d vs = _mm256_set1_pd(s);
    for (; i + sizeof(__m256d) / sizeof(double) <= len ; i += sizeof(__m256d) / sizeof(double)) {
      const __m256d va = _mm256_loadu_pd(a + i);
      const __m256d vb = _mm256_loadu_pd(b + i);
      const __m256d mults = _mm256_mul_pd(vb,vs);
      const __m256d vavb = _mm256_add_pd(va,mults);
      _mm256_storeu_pd(a,vavb);
    }
}

void init(double * a, double * b, size_t N) {
   for(size_t i = 0; i < N ; i++ ) {
    double A = i; 
    double B = i + 2;
    memcpy(a + i,&A,sizeof(A));
    memcpy(b + i,&B,sizeof(B));
   }
}

void demo(size_t N) {
  printf("N = %zu \n",N);
  printf("each array spans %f Kb \n", N*sizeof(double)/1024.0);
  int8_t * farray1 = (int8_t *) malloc((2*N+1) * sizeof(double));
  memset(farray1, 0, (2*N+1)*sizeof(double)); // fully clean
  const int repeat = 3;
  const double s = 1.2;
  for(size_t offset = 0; offset <= sizeof(double); offset++) {
   printf("offset %zu \n",offset);
   double * a = (double *)(farray1 + offset);
   double * b = (double *)(farray1 + N * sizeof(double) + offset);
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdsum(a,b,s,N), , repeat, N, true);
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdaxy(a,b,s,N), , repeat, N, true);
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdaxpy(a,b,N), , repeat, N, true);
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdaxpybb(a,b,N), , repeat, N, true);
  }
  free(farray1);
}

int main() {
  demo(1000);
  demo(1024);
  return 0;
}
