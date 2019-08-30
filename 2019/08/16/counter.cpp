// we require Linux
#include "linux-perf-events.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <immintrin.h>

namespace {

// used by natefastscancount
uint32_t *natefastscancount_maincheck(uint8_t *counters, size_t &it,
                    const uint32_t * d, 
                    size_t start, size_t range,
                    size_t threshold, uint32_t *out) {
  uint8_t t = threshold + 1;
  uint32_t bigend = start + range;
  uint8_t * const deccounters = counters - start;
  size_t i = it;
  uint32_t val = d[i];
  while(val < bigend) {
      uint8_t * location = deccounters + val;
      uint8_t c = *location;
      c++;
      *location = c;
      if(c == t){*out++ = val;}  
      i++;
      val = d[i];
  }
  it = i;
  return out;
}

// used by natefastscancount
uint32_t *natefastscancount_finalcheck(uint8_t * counters, size_t &it,
                     const uint32_t  * d, size_t start,
                     size_t itend,
                     size_t threshold, uint32_t *out) {
  uint8_t t = threshold + 1;
  uint8_t * const deccounters = counters - start;
  size_t i = it;
  for (; i < itend; i++) {
    uint32_t val = d[i];
    uint8_t * location = deccounters + val;
    uint8_t c = *location;
    c++;
    *location = c;
    if (c == t) {
      *out++ = val;
    }
  }
  it = i;
  return out;
}
}

__attribute__((noinline)) void natefastscancount(
    std::vector<uint8_t> &counters, std::vector<std::vector<uint32_t>> &data,
    std::vector<uint32_t> &out, size_t threshold, size_t cache_size) {
  size_t cs = counters.size();
  size_t ds = data.size();
  // range is smallest of counters.size() and cache_size
  size_t range = cache_size > cs ? cs : cache_size;
  out.resize(out.size() + 4 * range); // let us add lots of capacity
  uint32_t *output = out.data();
  uint32_t *initout = out.data();
  std::vector<size_t> iters(ds);
  size_t countsofar = 0;
  // we are assuming that all vectors in data are non-empty
  for (size_t start = 0; start < cs; start += range) {
    // make sure that the capacity is sufficient
    countsofar = output - initout;
    if (out.size() - countsofar < range) {
      out.resize(out.size() + 4 * range);
      initout = out.data();
      output = out.data() + countsofar;
    }
    memset(counters.data(), 0, range);
    for (size_t c = 0; c < ds; c++) {
      size_t it = iters[c]; // recover where we were
      std::vector<uint32_t> &d = data[c];
      const size_t itend = d.size();
      if (it == itend) // check that there is data to be processed
        continue;      // exhausted
      // check if we need to be careful:
      bool near_the_end = (d[itend - 1] < start + range);
      if (near_the_end) {
        output = natefastscancount_finalcheck(counters.data(), it, d.data(), start, itend, threshold, output);
      } else {
        output = natefastscancount_maincheck(counters.data(), it, d.data(), start, range, threshold, output);
      }
      iters[c] = it; // store it for next round
    }
  }
  countsofar = output - initout;
  out.resize(countsofar);
}

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

static inline size_t find_next_gt(uint8_t* array, size_t size, uint8_t threshold) {
  size_t vsize = size / 32;
  __m256i* varray = (__m256i *)array;
  const __m256i comprand = _mm256_set1_epi8(threshold);
  int bits = 0;

  for (size_t i = 0; i < vsize; i++) {
    __m256i v = _mm256_loadu_si256(varray + i);
    __m256i cmp = _mm256_cmpgt_epi8(v, comprand);
    if ((bits = _mm256_movemask_epi8(cmp))) {
      return i * 32 + __builtin_ctz(bits);
    }
  }

  // tail handling
  for (size_t i = vsize * 32; i < size; i++) {
    auto v = array[i];
    if (v > threshold) return i;
  }

  return SIZE_MAX;
}

__attribute__((noinline))
void populate_hits_avx(std::vector<uint8_t> &counters, size_t range,
    size_t threshold, size_t start, std::vector<uint32_t> &out) {
  uint8_t* array = counters.data();
  size_t ro = range;
  while (true) {
    size_t next = find_next_gt(array, range, (uint8_t)threshold);
    if (next == SIZE_MAX) break;
    out.push_back(start + next);
    range -= (next + 1);
    array += (next + 1);
  }
}

__attribute__((noinline))
void update_counters(const uint32_t*& it_, uint8_t* counters, uint32_t range_end) {
  const uint32_t* it = it_;
  for (uint32_t e; (e = *it) < range_end; ++it) {
    counters[e]++;
  }
  it_ = it;
}

__attribute__((noinline))
void update_counters_final(const uint32_t*& it_, const uint32_t *end, uint8_t* counters) {
  uint64_t e;
  const uint32_t* it = it_;
  for (; it != end; it++) {
    counters[*it]++;
  }
  it_ = end;
}

// even faster scancount
//
template <size_t cache_size>
__attribute__((noinline))
void fasterscancount(std::vector<uint8_t> &counters,
              std::vector<std::vector<uint32_t>> &data,
              std::vector<uint32_t> &out, size_t threshold) {
  out.clear();
  const size_t dsize = data.size();

  struct data_info {
    const uint32_t *cur; // current pointer into data
    const uint32_t *end; // pointer to end
    uint32_t last;       // value of last element
    data_info(const uint32_t *cur, const uint32_t *end, uint32_t last) : cur{cur}, end{end}, last{last} {}
  };

  std::vector<data_info> iter_data;
  iter_data.reserve(dsize);
  for (auto& d : data) {
    iter_data.emplace_back(d.data(), d.data() + d.size(), d.back());
  }

  uint32_t csize = counters.size();
  auto cdata = counters.data();
  for (uint32_t start = 0; start < csize; start += cache_size) {
    memset(cdata, 0, cache_size * sizeof(counters[0]));
    for (auto& id : iter_data) {
      // determine if the loop will end because we get to the end of
      // data, or because we get to the end of the range
      if (__builtin_expect(id.last >= start + cache_size, 1)) {
        // the iteration is guaranteed to end because an element becomes >=
        // range_end, so we don't need to check for end of data
        update_counters(id.cur, cdata - start, start + cache_size);
      } else {
        // the iteration is guaranteed to end because we get to the end of the data
        update_counters_final(id.cur, id.end, cdata - start);
      }
    }

    populate_hits_avx(counters, cache_size, threshold, start, out);
  }
}

template <typename F>
void bench(F f,
          const std::string& name,
          LinuxEvents<PERF_TYPE_HARDWARE>& unified,
          std::vector<unsigned long long>& results,
          std::vector<uint32_t>& answer,
          size_t sum,
          size_t expected,
          bool print) {
    unified.start();
    f(answer);
    unified.end(results);
    if (answer.size() != expected) std::cerr << "bug: expected " << expected
        << " but got " << answer.size() << "\n";
    if (print) {
      std::cout << name << std::endl;
      std::cout << double(results[0]) / sum << " cycles/element " << std::endl;
    }
}

#define REPEATS 10
#define CACHE_SIZE 262144 // roughly my L2

#ifndef CACHE_SIZEB
#define CACHE_SIZEB 40000
#endif

void demo(size_t N, size_t length, size_t array_count, size_t threshold) {
  std::vector<std::vector<uint32_t>> data(array_count);
  std::vector<uint8_t> counters(N);
  std::vector<uint32_t> answer;
  answer.reserve(N);
  std::cout << "size of the counter array "
            << double(counters.size()) / (1024 * 1024) << " MB" << std::endl;
  size_t cache_size = CACHE_SIZE;
  std::cout << "size of the small counter array (cache-sized) "
            << double(cache_size) / 1024 << " kB" << std::endl;
  size_t tiny_cache_size = 32768; // roughly my L1
  std::cout << "size of the tiny small counter array (cache-sized) "
            << double(tiny_cache_size) / 1024 << " kB" << std::endl;
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
  scancount(counters, data, answer, threshold);
  const size_t expected = answer.size();
  std::cout << "Got " << expected << " hits\n";
  for (size_t t = 0; t < REPEATS; t++) {

    bool last = (t == REPEATS - 1);

    bench([&](std::vector<uint32_t>& ans) { scancount(counters, data, ans, threshold); },
        "naive scancount", unified, results, answer, sum, expected, last);

    bench([&](std::vector<uint32_t>& ans) { fastscancount(counters, data, answer, threshold, cache_size); },
        "cache-sensitive scancount", unified, results, answer, sum, expected, last);

    bench([&](std::vector<uint32_t>& ans) { natefastscancount(counters, data, answer, threshold, tiny_cache_size); },
        "optimized cache-sensitive scancount", unified, results, answer, sum, expected, last);


    bench([&](std::vector<uint32_t>& ans) { fasterscancount<CACHE_SIZEB>(counters, data, answer, threshold); },
        "SIMD-based scancount (Travis Downs)", unified, results, answer, sum, expected, last);
  }

}
int main() {
  demo(20000000, 50000, 100, 3);
  return EXIT_SUCCESS;
}
