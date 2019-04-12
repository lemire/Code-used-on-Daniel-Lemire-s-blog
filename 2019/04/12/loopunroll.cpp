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

__attribute__((optimize("no-tree-vectorize", "no-unroll-loops"))) uint64_t
scalar(uint32_t *x, uint32_t *y, size_t length) {
  uint64_t sum = 0;
  for (size_t i = 0; i < length; i++)
    sum += (uint64_t)x[i] * y[i];
  return sum;
}

__attribute__((optimize("no-tree-vectorize", "no-unroll-loops"))) uint64_t
scalar2(uint32_t *x, uint32_t *y, size_t length) {
  uint64_t sum = 0;
  size_t i = 0;
  if (length > 1)
    for (; i < length - 1; i += 2)
      sum += (uint64_t)x[i] * y[i] + (uint64_t)x[i + 1] * y[i + 1];
  for (; i < length; i++)
    sum += (uint64_t)x[i] * y[i];
  return sum;
}
__attribute__((optimize("no-tree-vectorize", "no-unroll-loops"))) uint64_t
scalar4(uint32_t *x, uint32_t *y, size_t length) {
  uint64_t sum = 0;
  size_t i = 0;
  if (length > 3)
    for (; i < length - 3; i += 4)
      sum += (uint64_t)x[i] * y[i] + (uint64_t)x[i + 1] * y[i + 1] +
             (uint64_t)x[i + 2] * y[i + 2] + (uint64_t)x[i + 3] * y[i + 3];
  for (; i < length; i++)
    sum += (uint64_t)x[i] * y[i];
  return sum;
}
__attribute__((optimize("no-tree-vectorize", "no-unroll-loops"))) uint64_t
scalar8(uint32_t *x, uint32_t *y, size_t length) {
  uint64_t sum = 0;
  size_t i = 0;
  if (length > 7)
    for (; i < length - 7; i += 8)
      sum += (uint64_t)x[i] * y[i] + (uint64_t)x[i + 1] * y[i + 1] +
             (uint64_t)x[i + 2] * y[i + 2] + (uint64_t)x[i + 3] * y[i + 3] +
             (uint64_t)x[i + 4] * y[i + 4] + (uint64_t)x[i + 5] * y[i + 5] +
             (uint64_t)x[i + 6] * y[i + 6] + (uint64_t)x[i + 7] * y[i + 7];
  for (; i < length; i++)
    sum += (uint64_t)x[i] * y[i];
  return sum;
}
__attribute__((optimize("no-tree-vectorize", "no-unroll-loops"))) uint64_t
scalar16(uint32_t *x, uint32_t *y, size_t length) {
  uint64_t sum = 0;
  size_t i = 0;
  if (length > 15)
    for (; i < length - 15; i += 16)
      sum += (uint64_t)x[i] * y[i] + (uint64_t)x[i + 1] * y[i + 1] +
             (uint64_t)x[i + 2] * y[i + 2] + (uint64_t)x[i + 3] * y[i + 3] +
             (uint64_t)x[i + 4] * y[i + 4] + (uint64_t)x[i + 5] * y[i + 5] +
             (uint64_t)x[i + 6] * y[i + 6] + (uint64_t)x[i + 7] * y[i + 7] +
             (uint64_t)x[i + 8] * y[i + 8] + (uint64_t)x[i + 9] * y[i + 9] +
             (uint64_t)x[i + 10] * y[i + 10] + (uint64_t)x[i + 11] * y[i + 11] +
             (uint64_t)x[i + 12] * y[i + 12] + (uint64_t)x[i + 13] * y[i + 13] +
             (uint64_t)x[i + 14] * y[i + 14] + (uint64_t)x[i + 15] * y[i + 15];
  for (; i < length; i++)
    sum += (uint64_t)x[i] * y[i];
  return sum;
}
typedef uint64_t (*scalartype)(uint32_t *x, uint32_t *y, size_t l);

void test(size_t N, scalartype fn) {
  uint32_t *x = (uint32_t *)malloc(sizeof(uint32_t) * N);
  uint32_t *y = (uint32_t *)malloc(sizeof(uint32_t) * N);
  for (size_t i = 0; i < N; i++) {
    x[i] = i;
    y[i] = 3 * i;
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
    sum += fn(x, y, N);
    unified.end(results);

    allresults.push_back(results);
  }
  if (sum == 0)
    printf("bogus\n.");
  std::vector<unsigned long long> mins = compute_mins(allresults);
  std::vector<double> avg = compute_averages(allresults);

  printf(
      "instructions per cycle %4.2f, cycles per pair of 32-bit words:  %4.3f, "
      "instructions per pair of 32-bit words: %4.3f \n",
      double(mins[1]) / mins[0], double(mins[0]) / N, double(mins[1]) / N);
  // first we display mins
  printf("min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu "
         "cache ref., %8llu cache mis.\n",
         mins[0], mins[1], mins[2], mins[3], mins[4]);
  printf("avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f "
         "cache ref., %8.1f cache mis.\n",
         avg[0], avg[1], avg[2], avg[3], avg[4]);
  free(x);
  free(y);
  printf("sum = %zu \n", (size_t)sum);
}

int main() {
  size_t N = 1000 * 1000;
  printf("N= %zu \n", N);
  printf("scalar\n");
  test(N, &scalar);
  printf("scalar2\n");
  test(N, &scalar2);
  printf("scalar4\n");
  test(N, &scalar4);
  printf("scalar8\n");
  test(N, &scalar8);
  printf("scalar16\n");
  test(N, &scalar16);
}
