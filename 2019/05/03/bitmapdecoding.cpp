#ifdef __linux__
#include "liinux-perf-events.h"
#elif defined(__APPLE__) && defined(__aarch64__)
#include "m1cycles.cpp"
#include "m1cycles.h"

#else
#warning "Expect linux x64 or Apple + ARM"
#endif
#ifndef __GNUC__
#warning "We assume a GCC-like compiler."
#endif
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <vector>

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
  while (bits != 0) {
    base_ptr[base] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base++;
  }
}
#if defined(__aarch64__)
// based on an idea by Samuel Lee
void basic_arm_decoder(uint32_t *base_ptr, uint32_t &base, uint32_t idx,
                       uint64_t bits) {
  uint64_t rev_bits;
  __asm volatile("rbit %0, %1" : "=r"(rev_bits) : "r"(bits));
  while (rev_bits != 0) {
    int lz = __builtin_clzll(rev_bits);
    base_ptr[base] = static_cast<uint32_t>(idx) + lz;
    rev_bits = rev_bits & ~(int64_t(0x8000000000000000) >> lz);
    base++;
  }
}
#endif

static inline void just(uint32_t *base_ptr, uint32_t &base, uint32_t idx,
                        uint64_t bits) {
  uint32_t cnt = hamming(bits);
  base_ptr += base;
  base += cnt;
  base_ptr[0] = idx;
}

static inline void simdjson_decoder(uint32_t *base_ptr, uint32_t &base,
                                    uint32_t idx, uint64_t bits) {
  if (bits == 0)
    return;
  uint32_t cnt = hamming(bits);
  base_ptr += base;
  base += cnt;
  if (bits != 0) {
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
}

static inline void simdjson_decoder_rolled(uint32_t *base_ptr, uint32_t &base,
                                             uint32_t idx, uint64_t bits) {
  if (bits == 0)
    return;
  uint32_t cnt = hamming(bits);
  base_ptr += base;
  base += cnt;
  if (bits != 0) {
    for (int i = 0; i < 8; i++) {
      base_ptr[i] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
      bits = bits & (bits - 1);
    }

    if (cnt > 8) {
      for (int i = 8; i < 16; i++) {
        base_ptr[i] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
        bits = bits & (bits - 1);
      }
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

template <void (*F)(uint32_t *, uint32_t &, uint32_t, uint64_t)>
void test(const char *filename, char target) {
  size_t wordcount;
  uint64_t *array = build_bitmap(filename, target, &wordcount);
  uint32_t *bigarray = new uint32_t[wordcount * 64];
  wordcount = 10;
  size_t iterations = ITERATIONS;
#ifdef __linux__
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
#elif defined(__APPLE__) && defined(__aarch64__)
  performance_counters agg_min{1e300};
  performance_counters agg_avg{0.0};
#endif
  uint32_t matches = 0;
  for (uint32_t i = 0; i < iterations; i++) {
    matches = 0;
#ifdef __linux__
    unified.start();
#elif defined(__APPLE__) && defined(__aarch64__)
    performance_counters start = get_counters();
#endif
    for (size_t idx = 0; idx < wordcount; idx++) {
      F(bigarray, matches, idx, array[idx]);
    }
#ifdef __linux__
    unified.end(results);
    allresults.push_back(results);
#elif defined(__APPLE__) && defined(__aarch64__)
    performance_counters end = get_counters();
    performance_counters diff = end - start;
    agg_min = agg_min.min(diff);
    agg_avg += diff;
#endif
  }
  if ((bigarray[0] == 0) && (bigarray[117] == 0) && (bigarray[116] == 1))
    printf("bogus.\n");
  printf(
      "matches = %u words = %zu 1-bit density %4.3f %% 1-bit per word %4.3f ",
      matches, wordcount, double(matches) / (64 * wordcount) * 100,
      matches * 1.0 / wordcount);
  printf("bytes per index = %4.3f \n", wordcount * 64.0 / matches);
#ifdef __linux__

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
#elif defined(__APPLE__) && defined(__aarch64__)
  agg_avg /= iterations;
  printf(" %8.2f instructions/match (+/- %3.1f %%) ",
         agg_min.instructions / matches,
         (agg_avg.instructions - agg_min.instructions) / agg_avg.instructions);
  printf(" %8.2f cycles/match (+/- %3.1f %%) ", agg_min.cycles / matches,
         (agg_avg.cycles - agg_min.cycles) / agg_avg.cycles);
#endif
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
    F(decoded, matches, 0, x);
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
    F(decoded, matches, 0, x);
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
  return true;
}

/***
Insanity follows
***/

typedef struct lead_s {
  uint64_t word;
  uint32_t idx;
  uint32_t location;
} lead_t;

typedef struct buf_s {
  uint32_t matches;
  lead_t **buffer;
  lead_t **initbuffer;
} buf_t;

// look: you can do C in C++
void init_buf(buf_t *b, size_t capacity) {
  b->matches = 0;
  b->buffer = new lead_t *[65];
  b->initbuffer = new lead_t *[65];
  for (int i = 1; i <= 64; i++) {
    b->buffer[i] = new lead_t[capacity];
    b->initbuffer[i] = b->buffer[i];
  }
}
void reset_buf(buf_t *b) {
  b->matches = 0;
  for (int i = 1; i <= 64; i++) {
    b->buffer[i] = b->initbuffer[i];
  }
}

void destroy_buf(buf_t *b) {
  for (int i = 1; i <= 64; i++) {
    delete[] b->initbuffer[i];
  }
  delete[] b->buffer;
  delete[] b->initbuffer;
}

template <uint32_t width>
static void unpack(lead_t *begin, lead_t *end, uint32_t *out) {
  for (lead_t *j = begin; j != end; j++) {
    uint64_t bits = j->word;
    uint32_t *o = out + j->location;
    for (uint32_t i = 0; i < width; i++) {
      o[i] = j->idx + trailingzeroes(bits);
      bits = bits & (bits - 1);
    }
  }
}

/***
end of insanity
***/

int main(int argc, char **argv) {
#if defined(__APPLE__) && defined(__aarch64__)
  setup_performance_counters();
#endif
  printf("just scanning:\n");
  test<just>("nfl.csv", ',');

  printf("simdjson_decoder:\n");
  unit<simdjson_decoder>();
  test<simdjson_decoder>("nfl.csv", ',');

  printf("simdjson_decoder_rolled:\n");
  unit<simdjson_decoder_rolled>();
  test<simdjson_decoder_rolled>("nfl.csv", ',');

  printf("faster_decoder:\n");
  unit<faster_decoder>();
  test<faster_decoder>("nfl.csv", ',');

#if defined(__aarch64__)
  unit<basic_arm_decoder>();
  printf("basic_arm_decoder:\n");
  test<basic_arm_decoder>("nfl.csv", ',');
#endif

  unit<basic_decoder>();
  printf("basic_decoder:\n");
  test<basic_decoder>("nfl.csv", ',');
  printf("\n");
}
