#include "linux-perf-events.h"
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
using namespace std;

static uint64_t murmur64(uint64_t h) {
  h ^= h >> 33;
  h *= UINT64_C(0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= UINT64_C(0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}

int main() {
  const size_t N = 100000000;
  uint64_t *array = new uint64_t[N];
  for (size_t i = 0; i < N; i++)
    array[i] = i;

  vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  vector<unsigned long long> results;
  results.resize(evts.size());
  cout << endl;
  int seed = rand();

  unified.start();
  size_t M = 10000000;
  size_t count = 0;
  for (uint64_t j = 0; j < M; j++)
    count += array[((uint32_t)murmur64(j + seed) * N) >> 32];
  unified.end(results);
  printf("cycles = %10.zu (cycles per key %10.3f) instructions = %10.zu "
         "(ins/key %10.3f,ins/cycles %10.3f) cache misses = %10.zu (misses per "
         "keys %10.3f) branch misses = %10.zu (misses per keys %10.3f) \n",
         (size_t)results[0], results[0] * 1.0 / M, (size_t)results[1],
         results[1] * 1.0 / M, results[1] * 1.0 / results[0],
         (size_t)results[2], results[2] * 1.0 / M, (size_t)results[3],
         results[3] * 1.0 / M);
  printf("%d \n", (uint32_t)count);
  uint32_t *precomp = new uint32_t[M];
  for (uint64_t j = 0; j < M; j++)
    precomp[j] = ((uint32_t)murmur64(j + seed) * N) >> 32;
  unified.start();

  count = 0;
  for (uint64_t j = 0; j < M; j++)
    count += array[precomp[j]];
  unified.end(results);

  printf("cycles = %10.zu (cycles per key %10.3f) instructions = %10.zu "
         "(ins/key %10.3f,ins/cycles %10.3f) cache misses = %10.zu (misses per "
         "keys %10.3f) branch misses = %10.zu (misses per keys %10.3f) \n",
         (size_t)results[0], results[0] * 1.0 / M, (size_t)results[1],
         results[1] * 1.0 / M, results[1] * 1.0 / results[0],
         (size_t)results[2], results[2] * 1.0 / M, (size_t)results[3],
         results[3] * 1.0 / M);

  printf("%d \n", (uint32_t)count);
  unified.start();

  count = 0;
  for (uint64_t j = 0; j < M; j++)
    count +=
        array[((uint32_t)murmur64(murmur64(murmur64(j + seed))) * N) >> 32];
  unified.end(results);

  printf("cycles = %10.zu (cycles per key %10.3f) instructions = %10.zu "
         "(ins/key %10.3f,ins/cycles %10.3f) cache misses = %10.zu (misses per "
         "keys %10.3f) branch misses = %10.zu (misses per keys %10.3f) \n",
         (size_t)results[0], results[0] * 1.0 / M, (size_t)results[1],
         results[1] * 1.0 / M, results[1] * 1.0 / results[0],
         (size_t)results[2], results[2] * 1.0 / M, (size_t)results[3],
         results[3] * 1.0 / M);

  printf("%d \n", (uint32_t)count);
  delete[] precomp;
  delete[] array;
}
