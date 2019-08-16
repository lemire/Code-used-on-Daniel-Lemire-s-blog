// we require Linux
#include "linux-perf-events.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

// textbook scancount
__attribute__((noinline)) void
scancount(std::vector<uint8_t> &counters,
          std::vector<std::vector<uint32_t>> &data, std::vector<uint32_t> &out,
          size_t threshold) {
  std::fill(counters.begin(), counters.end(), 0);
  out.clear();
  for (size_t c = 0; c < data.size(); c++) {
    std::vector<uint32_t> &v = data[c];
    for (size_t i = 0; i < v.size(); i++) {
      counters[v[i]]++;
    }
  }
  for (uint32_t i = 0; i < counters.size(); i++) {
    if (counters[i] > threshold)
      out.push_back(i);
  }
}

// cache-sensitive scancount (D. Lemire)
// cache_size might be about your L2 size
__attribute__((noinline)) void
fastscancount(std::vector<uint8_t> &counters,
              std::vector<std::vector<uint32_t>> &data,
              std::vector<uint32_t> &out, size_t threshold, size_t cache_size) {
  out.clear();
  size_t range = cache_size > counters.size() ? counters.size() : cache_size;
  std::vector<std::vector<uint32_t>::const_iterator> iters;
  for (size_t c = 0; c < data.size(); c++) {
    iters.push_back(data[c].begin());
  }
  for (size_t start = 0; start < counters.size(); start += range) {
    std::fill(counters.begin(), counters.begin() + range, 0);
    for (size_t c = 0; c < data.size(); c++) {
      auto &it = iters[c];

      for (; (it != data[c].end()) && (*it < start + range); it++) {
        counters[*it - start]++;
      }
    }
    for (uint32_t i = 0; i < range; i++) {
      if (counters[i] > threshold)
        out.push_back(start + i);
    }
  }
}

void demo(size_t N, size_t length, size_t array_count, size_t threshold) {
  std::vector<std::vector<uint32_t>> data(array_count);
  std::vector<uint8_t> counters(N);
  std::vector<uint32_t> answer;
  answer.reserve(N);
  std::cout << "size of the counter array "
            << double(counters.size()) / (1024 * 1024) << " MB" << std::endl;
  size_t cache_size = 262144; // roughly my L2
  std::cout << "size of the small counter array (cache-sized) "
            << double(cache_size) / 1024 << " kB" << std::endl;
  size_t sum = 0;
  for (size_t c = 0; c < array_count; c++) {
    std::vector<uint32_t> &v = data[c];
    for (size_t i = 0; i < length; i++) {
      v.push_back(rand() % N);
    }
    std::sort(v.begin(), v.end());
    v.resize(std::distance(v.begin(), unique(v.begin(), v.end())));
    sum += v.size();
  }
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  results.resize(evts.size());
  for (size_t t = 0; t < 3; t++) {
    unified.start();
    scancount(counters, data, answer, threshold);
    unified.end(results);
    size_t expected = answer.size();
    if (t == 2) {
      std::cout << "naive scancount " << std::endl;
      std::cout << double(results[0]) / sum << " cycles/hit " << std::endl;
    }
    unified.start();
    fastscancount(counters, data, answer, threshold, cache_size);
    unified.end(results);
    if(answer.size() != expected) std::cerr << "bug" << std::endl;
    if (t == 2) {
      std::cout << "cache-sensitive scancount " << std::endl;
      std::cout << double(results[0]) / sum << " cycles/hit " << std::endl;
    }
  }
}
int main() {
  demo(20000000, 50000, 100, 3);
  return EXIT_SUCCESS;
}
