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
  if ( length >= 8*8) {
      __m256 vsum1 = _mm256_setzero_ps();
      __m256 vsum2 = _mm256_setzero_ps();
      __m256 vsum3 = _mm256_setzero_ps();
      __m256 vsum4 = _mm256_setzero_ps();
      __m256 vsum5 = _mm256_setzero_ps();
      __m256 vsum6 = _mm256_setzero_ps();
      __m256 vsum7 = _mm256_setzero_ps();
      __m256 vsum8 = _mm256_setzero_ps();
      for (; i + 8*8-1 < length; i += 8*8) { // could unroll further
        vsum1 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i),_mm256_loadu_ps(array2 + i),vsum1);
        vsum2 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 8),_mm256_loadu_ps(array2 + i + 8),vsum2);
        vsum3 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 16),_mm256_loadu_ps(array2 + i + 16),vsum3);
        vsum4 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 24),_mm256_loadu_ps(array2 + i + 24),vsum4);
        vsum5 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 32),_mm256_loadu_ps(array2 + i + 32),vsum5);
        vsum6 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 40),_mm256_loadu_ps(array2 + i + 40),vsum6);
        vsum7 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 48),_mm256_loadu_ps(array2 + i + 48),vsum7);
        vsum8 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 56),_mm256_loadu_ps(array2 + i + 56),vsum8);
      }
      vsum1 = _mm256_add_ps(vsum1, vsum2);
      vsum3 = _mm256_add_ps(vsum3, vsum4);
      vsum5 = _mm256_add_ps(vsum5, vsum6);
      vsum7 = _mm256_add_ps(vsum7, vsum8);
      vsum1 = _mm256_add_ps(vsum1, vsum3);
      vsum5 = _mm256_add_ps(vsum5, vsum7);
      vsum = _mm256_add_ps(vsum1, vsum5);
  }
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
