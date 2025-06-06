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



struct IEEE754f {
    uint32_t mantissa;
    uint32_t exponent;
    bool sign;
};

std::array<char,2> get_two_digits(uint32_t value) {
    constexpr std::array<std::array<char,2>, 100> hundreds_digit_table = []() {
      std::array<std::array<char,2>, 100> table;
      for (int i = 0; i < 100; ++i) {
          // Calculate the tens digit
          table[i][0] = (i / 10) + '0';
          // Calculate the units digit
          table[i][1] = (i % 10) + '0';
      }
      return table;
  }(); 
  return hundreds_digit_table[value];
}

void write_two_digits(char *buffer, uint32_t value) {
    std::memcpy(buffer, get_two_digits(value).data(), 2);
}

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
    write_two_digits(result + index + olength - i - 1, c0);
    write_two_digits(result + index + olength - i - 3, c1);
    write_two_digits(result + index + olength - i - 5, d0);
    write_two_digits(result + index + olength - i - 7, d1);
    i += 8;
  }

  uint64_t output = mantissa;
  // Next, we proceed in block of 4 digits.
  while (output >= 10000) {
    const uint64_t c = output % 10000;
    output /= 10000;
    const uint64_t c0 = (c % 100) << 1;
    const uint64_t c1 = (c / 100) << 1;
    write_two_digits(result + index + olength - i - 1, c0);
    write_two_digits(result + index + olength - i - 3, c1);
    i += 4;
  }
  // We can take care of two digits out of the 2 or 3 remaining.
  if (output >= 100) {
    const uint64_t c = (output % 100) << 1;
    output /= 100;
    write_two_digits(result + index + olength - i - 1, c);
    i += 2;
  }
  // Last digit.
  if (output >= 10) {
    auto digits = get_two_digits(output);
    result[index + olength - i] =  digits[1];
    result[index] = digits[0];
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
        write_two_digits(result + index, exp);
        index += 2;
      } else
        result[index++] = (char)('0' + exp);
    };
    if constexpr (is_double) {
      if (exp >= 100) {
        const int32_t c = exp % 10;
        write_two_digits(result + index, exp / 10);
        result[index + 2] = (char)('0' + c);
        index += 3;
      } else
        handle_common_cases();
    } else
      handle_common_cases();
  }

  return index;
}



// Templated to_digits function, temp should be in [0, 10**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_digits>
std::array<int, number_digits> to_digits(uint64_t temp) {
    static_assert(number_digits > 0 && number_digits <= 9,
                "number_digits must be between 1 and 9 inclusive");
  std::array<int, number_digits> result;
  constexpr auto int_pow = [](uint64_t base, uint64_t exponent) -> uint64_t {
    uint64_t result = 1;
    while (exponent > 0) {
      if (exponent & 1)
        result *= base;
      base *= base;
      exponent >>= 1;
    }
    return result;
  };
  if constexpr (number_digits < 6) {
    constexpr uint64_t constant =
        ((1ULL << 32) / int_pow(10, number_digits - 1)) + 1;
    uint64_t full = temp * constant; // Scale to get the first digit
    result[0] = full >> 32;

    for (int i : std::ranges::views::iota(1, number_digits)) {
      full = (full & 0xffffffff) * 10;
      result[i] = full >> 32;
    }
  } else {
    constexpr int number_of_most_significant_bits = 8;
    constexpr int number_of_least_significant_bits =
        64 - number_of_most_significant_bits;
    constexpr uint64_t mask =
        UINT64_C(0xFFFFFFFFFFFFFFFF) >> number_of_most_significant_bits;
    constexpr uint64_t constant = ((1ULL << number_of_least_significant_bits) /
                                   int_pow(10, number_digits - 1)) +
                                  1;
    uint64_t full = temp * constant; // Scale to get the first digit
    result[0] = full >> number_of_least_significant_bits;

    for (int i : std::ranges::views::iota(1, number_digits)) {
      full = (full & mask) * 10;
      result[i] = full >> number_of_least_significant_bits;
    }
  }

  return result;
}
/**
template <typename T>
int fast_to_chars(T mantissa, int32_t exponent, bool sign, char *const result) {
  constexpr bool is_double = sizeof(T) == 8;
  static_assert(is_double || sizeof(T) == 4, "Unsupported type size");

  int index = 0;
  if (sign)
    result[index++] = '-';
  // We use fast arithmetic to compute the number of digits.
  const uint32_t number_of_digits =
      is_double ? fast_digit_count64(mantissa) : fast_digit_count32(mantissa);
  bool need_decimal_point = number_of_digits > 1;
  const uint32_t digits_after_decimal = number_of_digits - 1;
  // All digits after the decimal point are written from index + 2 onwards.
  // + 2 because we have the leading digit and the decimal point.
  
  switch (digits_after_decimal) {
    case 0:
      break; // No decimal point needed.
    case 1:
      result[index + 2] = (char)('0' + mantissa);
      break;
    case 2:
      write_two_digits(result + index + 2, mantissa);
      break;
    case 3: {
      auto digits = to_digits<3>(mantissa);
      result[index++] = digits[0];
      result[index++] = digits[1];
      return index;
    }
    default:
      break; // Continue to handle larger lengths.
  }
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
    write_two_digits(result + index + number_of_digits - i - 1, c0);
    write_two_digits(result + index + number_of_digits - i - 3, c1);
    write_two_digits(result + index + number_of_digits - i - 5, d0);
    write_two_digits(result + index + number_of_digits - i - 7, d1);
    i += 8;
  }

  uint64_t output = mantissa;
  // Next, we proceed in block of 4 digits.
  while (output >= 10000) {
    const uint64_t c = output % 10000;
    output /= 10000;
    const uint64_t c0 = (c % 100) << 1;
    const uint64_t c1 = (c / 100) << 1;
    write_two_digits(result + index + number_of_digits - i - 1, c0);
    write_two_digits(result + index + number_of_digits - i - 3, c1);
    i += 4;
  }
  // We can take care of two digits out of the 2 or 3 remaining.
  if (output >= 100) {
    const uint64_t c = (output % 100) << 1;
    output /= 100;
    write_two_digits(result + index + number_of_digits - i - 1, c);
    i += 2;
  }
  // Last digit.
  if (output >= 10) {
    auto digits = get_two_digits(output);
    result[index + number_of_digits - i] =  digits[1];
    result[index] = digits[0];
  } else {
    result[index] = (char)('0' + output);
  }

  // Print decimal point if needed.
  if (number_of_digits > 1) {
    result[index + 1] = '.';
    index += number_of_digits + 1;
  } else {
    ++index;
  }

  // Print the exponent.
  int32_t exp = exponent + (int32_t)number_of_digits - 1;
  if (mantissa && exp) { // We do not print the exponent if mantissa is zero.
    result[index++] = 'E';
    if (exp < 0) {
      result[index++] = '-';
      exp = -exp;
    }

    const auto handle_common_cases = [&]() {
      if (exp >= 10) {
        write_two_digits(result + index, exp);
        index += 2;
      } else
        result[index++] = (char)('0' + exp);
    };
    if constexpr (is_double) {
      if (exp >= 100) {
        const int32_t c = exp % 10;
        write_two_digits(result + index, exp / 10);
        result[index + 2] = (char)('0' + c);
        index += 3;
      } else
        handle_common_cases();
    } else
      handle_common_cases();
  }

  return index;
}*/


template int to_chars<uint32_t>(uint32_t, int32_t, bool, char *const);
template int to_chars<uint64_t>(uint64_t, int32_t, bool, char *const);
#endif // CHAMPAGNE_LEMIRE_H