#include "linux-perf-events.h"
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <libgen.h>
#include <random>
#include <string>
#include <vector>
#include <x86intrin.h>

const signed char digittoval[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};
// returns a value with the high 16 bits set if not valid
// otherwise returns the conversion of the 4 hex digits at src into the bottom 16 bits of the 32-bit
// return register
// 
uint32_t hex_to_u32_lookup(const uint8_t *src) {// strictly speaking, static inline is a C-ism
  uint32_t v1 = digittoval[src[0]]; // uint32_t v1 = -1 becomes uint32_t v1 = 0xFFFFFFFF.
  uint32_t v2 = digittoval[src[1]];
  uint32_t v3 = digittoval[src[2]];
  uint32_t v4 = digittoval[src[3]];
  return static_cast<uint32_t>(v1 << 12 | v2 << 8 | v3 << 4 | v4);
}

uint32_t hex_to_u32_lookup2(const uint8_t *src) {// strictly speaking, static inline is a C-ism
  uint32_t input;
  memcpy(&input, src, 4);
  uint32_t v1 = digittoval[input & 0xFF]; // uint32_t v1 = -1 becomes uint32_t v1 = 0xFFFFFFFF.
  uint32_t v2 = digittoval[(input >> 8) && 0xFF];
  uint32_t v3 = digittoval[(input >> 16) && 0xFF];
  uint32_t v4 = digittoval[(input >> 24) & 0xFF];
  return static_cast<uint32_t>(v1 << 12 | v2 << 8 | v3 << 4 | v4);
}

static inline uint32_t convertone(uint8_t c) {
  uint32_t v = (c & 0xF) + 9 * (c >> 6);
  return v;
}

uint32_t hex_to_u32_math(const uint8_t *src) {
  uint32_t v1 = convertone(src[0]);
  uint32_t v2 = convertone(src[1]);
  uint32_t v3 = convertone(src[2]);
  uint32_t v4 = convertone(src[3]);
  return static_cast<uint32_t>(v1 << 12 | v2 << 8 | v3 << 4 | v4);
}

template <uint32_t (*F)(const uint8_t *src)> 
void test(size_t N) {
  uint8_t *x = (uint8_t *)malloc(sizeof(uint8_t) * ( N + 3));
  for (size_t i = 0; i < N + 3; i++) {
    int digit = (rand() % 16);
    if(digit < 10)
      x[i] = 48 + digit;
    else
      x[i] = 65 + digit - 10;
  }
  for (size_t i = 0; i < N; i++) {
    assert(hex_to_u32_lookup(x + i) == hex_to_u32_math(x +i)  );
  }
  size_t iterations = 50;
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  evts.push_back(PERF_COUNT_HW_REF_CPU_CYCLES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;

  std::vector<std::vector<unsigned long long>> allresults;
  results.resize(evts.size());

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 0xFFFF);

  uint64_t sum = 0;
  for (uint32_t i = 0; i < iterations; i++) {
    unified.start();
    for (size_t i = 0; i < N; i++) {
      sum += F(x + i);
    }
    unified.end(results);
    allresults.push_back(results);
  }
  if (sum == 0)
    printf("bogus\n.");
  std::vector<unsigned long long> mins = compute_mins(allresults);
  std::vector<double> avg = compute_averages(allresults);

  printf(
      "instructions per cycle %4.2f, cycles 4-character hex string:  %4.3f, "
      "instructions per 4-character hex string: %4.3f \n",
      double(mins[1]) / mins[0], double(mins[0]) / N, double(mins[1]) / N);
  // first we display mins
  printf("min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu "
         "cache ref., %8llu cache mis.\n",
         mins[0], mins[1], mins[2], mins[3], mins[4]);
  printf("avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f "
         "cache ref., %8.1f cache mis.\n",
         avg[0], avg[1], avg[2], avg[3], avg[4]);
  free(x);
  printf("sum = %zu \n", (size_t)sum);
}

int main() {
  size_t N = 1000 * 1000;
  printf("N= %zu \n", N);
  printf("lookup:\n");
  test<hex_to_u32_lookup>(N);
  printf("math:\n");
  test<hex_to_u32_math>(N);
}
