#ifndef CHAMPAGNE_LEMIRE_H
#define CHAMPAGNE_LEMIRE_H

#include <bit>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#ifdef _MSC_VER
#ifdef __clang__
#define WE_HAVE_CLANGCL 1
#else
#define WE_HAVE_VISUAL_STUDIO 1
#include <intrin.h>
#endif
#endif


constexpr uint8_t FloatMantissaBits = 23;
constexpr uint8_t FloatExponentBits = 8;

constexpr uint8_t DoubleMantissaBits = 52;
constexpr uint8_t DoubleExponentBits = 11;
static const char hundreds_digit_table[200] = {
    '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0',
    '7', '0', '8', '0', '9', '1', '0', '1', '1', '1', '2', '1', '3', '1', '4',
    '1', '5', '1', '6', '1', '7', '1', '8', '1', '9', '2', '0', '2', '1', '2',
    '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
    '3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3',
    '7', '3', '8', '3', '9', '4', '0', '4', '1', '4', '2', '4', '3', '4', '4',
    '4', '5', '4', '6', '4', '7', '4', '8', '4', '9', '5', '0', '5', '1', '5',
    '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
    '6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6',
    '7', '6', '8', '6', '9', '7', '0', '7', '1', '7', '2', '7', '3', '7', '4',
    '7', '5', '7', '6', '7', '7', '7', '8', '7', '9', '8', '0', '8', '1', '8',
    '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
    '9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9',
    '7', '9', '8', '9', '9'};
struct IEEE754f {
  uint32_t mantissa;
  uint32_t exponent;
  bool sign;
};

struct IEEE754d {
  uint64_t mantissa;
  uint32_t exponent;
  bool sign;
};
IEEE754f decode_ieee754(float f) {
  const uint32_t bits = std::bit_cast<uint32_t>(f);

  IEEE754f decomposed;
  decomposed.exponent =
      (bits >> FloatMantissaBits) & ((1 << FloatExponentBits) - 1);
  decomposed.mantissa = bits & ((1 << FloatMantissaBits) - 1);
  decomposed.sign = bits >> 31;
  return decomposed;
}

IEEE754d decode_ieee754(double f) {
  const uint64_t bits = std::bit_cast<uint64_t>(f);

  IEEE754d decomposed;
  decomposed.exponent =
      (bits >> DoubleMantissaBits) & ((1ull << DoubleExponentBits) - 1);
  decomposed.mantissa = bits & ((1ull << DoubleMantissaBits) - 1);
  decomposed.sign = bits >> 63;
  return decomposed;
}

////////////////////////
// We should use https://en.cppreference.com/w/cpp/numeric/countl_zero
////////////////////////
#if WE_HAVE_VISUAL_STUDIO
inline int leading_zeroes_64(uint64_t input_num) {
  unsigned long index;
#ifdef _WIN64 // highly recommended!!!
  _BitScanReverse64(&index, input_num);
#else  // if we must support 32-bit Windows
  if (input_num > 0xFFFFFFFF) {
    _BitScanReverse(&index, (uint32_t)(input_num >> 32));
    index += 32;
  } else {
    _BitScanReverse(&index, (uint32_t)(input_num));
  }
#endif // _WIN64
  return 63 - index;
}
#else
inline int leading_zeroes_64(uint64_t input_num) {
  return __builtin_clzll(input_num);
}
#endif

inline int int_log2_64(uint64_t x) { return 63 - leading_zeroes_64(x | 1); }

/**
 * Reference:
 * Daniel Lemire, "Computing the number of digits of an integer even faster," in
 * Daniel Lemire's blog, June 3, 2021,
 * https://lemire.me/blog/2021/06/03/computing-the-number-of-digits-of-an-integer-even-faster/.
 */
inline int fast_digit_count32(uint32_t x) {
  static uint64_t table[] = {
      4294967296,  8589934582,  8589934582,  8589934582,  12884901788,
      12884901788, 12884901788, 17179868184, 17179868184, 17179868184,
      21474826480, 21474826480, 21474826480, 21474826480, 25769703776,
      25769703776, 25769703776, 30063771072, 30063771072, 30063771072,
      34349738368, 34349738368, 34349738368, 34349738368, 38554705664,
      38554705664, 38554705664, 41949672960, 41949672960, 41949672960,
      42949672960, 42949672960};
  return uint32_t((x + table[int_log2_64(x)]) >> 32);
}

/**
 * Reference:
 * Daniel Lemire, "Counting the digits of 64-bit integers," in Daniel Lemire's
 * blog, January 7, 2025,
 * https://lemire.me/blog/2025/01/07/counting-the-digits-of-64-bit-integers/.
 */
inline int fast_digit_count64(uint64_t x) {
  static uint64_t table[] = {9,
                             99,
                             999,
                             9999,
                             99999,
                             999999,
                             9999999,
                             99999999,
                             999999999,
                             9999999999,
                             99999999999,
                             999999999999,
                             9999999999999,
                             99999999999999,
                             999999999999999ULL,
                             9999999999999999ULL,
                             99999999999999999ULL,
                             999999999999999999ULL,
                             9999999999999999999ULL};
  int y = (19 * int_log2_64(x) >> 6);
  y += x > table[y];
  return y + 1;
}

// Adapted from the Ryu implementation.
template <typename T>
int to_chars(T mantissa, int32_t exponent, bool sign, char *const result) {
  constexpr bool is_double = sizeof(T) == 8;
  static_assert(is_double || sizeof(T) == 4, "Unsupported type size");

  int index = 0;
  if (sign)
    result[index++] = '-';
  // We use fast arithmetic to compute the number of digits.
  const uint32_t olength =
      is_double ? fast_digit_count64(mantissa) : fast_digit_count32(mantissa);
  // Print the decimal digits.
  // for (uint32_t i = 0; i < olength - 1; ++i) {
  //   const uint32_t c = mantissa % 10; mantissa /= 10;
  //   result[index + olength - i] = (char) ('0' + c);
  // }
  // result[index] = '0' + mantissa % 10;
  //////////////////
  // Performance:
  // On 64-bit systems, 32-bit arithmetic is no faster than 64-bit,
  // and sometimes slower.
  /////////////////
  uint32_t i = 0;
  // We take care of the least significant eight digits first.
  if (mantissa >= 100'000'000) {
    const uint64_t q = mantissa / 100'000'000;
    uint64_t temp = mantissa % 100'000'000;
    mantissa = q;

    const uint64_t c = temp % 10000;
    temp /= 10000;
    const uint64_t d = temp % 10000;
    const uint64_t c0 = (c % 100) << 1;
    const uint64_t c1 = (c / 100) << 1;
    const uint64_t d0 = (d % 100) << 1;
    const uint64_t d1 = (d / 100) << 1;
    memcpy(result + index + olength - 1, hundreds_digit_table + c0, 2);
    memcpy(result + index + olength - 3, hundreds_digit_table + c1, 2);
    memcpy(result + index + olength - 5, hundreds_digit_table + d0, 2);
    memcpy(result + index + olength - 7, hundreds_digit_table + d1, 2);
    i += 8;
  }

  uint64_t output = mantissa;
  // Next, we proceed in block of 4 digits.
  while (output >= 10000) {
    const uint64_t c = output % 10000;
    output /= 10000;
    const uint64_t c0 = (c % 100) << 1;
    const uint64_t c1 = (c / 100) << 1;
    memcpy(result + index + olength - i - 1, hundreds_digit_table + c0, 2);
    memcpy(result + index + olength - i - 3, hundreds_digit_table + c1, 2);
    i += 4;
  }
  // We can take care of two digits out of the 2 or 3 remaining.
  if (output >= 100) {
    const uint64_t c = (output % 100) << 1;
    output /= 100;
    memcpy(result + index + olength - i - 1, hundreds_digit_table + c, 2);
    i += 2;
  }
  // Last digit.
  if (output >= 10) {
    const uint64_t c = output << 1;
    // We can't use memcpy here: the decimal dot goes between these two digits.
    result[index + olength - i] = hundreds_digit_table[c + 1];
    result[index] = hundreds_digit_table[c];
  } else {
    result[index] = (char)('0' + output);
  }

  // Print decimal point if needed.
  if (olength > 1) {
    result[index + 1] = '.';
    index += olength + 1;
  } else {
    ++index;
  }

  // Print the exponent.
  int32_t exp = exponent + (int32_t)olength - 1;
  if (mantissa && exp) { // We do not print the exponent if mantissa is zero.
    result[index++] = 'E';
    if (exp < 0) {
      result[index++] = '-';
      exp = -exp;
    }

    const auto handle_common_cases = [&]() {
      if (exp >= 10) {
        memcpy(result + index, hundreds_digit_table + 2 * exp, 2);
        index += 2;
      } else
        result[index++] = (char)('0' + exp);
    };
    if constexpr (is_double) {
      if (exp >= 100) {
        const int32_t c = exp % 10;
        memcpy(result + index, hundreds_digit_table + 2 * (exp / 10), 2);
        result[index + 2] = (char)('0' + c);
        index += 3;
      } else
        handle_common_cases();
    } else
      handle_common_cases();
  }

  return index;
}

template int to_chars<uint32_t>(uint32_t, int32_t, bool, char *const);
template int to_chars<uint64_t>(uint64_t, int32_t, bool, char *const);
#endif // CHAMPAGNE_LEMIRE_H