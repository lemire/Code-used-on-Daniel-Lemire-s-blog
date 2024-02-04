
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

uint32_t karprabin_hash(const char *data, size_t len, uint32_t B) {
  uint32_t hash = 0;
  for (size_t i = 0; i < len; i++) {
    hash = hash * B + data[i];
  }
  return hash;
}

__attribute__ ((noinline))
size_t karprabin_naive(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  for(size_t i = 0; i < len-N; i++) {
    uint32_t hash = 0;
    for(size_t j = 0; j < N; j++) {
      hash = hash * B + data[i+j];
    }
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

__attribute__ ((noinline))
size_t karprabin_rolling(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }
  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }
  if (hash == target) {
    counter++;
  }
  for(size_t i = N; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
}

size_t karprabin_rolling4(const char *data, size_t len, size_t N, uint32_t B, uint32_t target) {
  size_t counter = 0;
  uint32_t BtoN = 1;
  for(size_t i = 0; i < N; i++) {
    BtoN *= B;
  }
  uint32_t hash = 0;
  for(size_t i = 0; i < N; i++) {
    hash = hash * B + data[i];
  }
  if (hash == target) {
    counter++;
  }
  size_t i = N;
  for(; i + 3 < len; i+=4) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+1] - BtoN * data[i+1-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+2] - BtoN * data[i+2-N];
    if (hash == target) {
      counter++;
    }
    hash = hash * B + data[i+3] - BtoN * data[i+3-N];
    if (hash == target) {
      counter++;
    }
  }

  for(; i < len; i++) {
    hash = hash * B + data[i] - BtoN * data[i-N];
    // karprabin_hash(data+i-N+1, N, B) == hash
    if (hash == target) {
      counter++;
    }
  }
  return counter;
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
  const size_t N = 1024 * 1024;
  std::unique_ptr<char[]> data(new char[N]);
  for(size_t i = 0; i < N; i++) {
    data.get()[i] = i % 256;
  }
  size_t volume = N;
  volatile size_t counter = 0;
  for(size_t window = 8; window <= 32; window *= 2) {
    printf("window = %zu\n", window);
      pretty_print(1, volume, "karprabin_rolling4", bench([&data, &counter, &window]() {
      counter += karprabin_rolling4(data.get(), N, window, 31, 0);
    }));
    pretty_print(1, volume, "karprabin_rolling", bench([&data, &counter, &window]() {
      counter += karprabin_rolling(data.get(), N, window, 31, 0);
    }));
    pretty_print(1, volume, "karprabin_naive", bench([&data, &counter, &window]() {
      counter += karprabin_naive(data.get(), N, window, 31, 0);
    }));
  }
}
