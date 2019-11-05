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


__attribute__((noinline)) void cond_sum_random_extra(uint64_t howmany,
                                               uint64_t *out) {
  while (howmany != 0) {
    uint64_t randomval = rng(howmany);
    if(randomval == 4132412421) abort();
    if ((randomval & 1) == 1)
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
  std::vector<unsigned long long> results_cond_extra;
  results_cond_extra.resize(evts.size());
  std::vector<unsigned long long> results_mask;
  results_mask.resize(evts.size());
  //

#define NUMBER_TRIAL 10
  double extramisses[NUMBER_TRIAL];
  double condmisses[NUMBER_TRIAL];
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
    printf("# ==== trial %zu\n", trial);
    unified.start();
    cond_sum_random_extra(howmany, buffer);
    unified.end(results_cond_extra);
    printf("#extra    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f \n",
           results_cond_extra[0] * 1.0 / howmany, results_cond_extra[1] * 1.0 / howmany,
           results_cond_extra[2] * 1.0 / howmany);
    extramisses[trial] = results_cond_extra[2] * 1.0 / howmany;

  } 
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
    printf("# ==== trial %zu\n", trial);
    unified.start();
    cond_sum_random(howmany, buffer);
    unified.end(results_cond);
    printf("#cond    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f \n",
           results_cond[0] * 1.0 / howmany, results_cond[1] * 1.0 / howmany,
           results_cond[2] * 1.0 / howmany);
    condmisses[trial] = results_cond[2] * 1.0 / howmany;

  }
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
   printf("%zu %.3f %.3f \n", trial, extramisses[trial], condmisses[trial]);
  }
  delete[] buffer;
}

int main() {
  uint64_t large = 1000; 
  demo(large);
}
