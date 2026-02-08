#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

#include "benchmarker.h"
#include "counters/event_counter.h"

double pretty_print(const std::string &name, size_t num_values,
                    std::pair<counters::event_aggregate, size_t> result) {
  const auto &agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  std::print("{:<50} : ", name);
  std::print(" {:5.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} GB/s ", double(num_values) / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:5.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:5.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

size_t HexEncode(const char *src, size_t slen, char *dst, size_t dlen) {
  dlen = slen * 2;
  for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    static const char hex[] = "0123456789abcdef";
    uint8_t val = static_cast<uint8_t>(src[i]);
    dst[k + 0] = hex[val >> 4];
    dst[k + 1] = hex[val & 15];
  }
  return dlen;
}

inline char nibble(uint8_t x) { return x + '0' + ((x > 9) * 39); }


inline char nibble_alt(uint8_t x) {
   uint8_t add = (x >= 10) ? ('a' - 10) : '0';
   return x + add;
}

size_t HexEncodeArithmetic(const char *src, size_t slen, char *dst,
                           size_t dlen) {
  dlen = slen * 2;
  for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    uint8_t val = static_cast<uint8_t>(src[i]);
    dst[k + 0] = nibble(val >> 4);
    dst[k + 1] = nibble(val & 15);
  }
  return dlen;
}

size_t HexEncodeArithmeticalt(const char *src, size_t slen, char *dst,
                           size_t dlen) {
  dlen = slen * 2;
  for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    uint8_t val = static_cast<uint8_t>(src[i]);
    dst[k + 0] = nibble_alt(val >> 4);
    dst[k + 1] = nibble_alt(val & 15);
  }
  return dlen;
}

size_t HexEncodeTable(const char *src, size_t slen, char *dst, size_t dlen) {
  constexpr static std::array<std::array<char, 2>, 256> hex_table = [] {
    std::array<std::array<char, 2>, 256> t{};
    constexpr char hex[] = "0123456789abcdef";
    for (int i = 0; i < 256; ++i) {
      t[i] = {hex[i >> 4], hex[i & 0xF]};
    }
    return t;
  }();
  dlen = slen * 2;
  for (size_t i = 0, k = 0; k < dlen; i += 1, k += 2) {
    uint8_t val = static_cast<uint8_t>(src[i]);
    dst[k + 0] = hex_table[val][0];
    dst[k + 1] = hex_table[val][1];
  }
  return dlen;
}

#ifdef __ARM_NEON

size_t HexEncodeNEON2(const char *src, size_t slen, char *dst, size_t dlen) {
  static const uint8_t hex_table[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
  uint8x16_t table = vld1q_u8(hex_table);
  size_t i = 0;
  size_t maxv = (slen - (slen%32));
  for (; i < maxv; i += 32) {
    uint8x16_t val1 = vld1q_u8((uint8_t*)src + i);
    uint8x16_t val2 = vld1q_u8((uint8_t*)src + i + 16);
    uint8x16_t high1 = vshrq_n_u8(val1, 4);
    uint8x16_t low1 = vandq_u8(val1, vdupq_n_u8(15));
    uint8x16_t high2 = vshrq_n_u8(val2, 4);
    uint8x16_t low2 = vandq_u8(val2, vdupq_n_u8(15));
    uint8x16_t high_chars1 = vqtbl1q_u8(table, high1);
    uint8x16_t low_chars1 = vqtbl1q_u8(table, low1);
    uint8x16_t high_chars2 = vqtbl1q_u8(table, high2);
    uint8x16_t low_chars2 = vqtbl1q_u8(table, low2);
    uint8x16x2_t zipped1 = {high_chars1, low_chars1};
    uint8x16x2_t zipped2 = {high_chars2, low_chars2};
    vst2q_u8((uint8_t*)dst + i*2, zipped1);
    vst2q_u8((uint8_t*)dst + i*2 + 32, zipped2);
  }

  if(i + 16 <= slen) {
    uint8x16_t val = vld1q_u8((uint8_t*)src + i);
    uint8x16_t high = vshrq_n_u8(val, 4);
    uint8x16_t low = vandq_u8(val, vdupq_n_u8(15));
    uint8x16_t high_chars = vqtbl1q_u8(table, high);
    uint8x16_t low_chars = vqtbl1q_u8(table, low);
    uint8x16x2_t zipped = {high_chars, low_chars};
    vst2q_u8((uint8_t*)dst + i*2, zipped);
    i += 16;
  }
  // handle remaining
  for (; i < slen; ++i) {
    uint8_t val = (uint8_t)src[i];
    dst[i*2] = hex_table[val >> 4];
    dst[i*2 + 1] = hex_table[val & 15];
  }
  return slen * 2;
}

size_t HexEncodeNEON4(const char *src, size_t slen, char *dst, size_t dlen) {
  static const uint8_t hex_table[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
  uint8x16_t table = vld1q_u8(hex_table);
  size_t i = 0;
  size_t maxv = (slen - (slen % 128));
  for (; i < maxv; i += 128) {
    uint8x16_t val1 = vld1q_u8((uint8_t*)src + i);
    uint8x16_t val2 = vld1q_u8((uint8_t*)src + i + 16);
    uint8x16_t val3 = vld1q_u8((uint8_t*)src + i + 32);
    uint8x16_t val4 = vld1q_u8((uint8_t*)src + i + 48);
    uint8x16_t val5 = vld1q_u8((uint8_t*)src + i + 64);
    uint8x16_t val6 = vld1q_u8((uint8_t*)src + i + 80);
    uint8x16_t val7 = vld1q_u8((uint8_t*)src + i + 96);
    uint8x16_t val8 = vld1q_u8((uint8_t*)src + i + 112);

    uint8x16_t high1 = vshrq_n_u8(val1, 4);
    uint8x16_t low1 = vandq_u8(val1, vdupq_n_u8(15));
    uint8x16_t high2 = vshrq_n_u8(val2, 4);
    uint8x16_t low2 = vandq_u8(val2, vdupq_n_u8(15));
    uint8x16_t high3 = vshrq_n_u8(val3, 4);
    uint8x16_t low3 = vandq_u8(val3, vdupq_n_u8(15));
    uint8x16_t high4 = vshrq_n_u8(val4, 4);
    uint8x16_t low4 = vandq_u8(val4, vdupq_n_u8(15));
    uint8x16_t high5 = vshrq_n_u8(val5, 4);
    uint8x16_t low5 = vandq_u8(val5, vdupq_n_u8(15));
    uint8x16_t high6 = vshrq_n_u8(val6, 4);
    uint8x16_t low6 = vandq_u8(val6, vdupq_n_u8(15));
    uint8x16_t high7 = vshrq_n_u8(val7, 4);
    uint8x16_t low7 = vandq_u8(val7, vdupq_n_u8(15));
    uint8x16_t high8 = vshrq_n_u8(val8, 4);
    uint8x16_t low8 = vandq_u8(val8, vdupq_n_u8(15));

    uint8x16_t high_chars1 = vqtbl1q_u8(table, high1);
    uint8x16_t low_chars1 = vqtbl1q_u8(table, low1);
    uint8x16_t high_chars2 = vqtbl1q_u8(table, high2);
    uint8x16_t low_chars2 = vqtbl1q_u8(table, low2);
    uint8x16_t high_chars3 = vqtbl1q_u8(table, high3);
    uint8x16_t low_chars3 = vqtbl1q_u8(table, low3);
    uint8x16_t high_chars4 = vqtbl1q_u8(table, high4);
    uint8x16_t low_chars4 = vqtbl1q_u8(table, low4);
    uint8x16_t high_chars5 = vqtbl1q_u8(table, high5);
    uint8x16_t low_chars5 = vqtbl1q_u8(table, low5);
    uint8x16_t high_chars6 = vqtbl1q_u8(table, high6);
    uint8x16_t low_chars6 = vqtbl1q_u8(table, low6);
    uint8x16_t high_chars7 = vqtbl1q_u8(table, high7);
    uint8x16_t low_chars7 = vqtbl1q_u8(table, low7);
    uint8x16_t high_chars8 = vqtbl1q_u8(table, high8);
    uint8x16_t low_chars8 = vqtbl1q_u8(table, low8);

    uint8x16x2_t zipped1 = {high_chars1, low_chars1};
    uint8x16x2_t zipped2 = {high_chars2, low_chars2};
    uint8x16x2_t zipped3 = {high_chars3, low_chars3};
    uint8x16x2_t zipped4 = {high_chars4, low_chars4};
    uint8x16x2_t zipped5 = {high_chars5, low_chars5};
    uint8x16x2_t zipped6 = {high_chars6, low_chars6};
    uint8x16x2_t zipped7 = {high_chars7, low_chars7};
    uint8x16x2_t zipped8 = {high_chars8, low_chars8};

    vst2q_u8((uint8_t*)dst + i*2, zipped1);
    vst2q_u8((uint8_t*)dst + i*2 + 32, zipped2);
    vst2q_u8((uint8_t*)dst + i*2 + 64, zipped3);
    vst2q_u8((uint8_t*)dst + i*2 + 96, zipped4);
    vst2q_u8((uint8_t*)dst + i*2 + 128, zipped5);
    vst2q_u8((uint8_t*)dst + i*2 + 160, zipped6);
    vst2q_u8((uint8_t*)dst + i*2 + 192, zipped7);
    vst2q_u8((uint8_t*)dst + i*2 + 224, zipped8);
  }

  // handle remaining with NEON2
  size_t maxv32 = (slen - (slen % 32));
  for (; i < maxv32; i += 32) {
    uint8x16_t val1 = vld1q_u8((uint8_t*)src + i);
    uint8x16_t val2 = vld1q_u8((uint8_t*)src + i + 16);

    uint8x16_t high1 = vshrq_n_u8(val1, 4);
    uint8x16_t low1 = vandq_u8(val1, vdupq_n_u8(15));
    uint8x16_t high2 = vshrq_n_u8(val2, 4);
    uint8x16_t low2 = vandq_u8(val2, vdupq_n_u8(15));

    uint8x16_t high_chars1 = vqtbl1q_u8(table, high1);
    uint8x16_t low_chars1 = vqtbl1q_u8(table, low1);
    uint8x16_t high_chars2 = vqtbl1q_u8(table, high2);
    uint8x16_t low_chars2 = vqtbl1q_u8(table, low2);

    uint8x16x2_t zipped1 = {high_chars1, low_chars1};
    uint8x16x2_t zipped2 = {high_chars2, low_chars2};

    vst2q_u8((uint8_t*)dst + i*2, zipped1);
    vst2q_u8((uint8_t*)dst + i*2 + 32, zipped2);
  }

  if (i + 16 <= slen) {
    uint8x16_t val = vld1q_u8((uint8_t*)src + i);
    uint8x16_t high = vshrq_n_u8(val, 4);
    uint8x16_t low = vandq_u8(val, vdupq_n_u8(15));
    uint8x16_t high_chars = vqtbl1q_u8(table, high);
    uint8x16_t low_chars = vqtbl1q_u8(table, low);
    uint8x16x2_t zipped = {high_chars, low_chars};
    vst2q_u8((uint8_t*)dst + i*2, zipped);
    i += 16;
  }
  // handle remaining
  for (; i < slen; ++i) {
    uint8_t val = (uint8_t)src[i];
    dst[i*2] = hex_table[val >> 4];
    dst[i*2 + 1] = hex_table[val & 15];
  }
  return slen * 2;
}
#endif

void collect_benchmark_results(size_t number) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> char_dist(32, 126); // printable ASCII
  std::vector<char> strings(number);
  for (size_t i = 0; i < number; ++i) {
    strings[i] = static_cast<char>(char_dist(gen));
  }
  std::vector<char> output_classic(number * 2);

  auto to_hex = [&strings, &output_classic]() {
    HexEncode(strings.data(), strings.size(), output_classic.data(),
              output_classic.size());
  };
  auto to_hex_arth = [&strings, &output_classic]() {
    HexEncodeArithmetic(strings.data(), strings.size(), output_classic.data(),
                        output_classic.size());
  };
  auto to_hex_arth_alt = [&strings, &output_classic]() {
    HexEncodeArithmeticalt(strings.data(), strings.size(), output_classic.data(),
                        output_classic.size());
  };
  auto to_hex_table = [&strings, &output_classic]() {
    HexEncodeTable(strings.data(), strings.size(), output_classic.data(),
                   output_classic.size());
  };
#ifdef __ARM_NEON
  auto to_hex_neon2 = [&strings, &output_classic]() {
    HexEncodeNEON2(strings.data(), strings.size(), output_classic.data(),
                  output_classic.size());
  };
  auto neon_result2 = pretty_print("to_hex_neon2", number, bench(to_hex_neon2));
#endif
#ifdef __ARM_NEON
  auto to_hex_neon4 = [&strings, &output_classic]() {
    HexEncodeNEON4(strings.data(), strings.size(), output_classic.data(),
                  output_classic.size());
  };
  auto neon_result4 = pretty_print("to_hex_neon4", number, bench(to_hex_neon4));
#endif
  auto classic_result = pretty_print("to_hex", number, bench(to_hex));
  auto arth_result =
      pretty_print("to_hex_arithmetic", number, bench(to_hex_arth));
  auto arth_alt_result =
      pretty_print("to_hex_arithmetic_alt", number, bench(to_hex_arth_alt));
  auto table_result = pretty_print("to_hex_table", number, bench(to_hex_table));
}

int main(int argc, char **argv) { collect_benchmark_results(10000); }
