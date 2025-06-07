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

// Templated to_centi_digits function, temp should be in [0, 100**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits(uint64_t temp) {
  static_assert(number_centi_digits > 0 && number_centi_digits <= 5,
                "number_digits must be between 1 and 5 inclusive");
  std::array<int, number_centi_digits> result;
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
  constexpr int number_of_most_significant_bits =      number_centi_digits < 4 ? 32 : 8;
  constexpr int number_of_least_significant_bits =
      64 - number_of_most_significant_bits;
  constexpr uint64_t mask =
      UINT64_C(0xFFFFFFFFFFFFFFFF) >> number_of_most_significant_bits;
  constexpr uint64_t constant = ((1ULL << number_of_least_significant_bits) /
                                 int_pow(100, number_centi_digits - 1)) +
                                1;
  uint64_t full = temp * constant; // Scale to get the first digit
  result[0] = full >> number_of_least_significant_bits;

  for (int i : std::ranges::views::iota(1, number_centi_digits)) {
    full = (full & mask) * 100;
    result[i] = full >> number_of_least_significant_bits;
  }

  return result;
}


inline  std::array<char, 2> get_two_digits(uint32_t value) {
  constexpr std::array<std::array<char, 2>, 100> hundreds_digit_table = []() {
    std::array<std::array<char, 2>, 100> table;
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

inline void write_two_digits(char *buffer, uint32_t value) {
  std::memcpy(buffer, get_two_digits(value).data(), 2);
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
  /*if (mantissa >= 1'000'0000'000'000'000) {

    auto low_centi_digits = to_centi_digits<4>(mantissa % int_pow(10, 8));
    mantissa /= int_pow(10, 8);
    write_two_digits(result + index + olength - i - 1, low_centi_digits[3]);
    write_two_digits(result + index + olength - i - 3, low_centi_digits[2]);
    write_two_digits(result + index + olength - i - 5, low_centi_digits[1]);
    write_two_digits(result + index + olength - i - 7, low_centi_digits[0]);
    low_centi_digits = to_centi_digits<4>(mantissa % int_pow(10, 8));
    mantissa /= int_pow(10, 8);
    write_two_digits(result + index + olength - i - 9, low_centi_digits[3]);
    write_two_digits(result + index + olength - i - 11, low_centi_digits[2]);
    write_two_digits(result + index + olength - i - 13, low_centi_digits[1]);
    write_two_digits(result + index + olength - i - 15, low_centi_digits[0]);
  } else*/
  
  if (mantissa >= 10'000'000'000'000'000) {
    const uint64_t q = mantissa / 100'000'000;
    uint64_t temp = mantissa % 100'000'000;
    mantissa = q;
    const uint64_t c = temp % 10000;
    temp /= 10000;
    const uint64_t d = temp % 10000;
    const uint64_t c0 = (c % 100);
    const uint64_t c1 = (c / 100);
    const uint64_t d0 = (d % 100);
    const uint64_t d1 = (d / 100);
    write_two_digits(result + index + olength - i - 1, c0);
    write_two_digits(result + index + olength - i - 3, c1);
    write_two_digits(result + index + olength - i - 5, d0);
    write_two_digits(result + index + olength - i - 7, d1); //162.25 60 !!!
    i += 8;
  }else 
  if (mantissa >= 100'000'000) {
    const uint64_t q = mantissa / 100'000'000;
    uint64_t temp = mantissa % 100'000'000;
    mantissa = q;
    const uint64_t c = temp % 10000;
    temp /= 10000;
    const uint64_t d = temp % 10000;
    const uint64_t c0 = (c % 100);
    const uint64_t c1 = (c / 100);
    const uint64_t d0 = (d % 100);
    const uint64_t d1 = (d / 100);
    write_two_digits(result + index + olength - i - 1, c0);
    write_two_digits(result + index + olength - i - 3, c1);
    write_two_digits(result + index + olength - i - 5, d0);
    write_two_digits(result + index + olength - i - 7, d1);
    i += 8;
  }
  uint64_t output = mantissa;
  // Next, we proceed in block of 4 digits.
  while (output >= 10'000) {
    const uint64_t c = output % 10000;
    output /= 10000;
    const uint64_t c0 = (c % 100);
    const uint64_t c1 = (c / 100);
    write_two_digits(result + index + olength - i - 1, c0);
    write_two_digits(result + index + olength - i - 3, c1);
    i += 4;
  }
  // We can take care of two digits out of the 2 or 3 remaining.
  if (output >= 100) {
    const uint64_t c = (output % 100);
    output /= 100;
    write_two_digits(result + index + olength - i - 1, c);
    i += 2;
  }
  // Last digit.
  if (output >= 10) {
    auto digits = get_two_digits(output);
    result[index + olength - i] = digits[1];
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
    if (exp < 0) {
      result[index++] = 'E';
      result[index++] = '-';
      exp = -exp;
    } else {
      result[index++] = 'E';
    }
    

    if constexpr (is_double) {
      if (exp >= 100) { // 3 digits
        result[index++] = (char)('0' + exp / 100);
        exp %= 100;
        write_two_digits(result + index, exp);
      } else if (exp >= 10) { // 2 digits
        write_two_digits(result + index, exp);
      } else { // 1 digit  
        result[index++] = (char)('0' + exp);
      }
    } else {
      if (exp >= 10) { // 2 digits
        write_two_digits(result + index, exp);
      } else { // 1 digit
        result[index++] = (char)('0' + exp);
      }
    }
  }

  return index;
}

template <typename T>
int fast_to_chars(T mantissa, int32_t exponent, bool sign, char *const result) {
  constexpr bool is_double = sizeof(T) == 8;
  static_assert(is_double || sizeof(T) == 4, "Unsupported type size");
    for(size_t i = 0; i < 64; ++i) {
      result[i] = '_';
    }
  int index = 0;
  if (sign)
    result[index++] = '-';
  // We use fast arithmetic to compute the number of digits.
  const uint32_t number_of_digits = 
      is_double ? fast_digit_count64(mantissa) : fast_digit_count32(mantissa);
  bool need_decimal_point = number_of_digits > 1;
  uint32_t digits_after_decimal = number_of_digits - 1;

  // All digits after the decimal point are written from index + 2 onwards.
  // + 2 because we have the leading digit and the decimal point.
  if (need_decimal_point) {
    result[index + 1] = '.';
  }
  if (digits_after_decimal >= 15) {
    // We have at least 16 digits 
    auto low_centi_digits = to_centi_digits<4>(mantissa % int_pow(10, 8));
    mantissa /= int_pow(10, 8);
    auto high_centi_digits = to_centi_digits<4>(mantissa % int_pow(10, 8));
    if(digits_after_decimal == 16) {
      // We have 16 digits including 15 after the decimal point.
      mantissa /= int_pow(10, 8);
      write_two_digits(result + index + digits_after_decimal - 14, high_centi_digits[0]);
      result[index] = (char)('0' + mantissa);
    } else {
      // We have 16 digits including 15 after the decimal point.
      result[index] = (char)('0' + high_centi_digits[0]/10);
      result[index + 1] = (char)('0' + high_centi_digits[0]%10);
      digits_after_decimal -= 1;
    }
    write_two_digits(result + index + digits_after_decimal - 12, high_centi_digits[1]);
    write_two_digits(result + index + digits_after_decimal - 10, high_centi_digits[2]);
    write_two_digits(result + index + digits_after_decimal - 8, high_centi_digits[3]);
    write_two_digits(result + index + digits_after_decimal - 6, low_centi_digits[0]);
    write_two_digits(result + index + digits_after_decimal - 4, low_centi_digits[1]);
    write_two_digits(result + index + digits_after_decimal - 2, low_centi_digits[2]);
    write_two_digits(result + index + digits_after_decimal, low_centi_digits[3]);
    digits_after_decimal -= 16;
  } else  {
  if (digits_after_decimal >= 8) {
    auto low_centi_digits = to_centi_digits<4>(mantissa % int_pow(10, 8));
    mantissa /= int_pow(10, 8);
    write_two_digits(result + index + digits_after_decimal - 6, low_centi_digits[0]);
    write_two_digits(result + index + digits_after_decimal - 4, low_centi_digits[1]);
    write_two_digits(result + index + digits_after_decimal - 2, low_centi_digits[2]);
    write_two_digits(result + index + digits_after_decimal, low_centi_digits[3]);
    digits_after_decimal -= 8;
  }
  }
//  printf("digits_after_decimal: %d\n", digits_after_decimal);
  /*switch (digits_after_decimal) {
  case 0:
    result[index] = char('0' + mantissa);
    break; // No decimal point needed.
  case 1: {
    auto digits = get_two_digits(mantissa);
    result[index + 2] = digits[1];
    result[index] = digits[0];
    break;
  }
  case 2: {
    write_two_digits(result + index + 2, mantissa % int_pow(10, 2));
    mantissa /= int_pow(10, 2);
    result[index] = char('0' + mantissa);
    break;
  }
  case 3: {
    auto centi_digits = to_centi_digits<2>(mantissa);
    write_two_digits(result + index + 3, centi_digits[1]);
    auto digits = get_two_digits(centi_digits[0]);
    result[index + 2] = digits[1];
    result[index] = digits[0];
    break;
  }
  case 4: {
    auto centi_digits = to_centi_digits<2>(mantissa % int_pow(10, 4));
    mantissa /= int_pow(10, 4);
    write_two_digits(result + index + 2, centi_digits[0]);
    write_two_digits(result + index + 4, centi_digits[1]);

    result[index] = char('0' + mantissa);

    break;
  }
  case 5: {
    auto centi_digits = to_centi_digits<3>(mantissa);
    write_two_digits(result + index + 3, centi_digits[1]);
    write_two_digits(result + index + 5, centi_digits[2]);
    auto digits = get_two_digits(centi_digits[0]);
    result[index + 2] = digits[1];
    result[index] = digits[0];
    break;
  }
  case 6: {
    // Between 100000 and 999999, inclusive, although trailing zeroes are not
    // expected.
    auto centi_digits = to_centi_digits<3>(mantissa % int_pow(10, 6));
    mantissa /= int_pow(10, 6);
    write_two_digits(result + index + 2, centi_digits[0]);
    write_two_digits(result + index + 4, centi_digits[1]);
    write_two_digits(result + index + 6, centi_digits[2]);
    result[index] = char('0' + mantissa);

    break;
  }
  case 7: {
    auto centi_digits = to_centi_digits<4>(mantissa);
    write_two_digits(result + index + 3, centi_digits[1]);
    write_two_digits(result + index + 5, centi_digits[2]);
    write_two_digits(result + index + 7, centi_digits[3]);
    auto digits = get_two_digits(centi_digits[0]);
    result[index + 2] = digits[1];
    result[index] = digits[0];
    break;
  }
  default:
    break; // should never happen, as we only support up to 16 digits after the
           // decimal point.
  }*/
  index += number_of_digits + (need_decimal_point ? 1 : 0);

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
      } else {
        result[index++] = (char)('0' + exp);
      }
    };
    if constexpr (is_double) {
      if (exp >= 100) {
        const int32_t c = exp % 10;
        write_two_digits(result + index, exp / 10);
        result[index + 2] = (char)('0' + c);
        index += 3;
      } else {
        handle_common_cases();
      }
    } else {
      handle_common_cases();
    }
  }

  return index;
}

template int to_chars<uint32_t>(uint32_t, int32_t, bool, char *const);
template int to_chars<uint64_t>(uint64_t, int32_t, bool, char *const);
#endif // CHAMPAGNE_LEMIRE_H