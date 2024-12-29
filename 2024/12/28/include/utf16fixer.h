#ifndef UTF16_FIXER_H
#define UTF16_FIXER_H
#include <arm_neon.h>
#include <cstdlib>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool is_high_surrogate(char16_t c) { return (c >= 0xD800 && c <= 0xDBFF); }

bool is_low_surrogate(char16_t c) { return (c >= 0xDC00 && c <= 0xDFFF); }

void replace_invalid_utf16(char16_t *buffer, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    if (is_high_surrogate(buffer[i])) {
      if (i + 1 < length && is_low_surrogate(buffer[i + 1])) {
        i++;
      } else {
        buffer[i] = 0xFFFD; // Replacement character
      }
    } else if (is_low_surrogate(buffer[i])) {
      buffer[i] = 0xFFFD; // Replacement character
    }
  }
}

void replace_invalid_utf16_neon(char16_t *buffer, size_t length) {
  const size_t vec_size = 8;
  size_t i = 0;
  if (length >= vec_size) {
    uint16x8_t replacement = vdupq_n_u16(0xFFFD);
    uint16x8_t previous_high_surrogate_mask = vdupq_n_u16(0);
    for (; i + vec_size <= length; i += vec_size) {
      uint16x8_t vec = vld1q_u16((const uint16_t *)buffer + i);

      uint16x8_t low_surrogate_mask =
          vcleq_u16(vaddq_u16(vec, vdupq_n_u16(0x2400)), vdupq_n_u16(0x03ff));

      uint16x8_t high_surrogate_mask =
          vcleq_u16(vaddq_u16(vec, vdupq_n_u16(0x2800)), vdupq_n_u16(0x03ff));
      uint16x8_t offset_high_surrogate_mask =
          vextq_u16(previous_high_surrogate_mask, high_surrogate_mask, 7);
      uint16x8_t offset_low_surrogate_mask =
          (i + vec_size < length && is_low_surrogate(buffer[i + vec_size]))
              ? vextq_u16(low_surrogate_mask, vdupq_n_u16(0xFFFF), 1)
              : vextq_u16(low_surrogate_mask, vdupq_n_u16(0), 1);

      uint16x8_t low_not_preceded_by_high =
          vbicq_u16(low_surrogate_mask, offset_high_surrogate_mask);

      uint16x8_t high_not_followed_by_low =
          vbicq_u16(high_surrogate_mask, offset_low_surrogate_mask);

      uint16x8_t invalid_pair_mask =
          vorrq_u16(low_not_preceded_by_high, high_not_followed_by_low);

      uint16x8_t result = vbslq_u16(invalid_pair_mask, replacement, vec);
      vst1q_u16((uint16_t *)buffer + i, result);
      previous_high_surrogate_mask = high_surrogate_mask;
    }
  }

  // Handle remaining elements or small buffers
  for (; i < length; ++i) {
    if (is_high_surrogate(buffer[i])) {
      if (i + 1 < length && is_low_surrogate(buffer[i + 1])) {
        i++;
      } else {
        buffer[i] = 0xFFFD; // Replacement character
      }
    } else if (is_low_surrogate(buffer[i])) {
      buffer[i] = 0xFFFD; // Replacement character
    }
  }
}

#endif