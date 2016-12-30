//gcc -ffast-math -O3 -o scalarproduct  scalarproduct.c -march=native
// (clang does better)
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <x86intrin.h>
#include "benchmark.h"

// let us assume AVX2

float scalarproduct(float * array1, float * array2, size_t length) {
  float sum = 0.0f;
  for (size_t i = 0; i < length; ++i) {
    sum += array1[i] * array2[i];
  }
  return sum;
}


float avx_scalarproduct(float * array1, float * array2, size_t length) {
  __m256 vsum = _mm256_setzero_ps();
  size_t i = 0;
  for (; i + 7 < length; i += 8) { // could unroll further
    vsum = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i),_mm256_loadu_ps(array2 + i),vsum);
  }
  float buffer[8];
  _mm256_storeu_ps(buffer,vsum);
  float sum = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];
  for (; i < length; ++i) {
      sum += array1[i] * array2[i];
  }
  return sum;
}

int main() {
  const int N = 2048;
  float * array1 = (float*) malloc(N * sizeof(float));
  float * array2 = (float*) malloc(N * sizeof(float));
  for(size_t i = 0 ; i < N; ++i) {
    array1[i] = i ;
    array2[i] = 1;
  }
  float expected = scalarproduct(array1,array2,N);
  const int repeat = 50;

  BEST_TIME(scalarproduct(array1,array2,N), expected, , repeat, N, true);
  BEST_TIME(avx_scalarproduct(array1,array2,N), expected, , repeat, N, true);

}
