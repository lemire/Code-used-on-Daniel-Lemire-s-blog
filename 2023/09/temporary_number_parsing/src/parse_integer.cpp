
#include "parse_integer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

template <typename I> inline bool parse_digit(const uint8_t c, I &i) {
  const uint8_t digit = static_cast<uint8_t>(c - '0');
  if (digit > 9) {
    return false;
  }
  i = 10 * i + digit; // might overflow
  return true;
}

// Parse any number from 0 to 18,446,744,073,709,551,615
bool parse_unsigned(const char *src, uint64_t& value) noexcept {
  const uint8_t *p = (const uint8_t *)src;
  const uint8_t *const start_digits = p;
  uint64_t i = 0;
  while (parse_digit(*p, i)) {
    p++;
  }
  size_t digit_count = size_t(p - start_digits);
  if ((digit_count == 0) || (digit_count > 20)) {
    return false;
  }
  if ((digit_count > 1) && ('0' == *start_digits)) {
    return false;
  }
  if (digit_count == 20) {
    if (src[0] != uint8_t('1') || i <= uint64_t(INT64_MAX)) {
      return false;
    }
  }
  value = i;
  return true;
}

bool
parse_unsigned_bounded(const char *start_digits,
                       const char *end_digits, uint64_t& value) noexcept {
  size_t digit_count = size_t(end_digits - start_digits);
  if ((digit_count == 0) || (digit_count > 20)) {
    return false;
  }
  if (('0' == *start_digits) && (digit_count > 1)) {
    return false;
  }
  const uint8_t *p = (const uint8_t *)start_digits;
  uint64_t i = 0;
  while (parse_digit(*p, i)) {
    p++;
  }
  if ((const char *)p != end_digits) {
    return false;
  }
  if (digit_count == 20) {
    if (start_digits[0] != uint8_t('1') || i <= uint64_t(INT64_MAX)) {
      return false;
    }
  }
  value = i;
  return true;
}

bool
parse_unsigned_bounded_reverse(const char *start_digits,
                               const char *end_digits, uint64_t& value) noexcept {
  size_t digit_count = size_t(end_digits - start_digits);
  const static uint64_t table[21][10] = {
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
      {0, 10, 20, 30, 40, 50, 60, 70, 80, 90},
      {0, 100, 200, 300, 400, 500, 600, 700, 800, 900},
      {0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000},
      {0, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000},
      {0, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000,
       900000},
      {0, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000,
       8000000, 9000000},
      {0, 10000000, 20000000, 30000000, 40000000, 50000000, 60000000, 70000000,
       80000000, 90000000},
      {0, 100000000, 200000000, 300000000, 400000000, 500000000, 600000000,
       700000000, 800000000, 900000000},
      {0, 1000000000, 2000000000, 3000000000, 4000000000, 5000000000,
       6000000000, 7000000000, 8000000000, 9000000000},
      {0, 10000000000, 20000000000, 30000000000, 40000000000, 50000000000,
       60000000000, 70000000000, 80000000000, 90000000000},
      {0, 100000000000, 200000000000, 300000000000, 400000000000, 500000000000,
       600000000000, 700000000000, 800000000000, 900000000000},
      {0, 1000000000000, 2000000000000, 3000000000000, 4000000000000,
       5000000000000, 6000000000000, 7000000000000, 8000000000000,
       9000000000000},
      {0, 10000000000000, 20000000000000, 30000000000000, 40000000000000,
       50000000000000, 60000000000000, 70000000000000, 80000000000000,
       90000000000000},
      {0, 100000000000000, 200000000000000, 300000000000000, 400000000000000,
       500000000000000, 600000000000000, 700000000000000, 800000000000000,
       900000000000000},
      {0, 1000000000000000, 2000000000000000, 3000000000000000,
       4000000000000000, 5000000000000000, 6000000000000000, 7000000000000000,
       8000000000000000, 9000000000000000},
      {0, 10000000000000000, 20000000000000000, 30000000000000000,
       40000000000000000, 50000000000000000, 60000000000000000,
       70000000000000000, 80000000000000000, 90000000000000000},
      {0, 100000000000000000, 200000000000000000, 300000000000000000,
       400000000000000000, 500000000000000000, 600000000000000000,
       700000000000000000, 800000000000000000, 900000000000000000},
      {0, 1000000000000000000, 2000000000000000000, 3000000000000000000,
       4000000000000000000, 5000000000000000000, 6000000000000000000,
       7000000000000000000, 8000000000000000000, 9000000000000000000},
      {0, 10000000000000000000ULL, 1553255926290448384ULL,
       11553255926290448384ULL, 3106511852580896768ULL, 13106511852580896768ULL,
       4659767778871345152ULL, 14659767778871345152ULL, 6213023705161793536ULL,
       16213023705161793536ULL}};
  if ((digit_count == 0) || (digit_count > 20)) {
    return false;
  }
  if (('0' == *start_digits) && (digit_count > 1)) {
    return false;
  }
  const uint8_t *p = (const uint8_t *)end_digits;
  uint64_t i = 0;
  size_t counter = 0;
  while (counter < digit_count) {
    if ((p[-counter] - '0') > 9) {
      return false;
    }
    i += table[counter++][p[-counter] - '0'];
  }
  if (digit_count == 20) {
    if (start_digits[0] != uint8_t('1') || i <= uint64_t(INT64_MAX)) {
      return false;
    }
  }
  value = i;
  return true;
}

#include <x86intrin.h>

using simd8x32 = __m256i;
using simd16x16 = __m256i;
using simd32x4 = __m128i;
using simd16x8 = __m128i;
using simd8x16 = __m128i;

void print8x32(const char *name, simd8x32 a) {
  printf("%.40s :", name);
  uint8_t b[32];
  _mm256_storeu_si256((__m256i *)b, a);
  for (int i = 0; i < 32; i++) {
    printf(" %02x ", b[i]);
  }
  printf("\n");
}

void print16x16dec(const char *name, simd16x16 a) {
  printf("%.40s :", name);
  uint16_t b[16];
  _mm256_storeu_si256((__m256i *)b, a);
  for (int i = 0; i < 16; i++) {
    printf(" %u ", b[i]);
  }
  printf("\n");
}

void print8x16dec(const char *name, simd8x16 a) {
  printf("%.40s :", name);
  uint8_t b[16];
  _mm_storeu_si128((__m128i *)b, a);
  for (int i = 0; i < 16; i++) {
    printf(" %u ", b[i]);
  }
  printf("\n");
}

void print16x8dec(const char *name, simd16x8 a) {
  printf("%.40s :", name);
  uint16_t b[8];
  _mm_storeu_si128((__m128i *)b, a);
  for (int i = 0; i < 8; i++) {
    printf(" %u ", b[i]);
  }
  printf("\n");
}

void print32x4(const char *name, simd32x4 a) {
  printf("%.40s :", name);
  uint32_t b[4];
  _mm_storeu_si128((__m128i *)b, a);
  for (int i = 0; i < 4; i++) {
    printf(" %08x ", b[i]);
  }
  printf("\n");
}

void print32x4dec(const char *name, simd32x4 a) {
  printf("%.40s :", name);
  uint32_t b[4];
  _mm_storeu_si128((__m128i *)b, a);
  for (int i = 0; i < 4; i++) {
    printf(" %u ", b[i]);
  }
  printf("\n");
}

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
bool
parse_unsigned_avx512(const char *start,
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
