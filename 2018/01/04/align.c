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
void vecdaxpy(double * a, double * b, double s, size_t len) {
    size_t i = 0;
    assert(len % (sizeof(__m256d) / sizeof(double))  == 0);
    const __m256d vs = _mm256_set1_pd(s);
    for (; i + sizeof(__m256d) / sizeof(double) <= len ; i += sizeof(__m256d) / sizeof(double)) {
      const __m256d va = _mm256_loadu_pd(a + i);
      const __m256d vb = _mm256_loadu_pd(b + i);
      const __m256d mults = _mm256_mul_pd(vb,vs);
      const __m256d vavb = _mm256_add_pd(va,mults);
      _mm256_storeu_pd(a + i,vavb);
    }
    assert(i == len);
}

void init(double * a, double * b, size_t N) {
   for(size_t i = 0; i < N ; i++ ) {
    double A = i; 
    double B = i + 2;
    memcpy(a + i,&A,sizeof(A));
    memcpy(b + i,&B,sizeof(B));
   }
}

void check(double * a,  double s, int repeat, size_t N) {
   for(size_t i = 0; i < N ; i++ ) {
     double A = i;
     double B = i + 2;
     for(int j = 0; j < repeat; j++) {
      A = A + B * s;
     }
     double RA;
     memcpy(&RA, a + i, sizeof(RA));
     assert(RA == A);
   }
}


// https://stackoverflow.com/questions/11277984/how-to-flush-the-cpu-cache-in-linux-from-a-c-program#11278030
void mem_flush(const void *p, unsigned int allocation_size){
    const size_t cache_line = 64;
    const char *cp = (const char *)p;
    size_t i = 0;

    if (p == NULL || allocation_size <= 0)
            return;

    for (i = 0; i < allocation_size; i += cache_line) {
            asm volatile("clflush (%0)\n\t"
                         : 
                         : "r"(&cp[i])
                         : "memory");
    }

    asm volatile("sfence\n\t"
                 :
                 :
                 : "memory");
}

void demo(size_t N, bool flush) {
  printf("N = %zu \n",N);
  size_t farray1size = (2*N+1)*sizeof(double);
  int8_t * farray1 = (int8_t *) malloc(farray1size);
  memset(farray1, 0, farray1size); // fully clean
  const int repeat = 50;
  const double s = 1.2;
  if(flush) {
  //for(size_t offset = 0; offset <= sizeof(double); offset++) {
  // printf("offset %zu memflush -- ",offset);
   double * a = (double *)(farray1);
   double * b = (double *)(farray1 + N * sizeof(double) );
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdaxpy(a,b,s,N), mem_flush(farray1, farray1size), repeat, N, true);
   check(a,s,repeat,N);
  //}
} else {
  //for(size_t offset = 0; offset <= sizeof(double); offset++) {
 //  printf("offset %zu         -- ",offset);
   double * a = (double *)(farray1);
   double * b = (double *)(farray1 + N * sizeof(double) );
   init(a, b, N);
   BEST_TIME_NOCHECK(vecdaxpy(a,b,s,N), , repeat, N, true);
   check(a,s,repeat,N);
  //}
}
   free(farray1);
}

int main() {
  for(size_t i =  1000; i <= 1024; i += 4) {
   demo(i,false);
  } 
  return 0;
}
