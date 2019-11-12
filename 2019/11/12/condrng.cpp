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

__attribute__((noinline)) void cond_sum_random_extra(uint64_t seed, uint64_t howmany,
                                               uint64_t *out) {
  uint64_t randomval;
  while (howmany >= 2) {
    randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 1 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany-=2;
  }  
  while (howmany != 0) {
    uint64_t randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany--;
  }
}


__attribute__((noinline)) void cond_sum_random_extra8(uint64_t seed, uint64_t howmany,
                                               uint64_t *out) {
  uint64_t randomval;
  while (howmany >= 8) {
    randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 1 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 2 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 3 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 4 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 5 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 6 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 7 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany-=8;
  }  
  while (howmany != 0) {
    uint64_t randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany--;
  }
}

__attribute__((noinline)) void cond_sum_random_extra4(uint64_t seed, uint64_t howmany,
                                               uint64_t *out) {
  uint64_t randomval;
  while (howmany >= 4) {
    randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 1 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 2 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    randomval = rng(howmany - 3 + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany-=4;
  }  
  while (howmany != 0) {
    uint64_t randomval = rng(howmany + seed);
    if ((randomval & 1) == 1)
      *out++ = randomval;
    howmany--;
  }
}


__attribute__((noinline)) void cond_sum_random(uint64_t seed, uint64_t howmany,
                                               uint64_t *out) {
  while (howmany != 0) {
    uint64_t randomval = rng(howmany + seed);
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

void forget() {
  uint64_t x = 0;
  for(size_t i = 0; i < 100000000; i++) {
    x += rng(i);
    if((rng(i) & 0xF) == 1) x = 5;
  }
  if(x == 3332132132) printf("suprising\n");

}

void demo(uint64_t howmany, uint64_t * seed) {
  uint64_t *buffer = new uint64_t[howmany];
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);

  std::vector<unsigned long long> results;
  results.resize(evts.size());
  std::vector<unsigned long long> results_cond;
  results_cond.resize(evts.size());
  std::vector<unsigned long long> results_cond_extra;
  results_cond_extra.resize(evts.size());
  std::vector<unsigned long long> results_cond_extra4;
  results_cond_extra4.resize(evts.size());
  //

#define NUMBER_TRIAL 16
  double extra4misses[NUMBER_TRIAL];
  double extramisses[NUMBER_TRIAL];
  double condmisses[NUMBER_TRIAL];
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
    unified.start();
    cond_sum_random_extra4(*seed, howmany, buffer);
    unified.end(results_cond_extra4);
    extra4misses[trial] = results_cond_extra4[2] * 1.0 / howmany;

  } 
  *seed += howmany;
   for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
    unified.start();
    cond_sum_random_extra(*seed, howmany, buffer);
    unified.end(results_cond_extra);
    extramisses[trial] = results_cond_extra[2] * 1.0 / howmany;

  } 
  *seed += howmany;
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
    unified.start();
    cond_sum_random(*seed, howmany, buffer);
    unified.end(results_cond);
    condmisses[trial] = results_cond[2] * 1.0 / howmany;
  }
  *seed += howmany;
  for (size_t trial = 0; trial < NUMBER_TRIAL; trial++) {
   printf("%zu %.3f %.3f %.3f \n", trial, extra4misses[trial], extramisses[trial], condmisses[trial]);
  }
  delete[] buffer;
  printf("\n");
}

int main() {
  uint64_t large = 1000; 
  uint64_t seed = 0;
  demo(large, &seed);
  demo(large, &seed);
  demo(large, &seed);
}
