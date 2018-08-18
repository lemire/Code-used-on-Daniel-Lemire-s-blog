#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#include <x86intrin.h>
__attribute__((noinline)) float dot(float *x1, float *x2, size_t len) {
  float sum = 0;
  for (size_t i = 0; i < len; i++) {
    sum += x1[i] * x2[i];
  }
  return sum;
}

__attribute__((noinline)) float dot128(float *x1, float *x2, size_t len) {
  assert(len % 4 == 0);
  __m128 sum = _mm_setzero_ps();
  if (len > 3) {
    size_t limit = len - 3;
    for (size_t i = 0; i < limit; i += 4) {
      __m128 v1 = _mm_loadu_ps(x1 + i);
      __m128 v2 = _mm_loadu_ps(x2 + i);
      sum = _mm_add_ps(sum, _mm_mul_ps(v1, v2));
    }
  }
  float buffer[4];
  _mm_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3];
}
// wmu insisted
__attribute__((noinline)) float dot128dt(float *x1, float *x2, size_t len) {
  assert(len % 4 == 0);
  float sum = 0;
  if (len > 3) {
    size_t limit = len - 3;
    for (size_t i = 0; i < limit; i += 4) {
      __m128 v1 = _mm_loadu_ps(x1 + i);
      __m128 v2 = _mm_loadu_ps(x2 + i);
      sum += _mm_cvtss_f32(_mm_dp_ps(v1, v2, 0xf1));
    }
  }
  return sum;
}


#ifdef __FMA__
__attribute__((noinline)) float dot128fma(float *x1, float *x2, size_t len) {
  assert(len % 4 == 0);
  __m128 sum = _mm_setzero_ps();
  if (len > 3) {
    size_t limit = len - 3;
    for (size_t i = 0; i < limit; i += 4) {
      __m128 v1 = _mm_loadu_ps(x1 + i);
      __m128 v2 = _mm_loadu_ps(x2 + i);
      sum = _mm_fmadd_ps(v1, v2, sum);
    }
  }
  float buffer[4];
  _mm_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3];
}

#endif

#ifdef __AVX2__
__attribute__((noinline)) float dot256(float *x1, float *x2, size_t len) {
  assert(len % 8 == 0);
  __m256 sum = _mm256_setzero_ps();
  if (len > 7) {
    size_t limit = len - 7;
    for (size_t i = 0; i < limit; i += 8) {
      __m256 v1 = _mm256_loadu_ps(x1 + i);
      __m256 v2 = _mm256_loadu_ps(x2 + i);
      sum = _mm256_add_ps(sum, _mm256_mul_ps(v1, v2));
    }
  }
  float buffer[8];
  _mm256_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7];

}
#endif

#ifdef __FMA__

__attribute__((noinline)) float dot256fma(float *x1, float *x2, size_t len) {
  assert(len % 8 == 0);
  __m256 sum = _mm256_setzero_ps();
  if (len > 7) {
    size_t limit = len - 7;
    for (size_t i = 0; i < limit; i += 8) {
      __m256 v1 = _mm256_loadu_ps(x1 + i);
      __m256 v2 = _mm256_loadu_ps(x2 + i);
      sum = _mm256_fmadd_ps(v1, v2, sum);
    }
  }
  float buffer[8];
  _mm256_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7];
}
#endif

#ifdef __AVX512F__

__attribute__((noinline)) float dot512(float *x1, float *x2, size_t len) {
  assert(len % 16 == 0);
  __m512 sum = _mm512_setzero_ps();
  if (len > 15) {
    size_t limit = len - 15;
    for (size_t i = 0; i < limit; i += 16) {
      __m512 v1 = _mm512_loadu_ps(x1 + i);
      __m512 v2 = _mm512_loadu_ps(x2 + i);
      sum = _mm512_add_ps(sum, _mm512_mul_ps(v1, v2));
    }
  }
  float buffer[16];
  _mm512_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7] + buffer[8] + buffer[9] + buffer[10] +
         buffer[11] + buffer[12] + buffer[13] + buffer[14] + buffer[15];
}

__attribute__((noinline)) float dot512fma(float *x1, float *x2, size_t len) {
  assert(len % 16 == 0);
  __m512 sum = _mm512_setzero_ps();
  if (len > 15) {
    size_t limit = len - 15;
    for (size_t i = 0; i < limit; i += 16) {
      __m512 v1 = _mm512_loadu_ps(x1 + i);
      __m512 v2 = _mm512_loadu_ps(x2 + i);
      sum = _mm512_fmadd_ps(v1, v2, sum);
    }
  }
  float buffer[16];
  _mm512_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7] + buffer[8] + buffer[9] + buffer[10] +
         buffer[11] + buffer[12] + buffer[13] + buffer[14] + buffer[15];
}
__attribute__((noinline)) float dot512fma2(float *x1, float *x2, size_t len) {
  assert(len % 32 == 0);
  __m512 sum = _mm512_setzero_ps();
  if (len > 31) {
    size_t limit = len - 31;
    for (size_t i = 0; i < limit; i += 32) {

      __m512 v11 = _mm512_loadu_ps(x1 + i);
      __m512 v21 = _mm512_loadu_ps(x2 + i);
      __m512 v12 = _mm512_loadu_ps(x1 + i + 16);
      __m512 v22 = _mm512_loadu_ps(x2 + i + 16);
      sum = _mm512_fmadd_ps(v11, v21, sum);
      sum = _mm512_fmadd_ps(v12, v22, sum);
    }
  }
  float buffer[16];
  _mm512_storeu_ps(buffer, sum);
  return buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] +
         buffer[6] + buffer[7] + buffer[8] + buffer[9] + buffer[10] +
         buffer[11] + buffer[12] + buffer[13] + buffer[14] + buffer[15];
}

#endif


void demo(size_t number) {
  number = number / 16 * 16;
  printf("number = %zu.\n", number);
  size_t bytes = number * 2 * sizeof(float);
  if(bytes < 1024 * 1024) {
    printf(" %.2f kB \n", (number * 2 * sizeof(float)) / (1024.0));
  } else {
    printf(" %.2f MB \n", (number * 2 * sizeof(float)) / (1024.0* 1024.0));
  }
  float *x1 = (float *)malloc(sizeof(float) * number);
  float *x2 = (float *)malloc(sizeof(float) * number);

  for (size_t i = 0; i < number; i++) {
    x1[i] = i * 1.f / number;
    x2[i] = (10 - i) * 1.f / number;
  }
  int repeat = 50;
  float expected = dot(x1, x2, number);
  BEST_TIME(dot(x1, x2, number), expected, , repeat, number, bytes, true);
  expected = dot128(x1, x2, number);
  BEST_TIME(dot128(x1, x2, number), expected, , repeat, number, bytes, true);
  expected = dot128dt(x1, x2, number);
  BEST_TIME(dot128dt(x1, x2, number), expected, , repeat, number, bytes, true);
#ifdef __AVX2__
  expected = dot256(x1, x2, number);
  BEST_TIME(dot256(x1, x2, number), expected, , repeat, number, bytes, true);
#endif
#ifdef __FMA__
  expected = dot128fma(x1, x2, number);
  BEST_TIME(dot128fma(x1, x2, number), expected, , repeat, number, bytes, true);
  expected = dot256fma(x1, x2, number);
  BEST_TIME(dot256fma(x1, x2, number), expected, , repeat, number, bytes, true);
#endif
#ifdef __AVX512F__
  expected = dot512(x1, x2, number);
  BEST_TIME(dot512(x1, x2, number), expected, , repeat, number, bytes, true);
  expected = dot512fma(x1, x2, number);
  BEST_TIME(dot512fma(x1, x2, number), expected, , repeat, number, bytes, true);
  expected = dot512fma2(x1, x2, number);
  BEST_TIME(dot512fma2(x1, x2, number), expected, , repeat, number, bytes, true);
#endif
  printf("\n");
  free(x1);
  free(x2);
}

int main() {
  demo(1024);
  demo(2097152);
  demo(4194304);
  demo(8388608);
  demo(16777216);
  demo(33554432);
  demo(134217728);
  return EXIT_SUCCESS;
}
