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
#include <sstream>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <chrono>

// randomizer
static inline uint64_t rng(uint64_t h) {
  h ^= h >> 33;
  h *= UINT64_C(0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= UINT64_C(0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}
std::string randomints(uint64_t howmany) {
  std::stringstream out;
  uint64_t offset = 1190;
  for(size_t i = 1; i <= howmany; i++) {
    uint64_t x = rng(i + offset);
    out << x << " ";
  }
  return out.str();
}


std::string randomfloats(uint64_t howmany) {
  std::stringstream out;
  uint64_t offset = 1190;
  for(size_t i = 1; i <= howmany; i++) {
    uint64_t x = rng(i + offset);
    double d;
    ::memcpy(&d, &x, sizeof(double));
    // paranoid
    while((! std::isnormal(d))  || std::isnan(d) || std::isinf(d)) {
      offset++;
      x = rng(i + offset);
    ::memcpy(&d, &x, sizeof(double));
     }
    out << std::setprecision(17) << d << " ";
  }
  return out.str();
}

double sum(std::string s, size_t howmany) {
  double answer = 0;
  std::stringstream in(s);
  double x;
  while(in >> x) {
    answer += x;
    howmany --;
  }
  if(howmany != 0) throw std::runtime_error("bug");
  return answer;
}

uint64_t sumints(std::string s, size_t howmany) {
  uint64_t answer = 0;
  std::stringstream in(s);
  uint64_t x;
  while(in >> x) {
    answer += x;
    howmany --;
  }
  if(howmany != 0) throw std::runtime_error("bug");
  return answer;
}



void printvec(std::vector<unsigned long long> evts, size_t volume) {
  printf("%.2f cycles  %.2f instr  %.4f branch miss  %.2f cache ref %.2f cache "
         "miss \n",
         evts[0] * 1.0 / volume, evts[1] * 1.0 / volume, evts[2] * 1.0 / volume,
         evts[3] * 1.0 / volume, evts[4] * 1.0 / volume);
}

void demo(uint64_t howmany) {
  std::cout << "processing " << howmany << " floats "  << std::endl;
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
  std::string s = randomfloats(howmany);
  double volume = s.size() / (1024. * 1024); 
  printf("volume %.2f GB\n", volume);
  printf("bytes per number %.2f\n", s.size() * 1. / howmany);
  for (size_t trial = 0; trial < 3; trial++) {
    printf("\n ==== trial %zu\n", trial);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    unified.start();
    double ts = sum(s, howmany);
    unified.end(results);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    if(ts == 0) printf("bug\n");
    double dif = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();
    printf("basic    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f  MB/s %.2f cycles/byte %.2f \n",
           results[0] * 1.0 / howmany, results[1] * 1.0 / howmany,
           results[2] * 1.0 / howmany, volume * 1000000000/ dif, results[0]/(s.size() * 1.));
  }
}
void demoints(uint64_t howmany) {
  std::cout << "processing " << howmany << " integers "  << std::endl;
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
  std::string s = randomints(howmany);
  double volume = s.size() / (1024. * 1024); 
  printf("volume %.2f GB\n", volume);

  printf("bytes per number %.2f\n", s.size() * 1. / howmany);
  for (size_t trial = 0; trial < 3; trial++) {
    printf("\n ==== trial %zu\n", trial);

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    unified.start();
    uint64_t ts = sumints(s, howmany);
    unified.end(results);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    if(ts == 0) printf("bug\n");
    double dif = std::chrono::duration_cast<std::chrono::nanoseconds>( t2 - t1 ).count();
 
    printf("basic    %.2f cycles/value %.2f instructions/value branch "
           "misses/value %.2f MB/s %.2f cycles/byte %.2f \n",
           results[0] * 1.0 / howmany, results[1] * 1.0 / howmany,
           results[2] * 1.0 / howmany, volume *1000000000/ dif, results[0]/(s.size()*1.));
  }
}


int main() {
  demo(100 * 1000);
  demoints(100 * 1000);
}
