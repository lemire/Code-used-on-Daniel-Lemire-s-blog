
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

__attribute__ ((noinline))
uint64_t pair_factorial(uint64_t n) {
  if(n <= 1) { return 1; }
  uint64_t r = n;
  uint64_t s = 1;
  uint64_t rs = r * s;
  uint64_t dt = r - s - 1;
  uint64_t pd = rs;
  while(dt >= 2) {
    rs += dt;
    pd *= rs;
    dt -= 2;
  }
  if(n % 2) {
    pd *= (n+1) / 2;
  }
  return pd;
}


__attribute__ ((noinline))
uint64_t pair_factorial_unrolled(uint64_t n) {
  if(n <= 1) { return 1; }
  uint64_t r = n;
  uint64_t s = 1;
  uint64_t rs = r * s;
  uint64_t dt = r - s - 1;
  uint64_t pd = rs;
  while(dt >= 8) {// unroll
    rs += dt;
    pd *= rs;
    dt -= 2;
    rs += dt;
    pd *= rs;
    dt -= 2;
    rs += dt;
    pd *= rs;
    dt -= 2;
    rs += dt;
    pd *= rs;
    dt -= 2;
  }
  while(dt >= 4) {// unroll
    rs += dt;
    pd *= rs;
    rs += dt - 2;
    pd *= rs;
    dt -= 4;
  }
  while(dt >= 2) {
    rs += dt;
    pd *= rs;
    dt -= 2;
  }
  if(n % 2) {
    pd *= (n+1) / 2;
  }
  return pd;
}

__attribute__ ((noinline))
uint64_t factorial(uint64_t n) {
  uint64_t pd = 1;
  for(uint64_t i = 1; i <= n; i++) {
    pd *= i;
  }
  return pd;
}


void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  //printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
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
  constexpr size_t N = 10000;
  constexpr size_t volume = N*sizeof(uint64_t);
  volatile size_t counter = 0;
  printf("Verifying");
  for(size_t i = 1; i < N; i++) {
    if(factorial(i) != pair_factorial(i)) {
      std::cerr << "Error: " << i << " " << factorial(i) << " " << pair_factorial(i) << std::endl;
      return EXIT_FAILURE;
    }
    if(i%(N/10) == 0) {
      printf(".");
      fflush(stdout);
    }
  }
  printf("Verified!\n");
  pretty_print(N, volume, "factorial", bench([&counter]() {
      for(uint64_t i = 1; i < N; i++) {
        counter += factorial(i);
      }
  }));
  pretty_print(N, volume, "pair_factorial", bench([&counter]() {
      for(uint64_t i = 1; i < N; i++) {
        counter += pair_factorial(i);
      }
  }));
  pretty_print(N, volume, "pair_factorial_unrolled", bench([&counter]() {
      for(uint64_t i = 1; i < N; i++) {
        counter += pair_factorial_unrolled(i);
      }
  }));
}
