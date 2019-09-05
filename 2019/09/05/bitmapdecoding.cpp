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
#ifdef __x86_64__
#include <x86intrin.h> // assume GCC/clang
#elif defined(__aarch64__)
#include <arm_neon.h>
#endif
#define ITERATIONS 10

/* result might be undefined when input_num is zero */
static inline int trailingzeroes(uint64_t input_num) {
#ifdef __BMI2__
  return _tzcnt_u64(input_num);
#else
  return __builtin_ctzll(input_num);
#endif
}

/* result might be undefined when input_num is zero */
static inline int leadingzeroes(uint64_t input_num) {
#ifdef __BMI2__
  return _lzcnt_u64(input_num);
#else
  return __builtin_clzll(input_num);
#endif
}

/* result might be undefined when input_num is zero */
static inline int hamming(uint64_t input_num) {
#ifdef __POPCOUNT__
  return _popcnt64(input_num);
#else
  return __builtin_popcountll(input_num);
#endif
}

char *load_text_file(const char *filename, size_t *len) {
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp != nullptr) {
    std::fseek(fp, 0, SEEK_END);
    *len = std::ftell(fp);
    std::rewind(fp);
    char *data = new char[*len];
    size_t readb = std::fread(data, 1, *len, fp);
    std::fclose(fp);
    if (readb != *len) {
      throw std::runtime_error("could not read the data");
    }
    return data;
  } else {
    std::cout << "can't open " << filename << std::endl;
    return nullptr;
  }
}

uint64_t *build_bitmap(const char *filename, char target, size_t *wordcount) {
  size_t len;
  char *filedata = load_text_file(filename, &len);
  if (filedata == nullptr)
    return nullptr;
  *wordcount = len / 64;
  uint64_t *data = new uint64_t[*wordcount];
  memset(data, 0, *wordcount * sizeof(uint64_t));
  for (size_t i = 0; i < *wordcount * 64; i++) {
    if ((filedata[i] == target) || ((unsigned char)filedata[i] < 0x20)) {
      data[i / 64] |= (UINT64_C(1) << (i % 64));
    }
  }
  delete[] filedata;
  return data;
}

void basic_decoder(uint32_t *base_ptr, uint32_t &base, uint32_t idx,
                   uint64_t bits) {
  uint32_t b = base;      
  while (bits != 0) {
    base_ptr[b] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    b++;
  }
  base = b;
}

void backward_decoder(uint32_t *base_ptr, uint32_t &base, uint32_t idx,
                   uint64_t bits) {
  uint32_t b = base;      
  uint64_t constant = 0x8000000000000000;
  while (bits != 0) {
    uint32_t lz = leadingzeroes(bits);
    base_ptr[b] = idx + lz;
    bits = bits ^ (constant >> lz);
     b++;
  }
  base = b;
}


static inline void simdjson_decoder(uint32_t *base_ptr, uint32_t &base,
                                      uint32_t idx, uint64_t bits) {
  if (bits == 0)
    return;
  uint32_t cnt = hamming(bits);
  base_ptr += base;
  base += cnt;
  if (true) {
    base_ptr[0] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[1] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[2] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[3] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[4] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[5] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[6] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[7] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
  }
  if (cnt > 8) {
    base_ptr[8] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[9] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[10] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[11] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[12] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[13] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[14] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[15] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    if (cnt > 16) { // unluckly
      base_ptr += 16;
      do {
        base_ptr[0] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
        bits = bits & (bits - 1);
        base_ptr++;
      } while (bits != 0);
    }
  }
}



template <void (*F)(uint32_t *, uint32_t &, uint32_t, uint64_t)>
void test(const char *filename, char target) {
  size_t wordcount;
  uint64_t *array = build_bitmap(filename, target, &wordcount);
  uint32_t *bigarray = new uint32_t[wordcount * 64];

  size_t iterations = ITERATIONS;
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  evts.push_back(PERF_COUNT_HW_BRANCH_MISSES);
  evts.push_back(PERF_COUNT_HW_CACHE_REFERENCES);
  evts.push_back(PERF_COUNT_HW_CACHE_MISSES);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  std::vector<std::vector<unsigned long long>> allresults;
  results.resize(evts.size());
  uint32_t matches = 0;
  for (uint32_t i = 0; i < iterations; i++) {
    matches = 0;
    unified.start();
    for (size_t idx = 0; idx < wordcount; idx++) {
      F(bigarray, matches, idx, array[idx]);
    }
    unified.end(results);
    allresults.push_back(results);
  }
  if ((bigarray[0] == 0) && (bigarray[117] == 0))
    printf("bogus\n.");
  printf(
      "matches = %u words = %zu 1-bit density %4.3f %% 1-bit per word %4.3f \n",
      matches, wordcount, double(matches) / (64 * wordcount) * 100,
      matches * 1.0 / wordcount);
  printf("bytes per index = %4.3f \n", wordcount * 64.0 / matches);
  std::vector<unsigned long long> mins = compute_mins(allresults);
  std::vector<double> avg = compute_averages(allresults);
  printf("instructions per cycle %4.2f, cycles per value set:  "
         "%4.3f, "
         "instructions per value set: %4.3f, cycles per word: %4.3f, "
         "instructions per word: %4.3f \n",
         double(mins[1]) / mins[0], double(mins[0]) / matches,
         double(mins[1]) / matches, double(mins[0]) / wordcount,
         double(mins[1]) / wordcount);
  printf(" cycles per input byte %4.2f instructions per input byte %4.2f \n",
         double(mins[0]) / (64 * wordcount),
         double(mins[1]) / (64 * wordcount));
  // first we display mins
  printf("min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu "
         "cache ref., %8llu cache mis.\n",
         mins[0], mins[1], mins[2], mins[3], mins[4]);
  printf("avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f "
         "cache ref., %8.1f cache mis.\n",
         avg[0], avg[1], avg[2], avg[3], avg[4]);
  printf("\n");
  delete[] array;
  delete[] bigarray;
}

int main(int argc, char **argv) {
  printf("backward_decoder:\n");
  test<backward_decoder>("nfl.csv", ',');
  printf("basic_decoder:\n");
  test<basic_decoder>("nfl.csv", ',');
  printf("\n");
}
