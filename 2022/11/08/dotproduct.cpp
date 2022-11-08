#include <iostream>
#include <x86intrin.h>

float dot512fma_unsafe(float *x1, float *x2, size_t length) {
  // create a vector of 16 32-bit floats (zeroed)
  __m512 sum = _mm512_setzero_ps();
  for (size_t i = 0; i < length; i += 16) {
    // load 16 32-bit floats
    __m512 v1 = _mm512_loadu_ps(x1 + i);
    // load 16 32-bit floats

    __m512 v2 = _mm512_loadu_ps(x2 + i);
    // do sum[0] += v1[i]*v2[i] (fused multiply-add)
    sum = _mm512_fmadd_ps(v1, v2, sum);
  }
  // reduce: sums all elements
  return _mm512_reduce_add_ps(sum);
}

float dot512fma(float *x1, float *x2, size_t length) {
  // create a vector of 16 32-bit floats (zeroed)
  __m512 sum = _mm512_setzero_ps();
  size_t i = 0;
  for (; i + 16 <= length; i+=16) {
    // load 16 32-bit floats
    __m512 v1 = _mm512_loadu_ps(x1 + i);
    // load 16 32-bit floats
    __m512 v2 = _mm512_loadu_ps(x2 + i);
    // do sum[0] += v1[i]*v2[i] (fused multiply-add)
    sum = _mm512_fmadd_ps(v1, v2, sum);
  }
  if  (i  < length) {
    // load 16 32-bit floats, load only the first length-i floats
    // other floats are automatically set to zero
    __m512 v1 = _mm512_maskz_loadu_ps((1<<(length-i))-1, x1 + i);
    // load 16 32-bit floats, load only the first length-i floats
    __m512 v2 = _mm512_maskz_loadu_ps((1<<(length-i))-1, x2 + i);
    // do sum[0] += v1[i]*v2[i] (fused multiply-add)
    sum = _mm512_fmadd_ps(v1, v2, sum);
  }
  // reduce: sums all elements
  return _mm512_reduce_add_ps(sum);
}

int main() {
  float x1[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  float x2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::cout << dot512fma(x1, x2, 15) << std::endl;
  std::cout << dot512fma_unsafe(x1, x2, 15) << std::endl;
}