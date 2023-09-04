
#include "identifiers.h"
#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const uint8_t identifier_map[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // 0x
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // 1x
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // 2x
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0,   0,   0,   0,   0,   0, // 3x
    0,   255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, // 4x
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 0,   0,   0,   0,   255, // 5x
    0,   255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, // 6x
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 0,   0,   0,   0,   0, // 7x
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // 8x
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // 9x
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Ax
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Bx
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Cx
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Dx
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Ex
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, // Fx
};


/**
 * Returns a pointer to the first byte following the end of the identifier that
 * started just before the given source pointer.
 */
static const char *parse_identifier(const char *source, const char *end) {
  while (source < end && identifier_map[(unsigned)*source& 0xff]) {
    source++;
  }
  return source;
}

/**
 * Count the number of identifiers in the given source string and print out the
 * count to stdout.
 */
size_t count_identifiers(const char *source, size_t size) {
  size_t count = 0;
  const char *end = source + size;

  while (source < end) {
    uint8_t c = identifier_map[(unsigned)*source++& 0xff];
    if(c) {
      count += (c == 255);
      source = parse_identifier(source, end);
    }
  }
  return count;
}

static uint16_t to_bitmask_neon(uint8x16_t v) {
  const uint8x16_t bit_mask =
      (uint8x16_t){0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
                   0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
  uint8x16_t minput = vandq_u8(v, bit_mask);
  uint8x16_t tmp = vpaddq_u8(minput, minput);
  tmp = vpaddq_u8(tmp, tmp);
  tmp = vpaddq_u8(tmp, tmp);
  return vgetq_lane_u16(vreinterpretq_u16_u8(tmp), 0);
}

struct identifier_neon_state {
  uint16_t identifier_mask;
  uint16_t leading_identifier_mask;
};

// Parsing Gigabytes of JSON per Second, The VLDB Journal, 28(6), 2019
// https://arxiv.org/abs/1902.08318
static struct identifier_neon_state identifier_neon(const char *source) {
  const uint8x16_t low_nibble_mask = (uint8x16_t){
      42, 62, 62, 62, 62, 62, 62, 62, 62, 62, 60, 20, 20, 20, 20, 21};
  const uint8x16_t high_nibble_mask =
      (uint8x16_t){0, 0, 0, 2, 16, 33, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8x16_t chars = vld1q_u8((const uint8_t *)source);
  uint8x16_t low =
      vqtbl1q_u8(low_nibble_mask, vandq_u8(chars, vdupq_n_u8(0xf)));
  uint8x16_t high = vqtbl1q_u8(high_nibble_mask, vshrq_n_u8(chars, 4));
  uint8x16_t v = vtstq_u8(low, high);
  uint8x16_t v_no_number = vtstq_u8(low, vandq_u8(high, vdupq_n_u8(61)));
  return (struct identifier_neon_state){to_bitmask_neon(v),
                                        to_bitmask_neon(v_no_number)};
}

size_t count_identifiers_neon(const char *source, size_t size) {
  size_t count = 0;
  const char *end = source + size;
  if (size >= 16) {
    const char *end16 = source + size - 16;
    uint16_t lastbit = 0;
    while (source <= end16) {
      struct identifier_neon_state m = identifier_neon(source);
      uint16_t mask =
          m.leading_identifier_mask & ~(m.identifier_mask << 1 | lastbit);

      count += (size_t)__builtin_popcount(mask);
      lastbit = m.identifier_mask >> 15;
      source += 16;
    }
    if (lastbit) {
      source = parse_identifier(source, end);
    }
  }


  while (source < end) {
    uint8_t c = identifier_map[(unsigned)*source++& 0xff];
    if(c) {
      count += (c == 255);
      source = parse_identifier(source, end);
    }
  }

  return count;
}

struct identifier_neon_state_strager {
  uint32_t identifier_doublemask;
  uint32_t leading_identifier_doublemask;
};

// Explanation:
// https://github.com/quick-lint/quick-lint-js/blob/08ee4ea76bca1cc67e37e0b535e35ec76a4d0ce9/src/quick-lint-js/port/simd-neon-arm.h#L21
//
// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew "strager" Glazar
//
// This file is part of quick-lint-js.
//
// quick-lint-js is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// quick-lint-js is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with quick-lint-js.  If not, see <https://www.gnu.org/licenses/>.
//
// ---
//
// Portions of this file are
// Copyright (c) 2014-2020, Arm Limited.
// Source:
// https://github.com/ARM-software/optimized-routines/blob/7a9fd1603e1179b044406fb9b6cc5770d736cde7/string/aarch64/memchr.S
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
static uint32_t to_bitdoublemask_neon_strager(uint8x16_t v) {
  uint8x16_t magic = {
    0x01, 0x04, 0x10, 0x40, 0x01, 0x04, 0x10, 0x40,
    0x01, 0x04, 0x10, 0x40, 0x01, 0x04, 0x10, 0x40,
  };
  uint8x16_t garbage = v;  // arbitrary
  uint8x16_t mixed_0 = vandq_u8(magic, v);
  uint8x16_t mixed_1 = vpaddq_u8(mixed_0, garbage);
  uint8x16_t mixed_2 = vpaddq_u8(mixed_1, mixed_1);
  return vgetq_lane_u32(vreinterpretq_u32_u8(mixed_2), 0);
}
// End MIT+GPL-licenced code.

// Identical to identifier_neon but using to_bitdoublemask_neon_strager.
static struct identifier_neon_state_strager identifier_neon_strager(const char *source) {
  const uint8x16_t low_nibble_mask = (uint8x16_t){
      42, 62, 62, 62, 62, 62, 62, 62, 62, 62, 60, 20, 20, 20, 20, 21};
  const uint8x16_t high_nibble_mask =
      (uint8x16_t){0, 0, 0, 2, 16, 33, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8x16_t chars = vld1q_u8((const uint8_t *)source);
  uint8x16_t low =
      vqtbl1q_u8(low_nibble_mask, vandq_u8(chars, vdupq_n_u8(0xf)));
  uint8x16_t high = vqtbl1q_u8(high_nibble_mask, vshrq_n_u8(chars, 4));
  uint8x16_t v = vtstq_u8(low, high);
  uint8x16_t v_no_number = vtstq_u8(low, vandq_u8(high, vdupq_n_u8(61)));
  return (struct identifier_neon_state_strager){to_bitdoublemask_neon_strager(v),
                                                to_bitdoublemask_neon_strager(v_no_number)};
}

size_t count_identifiers_neon_strager(const char *source, size_t size) {
  size_t count = 0;
  const char *end = source + size;
  if (size >= 16) {
    const char *end16 = source + size - 16;
    uint32_t lastbit = 0;
    while (source <= end16) {
      struct identifier_neon_state_strager m = identifier_neon_strager(source);
      uint32_t mask =
          m.leading_identifier_doublemask & ~(m.identifier_doublemask << 2 | lastbit);

      count += (size_t)__builtin_popcount(mask);
      lastbit = m.identifier_doublemask >> 30;
      source += 16;
    }
    if (lastbit) {
      source = parse_identifier(source, end);
    }
  }


  while (source < end) {
    uint8_t c = identifier_map[(unsigned)*source++& 0xff];
    if(c) {
      count += (c == 255);
      source = parse_identifier(source, end);
    }
  }

  return count;
}

static struct identifier_neon_state_strager identifier_neon_strager_ranges(const char *source) {
  uint8x16_t chars = vld1q_u8((const uint8_t *)source);

  uint8x16_t chars_lower = vorrq_u8(chars, vdupq_n_u8('a' - 'A'));
  uint8x16_t is_alpha = vandq_u8(vcleq_u8(vdupq_n_u8('a'), chars_lower), vcleq_u8(chars_lower, vdupq_n_u8('z')));
  uint8x16_t is_symbol = vceqq_u8(chars, vdupq_n_u8('_'));
  uint8x16_t is_digit = vandq_u8(vcleq_u8(vdupq_n_u8('0'), chars), vcleq_u8(chars, vdupq_n_u8('9')));
  uint8x16_t is_ident_start = vorrq_u8(is_alpha, is_symbol);

  uint32_t is_digit_doublemask = to_bitdoublemask_neon_strager(is_digit);
  uint32_t is_ident_start_doublemask = to_bitdoublemask_neon_strager(is_ident_start);
  return (struct identifier_neon_state_strager){
    is_ident_start_doublemask | is_digit_doublemask,
    is_ident_start_doublemask,
  };
}

size_t count_identifiers_neon_strager_ranges(const char *source, size_t size) {
  size_t count = 0;
  const char *end = source + size;
  if (size >= 16) {
    const char *end16 = source + size - 16;
    uint32_t lastbit = 0;
    while (source <= end16) {
      struct identifier_neon_state_strager m = identifier_neon_strager_ranges(source);
      uint32_t mask =
          m.leading_identifier_doublemask & ~(m.identifier_doublemask << 2 | lastbit);

      count += (size_t)__builtin_popcount(mask);
      lastbit = m.identifier_doublemask >> 30;
      source += 16;
    }
    if (lastbit) {
      source = parse_identifier(source, end);
    }
  }


  while (source < end) {
    uint8_t c = identifier_map[(unsigned)*source++& 0xff];
    if(c) {
      count += (c == 255);
      source = parse_identifier(source, end);
    }
  }

  return count;
}


/****/


 
static uint64_t to_bitmask_neon_blob(uint8x16_t v0, uint8x16_t v1, uint8x16_t v2, uint8x16_t v3) {
  uint8x16_t t0 = vbslq_u8(vdupq_n_u8(0x55), v0, v1); // 01010101...
  uint8x16_t t1 = vbslq_u8(vdupq_n_u8(0x55), v2, v3); // 23232323...
  uint8x16_t combined = vbslq_u8(vdupq_n_u8(0x33), t0, t1); // 01230123...
  uint8x8_t sum = vshrn_n_u16(vreinterpretq_u16_u8(combined), 4);
  return vget_lane_u64(vreinterpret_u64_u8(sum), 0);
}
 
struct identifier_neon_state_blob {
  uint8x16_t identifiers;
  uint8x16_t numbers;
};
 
// Parsing Gigabytes of JSON per Second, The VLDB Journal, 28(6), 2019
// https://arxiv.org/abs/1902.08318
static struct identifier_neon_state_blob identifier_neon_blob(uint8x16_t chars) {
  const uint8x16x2_t thresh_table = (uint8x16x2_t){
    (uint8x16_t){127, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64},
    (uint8x16_t){'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 127, 127, 127, 127, 'A'}
  };
  
  // lookup threshold values to compare against
  uint8x16_t lookup = vandq_u8(chars, vdupq_n_u8(96 ^ 255));
  uint8x16_t thresh = vqtbl2q_u8(thresh_table, lookup);
  
  // compare equal for numbers, compare greater-equal for identifiers (but need to match underscore without matching against character 127, so implement this compare by causing 127 to wrap around)
  uint8x16_t v = vcgtq_s8(vreinterpretq_s8_u8(vaddq_u8(chars, vdupq_n_u8(1))), vreinterpretq_s8_u8(thresh));
  uint8x16_t v_num = vceqq_u8(chars, thresh);
  return (struct identifier_neon_state_blob){v, v_num};
}
 
size_t count_identifiers_neon_blob(const char *source, size_t size) {
  size_t count = 0;
  const char *end = source + size;
  if (size >= 64) {
    const char *end64 = source + size - 64;
    uint64_t lastbit = 0;
    while (source <= end64) {
      uint8x16x4_t chars = vld4q_u8((const uint8_t *)source);
      struct identifier_neon_state_blob m0 = identifier_neon_blob(chars.val[0]);
      struct identifier_neon_state_blob m1 = identifier_neon_blob(chars.val[1]);
      struct identifier_neon_state_blob m2 = identifier_neon_blob(chars.val[2]);
      struct identifier_neon_state_blob m3 = identifier_neon_blob(chars.val[3]);
      
      uint64_t number_mask = to_bitmask_neon_blob(m0.numbers, m1.numbers, m2.numbers, m3.numbers);
      uint64_t identifier_mask = to_bitmask_neon_blob(m0.identifiers, m1.identifiers, m2.identifiers, m3.identifiers);
      
      uint64_t mask =
          identifier_mask & ~number_mask & ~(identifier_mask << 1 | lastbit);
 
      count += (size_t)__builtin_popcountll(mask);
      lastbit = identifier_mask >> 63;
      source += 64;
    }
    if (lastbit) {
      source = parse_identifier(source, end);
    }
  }
 
 
  while (source < end) {
    uint8_t c = identifier_map[(unsigned)*source++& 0xff];
    if(c) {
      count += (c == 255);
      source = parse_identifier(source, end);
    }
  }
 
  return count;
}