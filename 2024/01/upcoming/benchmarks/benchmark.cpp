
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>
#include <x86intrin.h>

/***
 * We take a 16 MB array and we multiply it with a matrix
 * containing 256 rows/columns of 16 MB. The processor
 * cannot keep the matrix in cache, but it should be able
 * to keep the array.
 */

float multiply(float *v1, float *v2, size_t N) {
  __m512 acc = _mm512_setzero_ps();
  for (size_t i = 0; i < N; i += sizeof(__m512)) {
    __m512 V1 = _mm512_loadu_ps(v1 + i);
    __m512 V2 = _mm512_loadu_ps(v2 + i);
    acc = _mm512_fmadd_ps(V1, V2, acc);
  }
  return _mm512_reduce_add_ps(acc);
}

float multiply_nt(float *v1, float *v2, size_t N) {
  __m512 acc = _mm512_setzero_ps();
  for (size_t i = 0; i < N; i += sizeof(__m512)) {
    __m512 V1 = _mm512_loadu_ps(v1 + i);
    __m512 V2 = (__m512)_mm512_stream_load_si512(v2 + i);
    acc = _mm512_fmadd_ps(V1, V2, acc);
  }
  return _mm512_reduce_add_ps(acc);
}

std::vector<float> multiply(float *v1, float **v2, size_t M, size_t N) {
  std::vector<float> res(M);
  for (size_t i = 0; i < M; i++) {
    res[i] = multiply(v1, v2[i], N);
  }
  return res;
}

std::vector<float> multiply_nt(float *v1, float **v2, size_t M, size_t N) {
  std::vector<float> res(M);
  for (size_t i = 0; i < M; i++) {
    res[i] = multiply_nt(v1, v2[i], N);
  }
  return res;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}
void init(float *v, size_t N) {
  std::mt19937 gen(0);
  std::uniform_real_distribution<float> dis(0, 1);
  for (size_t i = 0; i < N; i++) {
    v[i] = dis(gen);
  }
}
int main(int argc, char **argv) {
  const size_t N = 32 * 1024 * 1024;
  const size_t M = 256;
  float *v1 = (float *)aligned_alloc(64, N);
  init(v1, N/sizeof(float));
  float **v2 = (float **)aligned_alloc(64, 256 * sizeof(float *));
  for (size_t i = 0; i < 256; i++) {
    v2[i] = (float *)aligned_alloc(64, N);
    init(v2[i], N/sizeof(float));
  }
  size_t volume = N * 256 ;

  std::vector<float> v;
  pretty_print(volume/ sizeof(__m512), volume, "multiply", bench([&v, v1, v2, M, N]() {
                 v = multiply(v1, v2, M, N / sizeof(float));
               }));
  pretty_print(volume/ sizeof(__m512), volume, "multiply_nt", bench([&v, v1, v2, M, N]() {
                 v = multiply_nt(v1, v2, M, N / sizeof(float));
               }));
}
