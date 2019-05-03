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
#include <x86intrin.h> // assume GCC/clang

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

static inline void basic_decoder(uint32_t *base_ptr, uint32_t &base,
                                 uint32_t idx, uint64_t bits) {
  while (bits != 0) {
    base_ptr[base] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base++;
  }
}

static inline void simdjson_decoder(uint32_t *base_ptr, uint32_t &base,
                                    uint32_t idx, uint64_t bits) {
  uint32_t cnt = hamming(bits);
  uint32_t next_base = base + cnt;
  while (bits != 0u) {
    base_ptr[base + 0] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 1] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 2] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 3] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 4] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 5] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 6] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 7] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base += 8;
  }
  base = next_base;
}

static inline void faster_decoder(uint32_t *base_ptr, uint32_t &base,
                                  uint32_t idx, uint64_t bits) {
  if (bits != 0u) {
    uint32_t cnt = hamming(bits);
    uint32_t next_base = base + cnt;
    base_ptr[base + 0] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 1] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 2] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 3] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 4] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 5] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 6] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 7] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    if (cnt > 8) {
      base_ptr[base + 8] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 9] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 10] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 11] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 12] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 13] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 14] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base_ptr[base + 15] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
    }
    if (cnt > 16) {
      base += 16;
      do {
        base_ptr[base] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
        bits = bits & (bits - 1);
        base++;
      } while (bits != 0);
    }
    base = next_base;
  }
}

static inline void fast_decoder(uint32_t *base_ptr, uint32_t &base,
                                uint32_t idx, uint64_t bits) {
  uint32_t cnt = hamming(bits);
  uint32_t next_base = base + cnt;
  if (bits != 0u) {
    base_ptr[base + 0] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 1] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 2] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 3] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
  }
  if (cnt > 4) {
    base_ptr[base + 4] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 5] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 6] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base_ptr[base + 7] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
  }
  if (cnt > 8) {
    base += 8;
    do {
      base_ptr[base] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
      base++;
    } while (bits != 0);
  }
  base = next_base;
}

template <void (*F)(uint32_t *, uint32_t &, uint32_t, uint64_t)>
void test(const char *filename, char target) {
  size_t wordcount;
  uint64_t *array = build_bitmap(filename, target, &wordcount);
  uint32_t *bigarray = new uint32_t[wordcount * 64];

  size_t iterations = 10;
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
  if (bigarray[0] == 0)
    printf("bogus\n.");
  printf("matches = %u words = %zu 1-bit density %4.3f %% \n", matches,
         wordcount, double(matches) / (64 * wordcount) * 100);
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

template <void (*F)(uint32_t *, uint32_t &, uint32_t, uint64_t)> bool unit() {
  uint32_t refdecoded[64];
  uint32_t decoded[64];
  for (size_t i = 0; i < 64; i++) {
    uint64_t x = 1;
    x <<= i;
    uint32_t matches = 0;
    faster_decoder(decoded, matches, 0, x);
    uint32_t refmatches = 0;
    basic_decoder(refdecoded, refmatches, 0, x);
    if (refmatches != matches) {
      printf("bug\n");
      return false;
    }
    for (size_t k = 0; k < matches; k++)
      if (decoded[k] != refdecoded[k]) {
        printf("bug\n");
        return false;
      }
  }
  for (size_t i = 0; i < 1000000; i++) {
    uint64_t x = rand();
    x <<= 32;
    x |= rand();
    uint32_t matches = 0;
    faster_decoder(decoded, matches, 0, x);
    uint32_t refmatches = 0;
    basic_decoder(refdecoded, refmatches, 0, x);
    if (refmatches != matches) {
      printf("bug\n");
      return false;
    }
    for (size_t k = 0; k < matches; k++) {
      if (decoded[k] != refdecoded[k]) {
        printf("bug\n");
        return false;
      }
    }
  }
  printf("Tests passed.\n");
  return true;
}
int main() {
  printf("fast_decoder:\n");
  unit<fast_decoder>();
  test<fast_decoder>("nfl.csv", ',');
  printf("simdjson_decoder:\n");
  unit<simdjson_decoder>();
  test<simdjson_decoder>("nfl.csv", ',');
  printf("basic_decoder:\n");
  unit<basic_decoder>();
  test<basic_decoder>("nfl.csv", ',');
  printf("faster_decoder:\n");
  unit<faster_decoder>();
  test<faster_decoder>("nfl.csv", ',');
  printf("\n");
}
