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
// Lemire, with optimizations by Jean-Marc Bourguet
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

// Optimized differently by David Fetter
int parse_uint8_fastswar_david(const char *str, size_t len, uint8_t *num) {
	/* Pre-compute the needed values of (4-len)*8. Position 0 is not used, but
	 * is there to avoid more computation and off-by-one funnies */
	static int the_shift[4] = {0, 24, 16, 8};
    union {
        uint8_t as_str[4];
        uint32_t as_int;
    } digits;
	if (len == 0 || len > 3) {
		return 0;
    }

	memcpy(&digits, str, sizeof(digits));

	/* Flip 0x30, detect non-digits */
	digits.as_int ^= 0x30303030lu;

	/* Shift off trash bytes. */
	digits.as_int <<= the_shift[len];

	uint32_t all_digits = ((digits.as_int | (0x06060606 + digits.as_int)) & 0xf0f0f0f0) == 0;
	*num = (uint8_t)((0x640a01 * digits.as_int) >> 24);
	return all_digits & ((__builtin_bswap32(digits.as_int) <= 0x020505));
}

#include <string>
#include <vector>

std::vector<uint16_t> lut_vector{};

inline uint32_t make_lut_index_masked(const char *str, size_t len) {
  if (len > 3)
    return 0;
  uint32_t mask = 0xffffffff >> 8 * (4 - len);
  uint32_t out = *reinterpret_cast<const uint32_t *>(str);
  out &= mask;
  return out;
}

void make_lut() {
  lut_vector.resize(256 * 256 * 256);
  std::fill(lut_vector.begin(), lut_vector.end(), 4 << 8);
  for (size_t val = 0; val <= 255; val++) {
    std::string s = std::to_string(val);
    uint32_t key = make_lut_index_masked(s.data(), s.size());
    lut_vector[key] = (uint16_t)(val | (s.size() << 8));
    // We also allow zero padding:
    while (s.size() < 3) {
      s = '0' + s;
      uint32_t key = make_lut_index_masked(s.data(), s.size());
      lut_vector[key] = (uint16_t)(val | (s.size() << 8));
    }
  }
}

int parse_uint8_lutold(const char *str, size_t len, uint8_t *num) {
  if (len > 3)
    return 0;
  uint32_t key;
  memcpy(&key, str, 4);
  uint32_t mask = 0xffffffff >> 8 * (4 - len);
  key &= mask;
  uint16_t result = lut_vector[key];
  *num = static_cast<uint8_t>(result);
  return (result >> 8) == len;
}

// credit: Nick Powell, modified by D. Lemire
int parse_uint8_lut(const char *str, size_t len, uint8_t *num) {
  if (len > 3)
    return 0;
  uint32_t key;
  memcpy(&key, str, 4);
  uint32_t mask = 0xffffffff >> (8 * (4 - len));
  key &= mask;
  uint16_t val = lut_vector[key];
  *num = uint8_t(val);
  return (val >> 8) == len;
}

int parse_uint8_switch_case(const char *str, size_t len, uint8_t *num) {
  uint8_t hi, mid, lo;
  #define as_u8(x) ((uint8_t)((x) - '0'))
  switch(len) {
    case 1:
    *num = as_u8(str[0]);
    return *num < 10;
    case 2:
    hi = as_u8(str[0]);
    lo = as_u8(str[1]);
    *num = hi * 10 + lo;
    return (hi < 10) && (lo < 10);
    case 3:
    hi = as_u8(str[0]);
    mid = as_u8(str[1]);
    lo = as_u8(str[2]);
    *num = hi * 100 + mid * 10 + lo;
    return (hi < 10) && (mid < 10) && (lo < 10);
    default:
    return 0;
  }
  #undef as_u8
}

void parse_uint8_nocheck(const char *str, size_t len, uint8_t *num) {
  const uint32_t shr = ((len << 3) - 8) & 0x18;
  uint32_t dgts;

  memcpy(&dgts, str, sizeof(dgts));
  dgts &= 0x000f0f0flu;
  *num = (uint8_t)((0x640a01 * dgts) >> shr);
}