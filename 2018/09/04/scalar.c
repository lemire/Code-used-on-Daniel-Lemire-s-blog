
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <memory.h>
#include <stdint.h>
#include <x86intrin.h>


float scalarproduct(float * array1, float * array2, size_t length) {
  __builtin_ia32_vzeroupper();
  float sum = 0.0f;
  for (size_t i = 0; i < length; ++i) {
    sum += array1[i] * array2[i];
  }
  return sum;
}

float avx_scalarproduct(float * array1, float * array2,
          size_t length) {
  __m256 vsum1 = _mm256_setzero_ps();
  __m256 vsum2 = _mm256_setzero_ps();
  __m256 vsum3 = _mm256_setzero_ps();
  __m256 vsum4 = _mm256_setzero_ps();

  size_t i = 0;
  for (; i + 32-1 < length; i += 32) {
    vsum1 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i),
                       _mm256_loadu_ps(array2 + i),vsum1);
    vsum2 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 8),
                       _mm256_loadu_ps(array2 + i + 8),vsum2);
    vsum3 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 16),
                       _mm256_loadu_ps(array2 + i + 16),vsum3);
    vsum4 = _mm256_fmadd_ps(_mm256_loadu_ps(array1 + i + 24),
                       _mm256_loadu_ps(array2 + i + 24),vsum4);
  }
  __m256 vsum = _mm256_add_ps(_mm256_add_ps(vsum1,vsum2),_mm256_add_ps(vsum3,vsum4));
  float buffer[8];
  _mm256_storeu_ps(buffer,vsum);
  float sum = buffer[0] + buffer[1] +
                      buffer[2] + buffer[3] +
                      buffer[4] + buffer[5] +
                      buffer[6] + buffer[7];
  for (; i < length; ++i) {
      sum += array1[i] * array2[i];
  }
  return sum;
}
#if defined(__AVX512F__)

float avx512_scalarproduct(float * array1, float * array2,
          size_t length) {
  __m512 vsum1 = _mm512_setzero_ps();
  __m512 vsum2 = _mm512_setzero_ps();
  __m512 vsum3 = _mm512_setzero_ps();
  __m512 vsum4 = _mm512_setzero_ps();

  size_t i = 0;
  for (; i + 64-1 < length; i += 64) {
    vsum1 = _mm512_fmadd_ps(_mm512_loadu_ps(array1 + i),
                       _mm512_loadu_ps(array2 + i),vsum1);
    vsum2 = _mm512_fmadd_ps(_mm512_loadu_ps(array1 + i + 16),
                       _mm512_loadu_ps(array2 + i + 16),vsum2);
    vsum3 = _mm512_fmadd_ps(_mm512_loadu_ps(array1 + i + 32),
                       _mm512_loadu_ps(array2 + i + 32),vsum3);
    vsum4 = _mm512_fmadd_ps(_mm512_loadu_ps(array1 + i + 48),
                       _mm512_loadu_ps(array2 + i + 48),vsum4);
  }
  __m512 vsum = _mm512_add_ps(_mm512_add_ps(vsum1,vsum2),_mm512_add_ps(vsum3,vsum4));
  float buffer[16];
  _mm512_storeu_ps(buffer,vsum);
  float sum = 0;
  for(size_t i = 0; i < 16; i++) sum += buffer[i];
  for (; i < length; ++i) {
      sum += array1[i] * array2[i];
  }
  return sum;
}
#endif

struct scalarparam {
  float * array1;
  float * array2;
  size_t length;
};

void* scalarthread(void * data) {
  struct scalarparam  *x = (struct scalarparam  *) data;
  float result = scalarproduct(x->array1, x->array2, x->length);
  printf("%f\n", result);
  return NULL;
}

void* avx_scalarthread(void * data) {
  struct scalarparam  *x = (struct scalarparam  *) data;
  float result = avx_scalarproduct(x->array1, x->array2, x->length);
  printf("%f\n", result);
  return NULL;
}

#if defined(__AVX512F__)

void* avx512_scalarthread(void * data) {
  struct scalarparam  *x = (struct scalarparam  *) data;
  float result = avx512_scalarproduct(x->array1, x->array2, x->length);
  printf("%f\n", result);
  return NULL;
}
#endif

int main(int argc, char **argv) {

  size_t badthreads = 1;
  if (argc > 1) {
    badthreads = atoi(argv[1]);
  }
  size_t nthreads = 4;
  if (argc > 2) {
    nthreads = atoi(argv[2]);
  }
  if(badthreads >nthreads) nthreads = badthreads;
  size_t w = 1000000000;

  printf("Using %zu heavy threads out of %zu threads  \n", badthreads,  nthreads);

  float* array1 = malloc(w * sizeof(float));
  float* array2 = malloc(w * sizeof(float));
  for(size_t i = 0; i < w; i++) {
    array1[i] = i;
    array2[i] = i;
  }
  struct scalarparam params;
  params.array1 = array1;
  params.array2 = array2;
  params.length = w;
  for(size_t t = 0; t < 10 ; t++) {
  pthread_t threads[nthreads];
  int i = 0;
  for(; i < badthreads; i++) {
#if defined(__AVX512F__)
  printf("avx512 is available\n");
  pthread_create(&threads[i], NULL, avx512_scalarthread, &params);
#else
  printf("avx512 is unavailable, going for AVX \n");
  pthread_create(&threads[i], NULL, avx_scalarthread, &params);
#endif
  }

  for (; i < nthreads; i++) {
    pthread_create(&threads[i], NULL, scalarthread, &params);
  }
  for (int i = 0; i < nthreads; i++) {
     pthread_join(threads[i], NULL);
  }
  printf("---------\n");
  }
  return 0;
}

