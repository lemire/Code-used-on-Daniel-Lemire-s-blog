#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#ifndef __GNUC__
#warning "We assume a GCC compiler."
#endif
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>

static inline uint64_t murmur64(uint64_t h) {
  h ^= h >> 33;
  h *= UINT64_C(0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= UINT64_C(0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}

void fillarray(uint64_t *bigarray, size_t size_p2, size_t N) {
  for (uint64_t i = 0; i < N; i++) {
    uint64_t random = murmur64(i);
    bigarray[random & (size_p2 - 1)] = i;
  }
}

typedef struct indexval_s {
  uint64_t index;
  uint64_t random;
} indexval_t;

void buffered_fillarray(uint64_t *bigarray, size_t size_p2, size_t N) {
  int blockshift = 18;
  size_t blocksize =
      1 << blockshift; // map each blocksize from bigarray into a buffer slot
  size_t buffersizeshift = 1;
  size_t buffersize = size_p2 >> buffersizeshift; // total size of the buffer
  indexval_t *buffer = (indexval_t *)malloc(buffersize * sizeof(indexval_t));
  size_t blockcount =
      (size_p2 + blocksize - 1) / blocksize; // number of blocks and slots
  size_t *counts = (size_t *)malloc(sizeof(size_t) * blockcount); // counters
  memset(counts, 0, sizeof(size_t) * blockcount);
  size_t capacity = buffersize / blockcount;
  printf("capacity of buffer slots = %zu slot count = %zu \n", capacity,
         blockcount);

  for (uint64_t i = 0; i < N; i++) {
    uint64_t random = murmur64(i);
    size_t loc = (random & (size_p2 - 1)) >> blockshift;
    if (counts[loc] == capacity) {
      for (size_t j = 0; j < counts[loc]; j++) {
        indexval_t iv = buffer[(loc << (blockshift - buffersizeshift)) + j];
        bigarray[iv.random] = iv.index;
      }
      counts[loc] = 0;
    }
    size_t add = (loc << (blockshift - buffersizeshift)) + counts[loc];
    buffer[add].index = i;
    buffer[add].random = random & (size_p2 - 1);
    counts[loc]++;
  }
  for (size_t i = 0; i < blockcount; i++) {
    size_t base = (i << (blockshift - buffersizeshift));
    for (size_t j = 0; j < counts[i]; j++) {
      indexval_t iv = buffer[base + j];
      bigarray[iv.random] = iv.index;
    }
  }
  free(counts);
  free(buffer);
}
template <void (*F)(uint64_t *, size_t, size_t)> void test(size_t N, size_t M) {
  uint64_t *bigarray = (uint64_t *)malloc(sizeof(uint64_t) * (N));
  memset(bigarray, 0, sizeof(uint64_t) * (N));

  size_t iterations = 3;
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
  for (uint32_t i = 0; i < iterations; i++) {
    unified.start();
    F(bigarray, N, M);
    unified.end(results);
    allresults.push_back(results);
  }
  if (bigarray[0] == 0)
    printf("bogus\n.");
  std::vector<unsigned long long> mins = compute_mins(allresults);
  std::vector<double> avg = compute_averages(allresults);
  printf("%zu %zu %zu %zu \n", bigarray[0], bigarray[1], bigarray[2],
         bigarray[3]);
  printf("instructions per cycle %4.2f, cycles per value set:  "
         "%4.3f, "
         "instructions per value set: %4.3f \n",
         double(mins[1]) / mins[0], double(mins[0]) / M, double(mins[1]) / M);
  // first we display mins
  printf("min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu "
         "cache ref., %8llu cache mis.\n",
         mins[0], mins[1], mins[2], mins[3], mins[4]);
  printf("avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f "
         "cache ref., %8.1f cache mis.\n",
         avg[0], avg[1], avg[2], avg[3], avg[4]);
  free(bigarray);
}

int main() {
  size_t N = 1 << 28;
  size_t M = N;
  printf("N= %zu M = %zu \n", N, M);
  for (size_t i = 0; i < 3; i++) {
    printf("buffered:\n");
    test<buffered_fillarray>(N, M);
    printf("standard:\n");
    test<fillarray>(N, M);
    printf("\n");
  }
}
