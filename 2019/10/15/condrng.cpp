#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

// randomizer
static inline uint64_t rng(uint64_t h) {
  h ^= h >> 33;
  h *= UINT64_C(0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= UINT64_C(0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}

__attribute__((noinline)) void sum_random(uint64_t howmany, uint64_t *out) {
  while (howmany != 0) {
    uint64_t randomval = rng(howmany);
    *out++ = randomval;
    howmany--;
  }
}

__attribute__((noinline)) void cond_sum_random(uint64_t howmany,
                                               uint64_t *out) {
  while (howmany != 0) {
    uint64_t randomval = rng(howmany);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany--;
  }
}

__attribute__((noinline)) void mask_sum_random(uint64_t howmany,
                                               uint64_t *out) {
  while (howmany != 0) {
    uint64_t randomval = rng(howmany);
    *out = randomval;
    out += (randomval & 1);
    howmany--;
  }
}

void printvec(std::vector<unsigned long long> evts, size_t volume) {
  printf("%.2f cycles  %.2f instr  %.4f branch miss  %.2f cache ref %.2f cache "
         "miss \n",
         evts[0] * 1.0 / volume, evts[1] * 1.0 / volume, evts[2] * 1.0 / volume,
         evts[3] * 1.0 / volume, evts[4] * 1.0 / volume);
}

void demo(uint64_t howmany) {
  uint64_t *buffer = new uint64_t[howmany];
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  //evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  //evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);

  std::vector<unsigned long long> results;
  results.resize(evts.size());
  std::vector<unsigned long long> results_cond;
  results_cond.resize(evts.size());
  std::vector<unsigned long long> results_mask;
  results_mask.resize(evts.size());
  //
  for (size_t trial = 0; trial < 1024; trial++) {
    printf("# ==== trial %zu\n", trial);

    unified.start();
    sum_random(howmany, buffer);
    unified.end(results);

    unified.start();
    cond_sum_random(howmany, buffer);
    unified.end(results_cond);

    unified.start();
    mask_sum_random(howmany, buffer);
    unified.end(results_mask);
    printf("#basic    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f \n",
           results[0] * 1.0 / howmany, results[1] * 1.0 / howmany,
           results[2] * 1.0 / howmany);
    printf("#cond    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f \n",
           results_cond[0] * 1.0 / howmany, results_cond[1] * 1.0 / howmany,
           results_cond[2] * 1.0 / howmany);
    printf("#mask    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f \n",
           results_mask[0] * 1.0 / howmany, results_mask[1] * 1.0 / howmany,
           results_mask[2] * 1.0 / howmany);
    printf("%zu %.3f %.5f \n", trial, results_cond[0] * 1.0 / howmany, results_cond[2] * 1.0 / howmany);
  }
  delete[] buffer;
}

int main() {
  uint64_t large = 2000; 
  demo(large);
}
