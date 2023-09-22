
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

#include "parse_integer.h"

#include <x86intrin.h>

using simd8x32 = __m256i;
using simd16x16 = __m256i;
using simd32x4 = __m128i;
using simd16x8 = __m128i;
using simd8x16 = __m128i;

inline simd32x4 parse_8digit_integers_simd_reverse(simd8x32 base10_8bit) {
  const simd8x32 DIGIT_VALUE_BASE10_8BIT =
      _mm256_set_epi8(1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1,
                      10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10);
  const simd8x16 DIGIT_VALUE_BASE10E2_8BIT = _mm_set_epi8(
      1, 100, 1, 100, 1, 100, 1, 100, 1, 100, 1, 100, 1, 100, 1, 100);
  const simd16x8 DIGIT_VALUE_BASE10E4_16BIT =
      _mm_set_epi16(1, 10000, 1, 10000, 1, 10000, 1, 10000);
  // Multiply pairs of base-10 digits by [10,1] and add them to create 16
  // base-10^2 digits.
  simd16x16 base10e2_16bit =
      _mm256_maddubs_epi16(base10_8bit, DIGIT_VALUE_BASE10_8BIT);
  simd8x16 base10e2_8bit = _mm256_cvtepi16_epi8(base10e2_16bit);

  // Multiply pairs of base-10^2 digits by [10^2,1] and add them to create a 16
  // base-10^4 digits.
  simd16x8 base10e4_16bit =
      _mm_maddubs_epi16(base10e2_8bit, DIGIT_VALUE_BASE10E2_8BIT);
  // Multiply pairs of base-10^4 digits by [10^4,1] and add them to create an 8
  // base-10^8 digits.
  simd32x4 base10e8_32bit =
      _mm_madd_epi16(base10e4_16bit, DIGIT_VALUE_BASE10E4_16BIT);
  return base10e8_32bit;
}

// Based on an initial design by John Keiser.
inline bool
parse_unsigned_avx512_inline(const char *start,
                                         const char *end, uint64_t& value) noexcept {
  size_t digit_count = size_t(end - start);
  if ((digit_count == 0) || (digit_count > 20)) {
    return false;
  }
  if ((digit_count > 1) && ('0' == *start)) {
    return false;
  }
  // Load bytes
  const simd8x32 ASCII_ZERO = _mm256_set1_epi8('0');
  const simd8x32 NINE = _mm256_set1_epi8(9);
  uint32_t mask = uint32_t(0xFFFFFFFF) << (start - end + 32);
  simd8x32 in = _mm256_maskz_loadu_epi8(mask, end - 32);
  simd8x32 base10_8bit = _mm256_maskz_sub_epi8(mask, in, ASCII_ZERO);
  auto nondigits = _mm256_mask_cmpgt_epu8_mask(mask, base10_8bit, NINE);
  if (nondigits) {
    return false;
  }

  // Convert bytes to a 32-digit base-10 integer by subtracting '0'.
  simd32x4 base10e8_32bit = parse_8digit_integers_simd_reverse(base10_8bit);

  // Maximum 64-bit unsigned integer is 1844 67440737 09551615 (20 digits)
  uint64_t result_1digit = (uint64_t)_mm_extract_epi32(base10e8_32bit, 3);
  if ((mask & 0xffffff) == 0) {
    value = result_1digit;
    return true;
  }

  uint64_t middle_part = (uint64_t)_mm_extract_epi32(base10e8_32bit, 2);

  uint64_t result_2digit = result_1digit + 100000000 * middle_part;
  if ((mask & 0xffff) == 0) {
    value = result_2digit;
    return true;
  }
  uint64_t high_part = (uint64_t)_mm_extract_epi32(base10e8_32bit, 1);

  uint64_t result = result_2digit + 10000000000000000 * high_part;
  if (high_part > 1844 || result < result_2digit) {
    return false;
  }
  value = result;
  return true;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  std::vector<std::string> input;
  size_t numbers = 10000;
  size_t volume = 0;
  volatile uint64_t sum = 0;
  for (size_t i = 0; i < numbers; i++) {
    input.push_back(std::to_string(rand()));
    volume += input.back().size();
  }

  pretty_print(
      numbers, volume, "parse_unsigned_avx512_inline", bench([&input, &sum]() {
        uint64_t value;
        for (const std::string &s : input) {
          if (parse_unsigned_avx512_inline(s.data(), s.data() + s.size(), value)) {
            sum += value;
          }
        }
      }));

  pretty_print(
      numbers, volume, "parse_unsigned_avx512", bench([&input, &sum]() {
        uint64_t value;
        for (const std::string &s : input) {
          if (parse_unsigned_avx512(s.data(), s.data() + s.size(), value)) {
            sum += value;
          }
        }
      }));

  pretty_print(numbers, volume, "std::from_chars", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   uint64_t val;

                   auto [ptr, ec] =
                       std::from_chars(s.data(), s.data() + s.size(), val);

                   if (ec == std::errc()) {
                     sum += val;
                   }
                 }
               }));
  pretty_print(numbers, volume, "parse_unsigned", bench([&input, &sum]() {
                 uint64_t value;
                 for (const std::string &s : input) {
                   if (parse_unsigned(s.c_str(), value)) {
                     sum += value;
                   }
                 }
               }));

  pretty_print(
      numbers, volume, "parse_unsigned_bounded", bench([&input, &sum]() {
        uint64_t value;
        for (const std::string &s : input) {
          if (parse_unsigned_bounded(s.data(), s.data() + s.size(), value)) {
            sum += value;
          }
        }
      }));
}
