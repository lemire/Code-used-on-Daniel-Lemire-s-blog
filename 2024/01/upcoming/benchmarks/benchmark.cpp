
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

volatile size_t g_sum = 0;
__attribute__ ((noinline))
uint64_t sum(const uint8_t *data, size_t start, size_t len, size_t skip = 1) {
    uint64_t sum = 0;
    for (size_t i = start; i < len; i+= skip) {
        sum += data[i];
    }
    g_sum += sum;
    return sum;
}


double estimate_bandwidth(float** data, size_t M, size_t N) {
    size_t segment_length = N;
    size_t cache_line = 64;
    size_t data_volume = M * N;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    uint64_t s = 0;
      for (size_t i = 0; i < M; i++) {
        const uint8_t *thisdata = (const uint8_t *)data[i];
        s += sum(thisdata, 0, segment_length, cache_line);
    }
    (void)s;
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double, std::nano>  elapsed = end - start;
    return data_volume / elapsed.count();
}

/***
 * We take a large array and we multiply it with a matrix
 * containing many rows/columns of 16 MB. The processor
 * cannot keep the matrix in cache, but it should be able
 * to keep the array.
 */
__attribute__ ((noinline))
float multiply(float *v1, float *v2, size_t N) {
  __m512 acc = _mm512_setzero_ps();
  for (size_t i = 0; i < N; i += sizeof(__m512)) {
    __m512 V1 = _mm512_loadu_ps(v1 + i);
    __m512 V2 = _mm512_loadu_ps(v2 + i);
    acc = _mm512_fmadd_ps(V1, V2, acc);
  }
  return _mm512_reduce_add_ps(acc);
}

__attribute__ ((noinline))
float multiply_nt(float *v1, float *v2, size_t N) {
  __m512 acc = _mm512_setzero_ps();
  for (size_t i = 0; i < N; i += sizeof(__m512)) {
    __m512 V1 = _mm512_loadu_ps(v1 + i);
    __m512 V2 = (__m512)_mm512_stream_load_si512(v2 + i);
    acc = _mm512_fmadd_ps(V1, V2, acc);
  }
  return _mm512_reduce_add_ps(acc);
}

__attribute__ ((noinline))
std::vector<float> multiply(float *v1, float **v2, size_t M, size_t N) {
  std::vector<float> res(M);
  for (size_t i = 0; i < M; i++) {
    res[i] = multiply(v1, v2[i], N);
  }
  return res;
}

__attribute__ ((noinline))
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
  printf("please be patient, this will take a few seconds...\n");
  const size_t N = 32 * 1024 * 1024;
  const size_t M = 512;
  float *v1 = (float *)aligned_alloc(64, N);
  init(v1, N/sizeof(float));
  float **v2 = (float **)aligned_alloc(64, M * sizeof(float *));
  for (size_t i = 0; i < M; i++) {
    v2[i] = (float *)aligned_alloc(64, N);
    init(v2[i], N/sizeof(float));
  }
  size_t volume = N * M + N;
  std::cout << "bandwidth " << estimate_bandwidth(v2, M, N) << " GB/s"<< std::endl;

  std::vector<float> v;
  pretty_print(volume / sizeof(__m512), volume, "multiply", bench([&v, v1, v2, M, N]() {
                 v = multiply(v1, v2, M, N / sizeof(float));
               }));
  pretty_print(volume / sizeof(__m512), volume, "multiply_nt", bench([&v, v1, v2, M, N]() {
                 v = multiply_nt(v1, v2, M, N / sizeof(float));
               }));
}
