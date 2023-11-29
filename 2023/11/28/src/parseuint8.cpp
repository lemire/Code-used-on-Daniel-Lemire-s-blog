#include "parseuint8.h"

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

// credit: Jeroen Koekkoek, does not pass tests.
int parse_uint8_swar(const char *str, size_t len, uint8_t *num) {
  union {
    uint8_t as_str[4];
    uint32_t as_int;
  } digits;

  memcpy(&digits.as_int, str, sizeof(digits));
  // flip 0x30, detect non-digits
  digits.as_int ^= 0x30303030lu;
  // shift off trash bytes
  digits.as_int <<= (4 - (len & 0x3)) * 8;

  const uint32_t x = digits.as_str[1] * 10 + digits.as_str[2];
  const uint32_t y = x * 10 + digits.as_str[3];
  *num = (uint8_t)y;
  return (digits.as_int & 0xf0f0f0f0) == 0 && y < 256 && len != 0 && len < 4;
}
// based on parse_uint8_swar by Jeroen Koekkoek, fixed and optimized by Daniel
// Lemire
int parse_uint8_fastswar(const char *str, size_t len, uint8_t *num) {
  if (len == 0 || len > 3) {
    return 0;
  }
  union {
    uint8_t as_str[4];
    uint32_t as_int;
  } digits;

  memcpy(&digits.as_int, str, sizeof(digits));
  // flip 0x30, detect non-digits
  digits.as_int ^= 0x30303030lu;
  // shift off trash bytes, technically undefined behaviour when ((4 - len) * 8)
  // is not in [0,32) prior to C17 / C++14.
  digits.as_int <<= ((4 - len) * 8);
  // check all digits
  uint32_t all_digits =
      ((digits.as_int | (0x06060606 + digits.as_int)) & 0xF0F0F0F0) == 0;
  *num = (uint8_t)((0x640a01 * digits.as_int) >> 24);
  return all_digits & ((__builtin_bswap32(digits.as_int) <= 0x020505));
}

int parse_uint8_fromchars(const char *str, size_t len, uint8_t *num) {
  auto [p, ec] = std::from_chars(str, str + len, *num);
  return (ec == std::errc());
}

// credit: Jeroen Koekkoek
int parse_uint8_naive(const char *str, size_t len, uint8_t *num) {
  uint32_t n = 0;

  for (size_t i = 0, r = len & 0x3; i < r; i++) {
    uint8_t d = (uint8_t)(str[i] - '0');
    if (d > 9)
      return 0;
    n = n * 10 + d;
  }

  *num = (uint8_t)n;
  return n < 256 && len && len < 4;
}
// based on parse_uint8_fastswar optimized by Perforated Bob
int parse_uint8_fastswar_bob(const char *str, size_t len, uint8_t *num) {
  union {
    uint8_t as_str[4];
    uint32_t as_int;
  } digits;

  memcpy(&digits.as_int, str, sizeof(digits));
  digits.as_int ^= 0x303030lu;
  digits.as_int <<= (len ^ 3) * 8;
  *num = (uint8_t)((0x640a01 * digits.as_int) >> 16);
  return ((((digits.as_int + 0x767676) | digits.as_int) & 0x808080) == 0) &&
         ((len ^ 3) < 3) && __builtin_bswap32(digits.as_int) <= 0x020505ff;
}
