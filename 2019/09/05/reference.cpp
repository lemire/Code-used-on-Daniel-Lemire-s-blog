#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>

#define SIZE 1000000
__attribute__((noinline)) void incrementr(uint64_t &i, uint64_t *x) {
  for (int k = 0; k < SIZE; k++) {
    x[k] += i++;
  }
}

__attribute__((noinline)) void incrementp(uint64_t *i, uint64_t *x) {
  for (int k = 0; k < SIZE; k++) {
    x[k] += (*i)++;
  }
}

__attribute__((noinline)) void increment(uint64_t &i, uint64_t *x) {
  uint64_t copyi = i;
  for (int k = 0; k < SIZE; k++) {
    x[k] += copyi++;
  }
  i = copyi;
}

__attribute__((noinline)) int classic_increment(uint64_t i, uint64_t *x) {
  for (int k = 0; k < SIZE; k++) {
    x[k] += i++;
  }
  return i;
}

__attribute__((noinline)) void incrementrr(uint64_t &i, uint64_t * __restrict__ x) {
  for (int k = 0; k < SIZE; k++) {
    x[k] += i++;
  }
}


void test() {
  uint64_t *bigarray = new uint64_t[SIZE];
  memset(bigarray, 0, SIZE * sizeof(uint64_t));
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results1;
  results1.resize(evts.size());
  std::vector<unsigned long long> results2;
  results2.resize(evts.size());
  std::vector<unsigned long long> results3;
  results3.resize(evts.size());
  std::vector<unsigned long long> results4;
  results4.resize(evts.size());
  std::vector<unsigned long long> results5;
  results5.resize(evts.size());
  uint64_t counter = 0;
  unified.start();
  increment(counter, bigarray);
  unified.end(results1);
  unified.start();
  incrementr(counter, bigarray);
  unified.end(results2);
  unified.start();
  incrementp(&counter, bigarray);
  unified.end(results3);
  unified.start();
  counter = classic_increment(counter, bigarray);
  unified.end(results4);
  unified.start();
  incrementrr(counter, bigarray);
  unified.end(results5);
  printf("standard  %.2f cycles/value %.2f instructions/value\n",
         results1[0] * 1.0 / SIZE, results1[1] * 1.0 / SIZE);
  printf("reference %.2f cycles/value %.2f instructions/value\n",
         results2[0] * 1.0 / SIZE, results2[1] * 1.0 / SIZE);
  printf("pointer   %.2f cycles/value %.2f instructions/value\n",
         results3[0] * 1.0 / SIZE, results3[1] * 1.0 / SIZE);
  printf("classic   %.2f cycles/value %.2f instructions/value\n",
         results4[0] * 1.0 / SIZE, results4[1] * 1.0 / SIZE);
  printf("restrict   %.2f cycles/value %.2f instructions/value\n",
         results5[0] * 1.0 / SIZE, results5[1] * 1.0 / SIZE);
  delete[] bigarray;
}

int main(int argc, char **argv) { test(); }
