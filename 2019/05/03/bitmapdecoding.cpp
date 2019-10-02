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
  while (bits != 0) {
    base_ptr[base] = static_cast<uint32_t>(idx) + trailingzeroes(bits);
    bits = bits & (bits - 1);
    base++;
  }
}
#if defined(__aarch64__)
void basic_arm_decoder(uint32_t *base_ptr, uint32_t &base, uint32_t idx,
                   uint64_t bits) {
  uint64_t rev_bits;
  __asm volatile ("rbit %0, %1" : "=r" (rev_bits) : "r" (bits) );
  while (rev_bits != 0) {
    int lz = __builtin_clzll(rev_bits);
    base_ptr[base] = static_cast<uint32_t>(idx) + lz;
    rev_bits = rev_bits & ~(int64_t(0x1000000000000000) >> lz);
    base++;
  }
}
#endif

static inline void just(uint32_t *base_ptr, uint32_t & base, uint32_t idx, uint64_t bits) {
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

static inline void sorter(uint64_t bits, uint32_t idx, buf_t *b) {
  if (bits != 0) {
    uint32_t cnt = hamming(bits);
    b->buffer[cnt]->idx = idx;
    b->buffer[cnt]->location = b->matches;
    b->buffer[cnt]->word = bits;
    b->buffer[cnt]++;
    b->matches += cnt;
  }
}

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
__attribute__((optimize("unroll-loops"))) static void
unpack(lead_t *begin, lead_t *end, uint32_t *out) {
  for (lead_t *j = begin; j != end; j++) {
    uint64_t bits = j->word;
    uint32_t *o = out + j->location;
    for (uint32_t i = 0; i < width; i++) {
      o[i] = j->idx + trailingzeroes(bits);
      bits = bits & (bits - 1);
    }
  }
}

static void sortme(buf_t *bfinal, uint32_t *out) {
  // I can copy paste faster than anyone
  if (bfinal->initbuffer[1] != bfinal->buffer[1])
    unpack<1>(bfinal->initbuffer[1], bfinal->buffer[1], out);
  if (bfinal->initbuffer[2] != bfinal->buffer[2])
    unpack<2>(bfinal->initbuffer[2], bfinal->buffer[2], out);
  if (bfinal->initbuffer[3] != bfinal->buffer[3])
    unpack<3>(bfinal->initbuffer[3], bfinal->buffer[3], out);
  if (bfinal->initbuffer[4] != bfinal->buffer[4])
    unpack<4>(bfinal->initbuffer[4], bfinal->buffer[4], out);
  if (bfinal->initbuffer[5] != bfinal->buffer[5])
    unpack<5>(bfinal->initbuffer[5], bfinal->buffer[5], out);
  if (bfinal->initbuffer[6] != bfinal->buffer[6])
    unpack<6>(bfinal->initbuffer[6], bfinal->buffer[6], out);
  if (bfinal->initbuffer[7] != bfinal->buffer[7])
    unpack<7>(bfinal->initbuffer[7], bfinal->buffer[7], out);
  if (bfinal->initbuffer[8] != bfinal->buffer[8])
    unpack<8>(bfinal->initbuffer[8], bfinal->buffer[8], out);
  if (bfinal->initbuffer[9] != bfinal->buffer[9])
    unpack<9>(bfinal->initbuffer[9], bfinal->buffer[9], out);
  if (bfinal->initbuffer[10] != bfinal->buffer[10])
    unpack<10>(bfinal->initbuffer[10], bfinal->buffer[10], out);
  if (bfinal->initbuffer[11] != bfinal->buffer[11])
    unpack<11>(bfinal->initbuffer[11], bfinal->buffer[11], out);
  if (bfinal->initbuffer[12] != bfinal->buffer[12])
    unpack<12>(bfinal->initbuffer[12], bfinal->buffer[12], out);
  if (bfinal->initbuffer[13] != bfinal->buffer[13])
    unpack<13>(bfinal->initbuffer[13], bfinal->buffer[13], out);
  if (bfinal->initbuffer[14] != bfinal->buffer[14])
    unpack<14>(bfinal->initbuffer[14], bfinal->buffer[14], out);
  if (bfinal->initbuffer[15] != bfinal->buffer[15])
    unpack<15>(bfinal->initbuffer[15], bfinal->buffer[15], out);
  if (bfinal->initbuffer[16] != bfinal->buffer[16])
    unpack<16>(bfinal->initbuffer[16], bfinal->buffer[16], out);
  if (bfinal->initbuffer[17] != bfinal->buffer[17])
    unpack<17>(bfinal->initbuffer[17], bfinal->buffer[17], out);
  if (bfinal->initbuffer[18] != bfinal->buffer[18])
    unpack<18>(bfinal->initbuffer[18], bfinal->buffer[18], out);
  if (bfinal->initbuffer[19] != bfinal->buffer[19])
    unpack<19>(bfinal->initbuffer[19], bfinal->buffer[19], out);
  if (bfinal->initbuffer[20] != bfinal->buffer[20])
    unpack<20>(bfinal->initbuffer[20], bfinal->buffer[20], out);
  if (bfinal->initbuffer[21] != bfinal->buffer[21])
    unpack<21>(bfinal->initbuffer[21], bfinal->buffer[21], out);
  if (bfinal->initbuffer[22] != bfinal->buffer[22])
    unpack<22>(bfinal->initbuffer[22], bfinal->buffer[22], out);
  if (bfinal->initbuffer[23] != bfinal->buffer[23])
    unpack<23>(bfinal->initbuffer[23], bfinal->buffer[23], out);
  if (bfinal->initbuffer[24] != bfinal->buffer[24])
    unpack<24>(bfinal->initbuffer[24], bfinal->buffer[24], out);
  if (bfinal->initbuffer[25] != bfinal->buffer[25])
    unpack<25>(bfinal->initbuffer[25], bfinal->buffer[25], out);
  if (bfinal->initbuffer[26] != bfinal->buffer[26])
    unpack<26>(bfinal->initbuffer[26], bfinal->buffer[26], out);
  if (bfinal->initbuffer[27] != bfinal->buffer[27])
    unpack<27>(bfinal->initbuffer[27], bfinal->buffer[27], out);
  if (bfinal->initbuffer[28] != bfinal->buffer[28])
    unpack<28>(bfinal->initbuffer[28], bfinal->buffer[28], out);
  if (bfinal->initbuffer[29] != bfinal->buffer[29])
    unpack<29>(bfinal->initbuffer[29], bfinal->buffer[29], out);
  if (bfinal->initbuffer[30] != bfinal->buffer[30])
    unpack<30>(bfinal->initbuffer[30], bfinal->buffer[30], out);
  if (bfinal->initbuffer[31] != bfinal->buffer[31])
    unpack<31>(bfinal->initbuffer[31], bfinal->buffer[31], out);
  if (bfinal->initbuffer[32] != bfinal->buffer[32])
    unpack<32>(bfinal->initbuffer[32], bfinal->buffer[32], out);
  if (bfinal->initbuffer[33] != bfinal->buffer[33])
    unpack<33>(bfinal->initbuffer[33], bfinal->buffer[33], out);
  if (bfinal->initbuffer[34] != bfinal->buffer[34])
    unpack<34>(bfinal->initbuffer[34], bfinal->buffer[34], out);
  if (bfinal->initbuffer[35] != bfinal->buffer[35])
    unpack<35>(bfinal->initbuffer[35], bfinal->buffer[35], out);
  if (bfinal->initbuffer[36] != bfinal->buffer[36])
    unpack<36>(bfinal->initbuffer[36], bfinal->buffer[36], out);
  if (bfinal->initbuffer[37] != bfinal->buffer[37])
    unpack<37>(bfinal->initbuffer[37], bfinal->buffer[37], out);
  if (bfinal->initbuffer[38] != bfinal->buffer[38])
    unpack<38>(bfinal->initbuffer[38], bfinal->buffer[38], out);
  if (bfinal->initbuffer[39] != bfinal->buffer[39])
    unpack<39>(bfinal->initbuffer[39], bfinal->buffer[39], out);
  if (bfinal->initbuffer[40] != bfinal->buffer[40])
    unpack<40>(bfinal->initbuffer[40], bfinal->buffer[40], out);
  if (bfinal->initbuffer[41] != bfinal->buffer[41])
    unpack<41>(bfinal->initbuffer[41], bfinal->buffer[41], out);
  if (bfinal->initbuffer[42] != bfinal->buffer[42])
    unpack<42>(bfinal->initbuffer[42], bfinal->buffer[42], out);
  if (bfinal->initbuffer[43] != bfinal->buffer[43])
    unpack<43>(bfinal->initbuffer[43], bfinal->buffer[43], out);
  if (bfinal->initbuffer[44] != bfinal->buffer[44])
    unpack<44>(bfinal->initbuffer[44], bfinal->buffer[44], out);
  if (bfinal->initbuffer[45] != bfinal->buffer[45])
    unpack<45>(bfinal->initbuffer[45], bfinal->buffer[45], out);
  if (bfinal->initbuffer[46] != bfinal->buffer[46])
    unpack<46>(bfinal->initbuffer[46], bfinal->buffer[46], out);
  if (bfinal->initbuffer[47] != bfinal->buffer[47])
    unpack<47>(bfinal->initbuffer[47], bfinal->buffer[47], out);
  if (bfinal->initbuffer[48] != bfinal->buffer[48])
    unpack<48>(bfinal->initbuffer[48], bfinal->buffer[48], out);
  if (bfinal->initbuffer[49] != bfinal->buffer[49])
    unpack<49>(bfinal->initbuffer[49], bfinal->buffer[49], out);
  if (bfinal->initbuffer[50] != bfinal->buffer[50])
    unpack<50>(bfinal->initbuffer[50], bfinal->buffer[50], out);
  if (bfinal->initbuffer[51] != bfinal->buffer[51])
    unpack<51>(bfinal->initbuffer[51], bfinal->buffer[51], out);
  if (bfinal->initbuffer[52] != bfinal->buffer[52])
    unpack<52>(bfinal->initbuffer[52], bfinal->buffer[52], out);
  if (bfinal->initbuffer[53] != bfinal->buffer[53])
    unpack<53>(bfinal->initbuffer[53], bfinal->buffer[53], out);
  if (bfinal->initbuffer[54] != bfinal->buffer[54])
    unpack<54>(bfinal->initbuffer[54], bfinal->buffer[54], out);
  if (bfinal->initbuffer[55] != bfinal->buffer[55])
    unpack<55>(bfinal->initbuffer[55], bfinal->buffer[55], out);
  if (bfinal->initbuffer[56] != bfinal->buffer[56])
    unpack<56>(bfinal->initbuffer[56], bfinal->buffer[56], out);
  if (bfinal->initbuffer[57] != bfinal->buffer[57])
    unpack<57>(bfinal->initbuffer[57], bfinal->buffer[57], out);
  if (bfinal->initbuffer[58] != bfinal->buffer[58])
    unpack<58>(bfinal->initbuffer[58], bfinal->buffer[58], out);
  if (bfinal->initbuffer[59] != bfinal->buffer[59])
    unpack<59>(bfinal->initbuffer[59], bfinal->buffer[59], out);
  if (bfinal->initbuffer[60] != bfinal->buffer[60])
    unpack<60>(bfinal->initbuffer[60], bfinal->buffer[60], out);
  if (bfinal->initbuffer[61] != bfinal->buffer[61])
    unpack<61>(bfinal->initbuffer[61], bfinal->buffer[61], out);
  if (bfinal->initbuffer[62] != bfinal->buffer[62])
    unpack<62>(bfinal->initbuffer[62], bfinal->buffer[62], out);
  if (bfinal->initbuffer[63] != bfinal->buffer[63])
    unpack<63>(bfinal->initbuffer[63], bfinal->buffer[63], out);
  if (bfinal->initbuffer[64] != bfinal->buffer[64])
    unpack<64>(bfinal->initbuffer[64], bfinal->buffer[64], out);
}

void fasttest(const char *filename, char target) {
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
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  std::vector<std::vector<unsigned long long>> allresults;
  results.resize(evts.size());
  uint32_t matches = 0;
  buf_t b;
  init_buf(&b, 64 * wordcount);
  for (uint32_t i = 0; i < iterations; i++) {
    matches = 0;
    unified.start();
    reset_buf(&b);
    for (size_t idx = 0; idx < wordcount; idx++) {
      sorter(array[idx], idx, &b);
    }
    sortme(&b, bigarray);
    matches = b.matches;
    unified.end(results);
    allresults.push_back(results);
  }
  destroy_buf(&b);
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

/***
end of insanity
***/

int main(int argc, char **argv) {
  printf("just scanning:\n");
  test<just>("nfl.csv", ',');
  printf("simdjson_decoder:\n");
  unit<simdjson_decoder>();
  test<simdjson_decoder>("nfl.csv", ',');
  printf("basic_decoder:\n");
  unit<basic_decoder>();
#if defined(__aarch64__)
  // does not work:
  //test<basic_arm_decoder>("nfl.csv", ',');
#endif
  test<basic_decoder>("nfl.csv", ',');
  printf("\n");
}
