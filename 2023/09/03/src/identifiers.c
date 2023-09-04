
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
/*
print8(const char *source) {
  for(size_t i = 0; i < 16; i++) {
    printf("%c", identifier_map[(unsigned)source[i]& 0xff]?(leading_identifier_map[(unsigned)source[i]&0xff]?'L':'I'):'_');

  }
  printf("\n");
for(size_t i = 0; i < 16; i++) {
    printf("%02x ", (unsigned)source[i]&0xff);
  }
  printf("\n");
for(size_t i = 0; i < 16; i++) {
    printf("%c ", ((unsigned)source[i]<128 && (unsigned)source[i]>=' ')?source[i]:'_');
  }
  printf("\n");
}*/

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
