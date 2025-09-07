#ifndef SSSE3_H
#define SSSE3_H

#ifdef __SSSE3__

#define HAVE16

#include <cstdint>
#include <cstring>
#include <x86intrin.h>

#include "scalar.h"


// Precomputed shuffle masks for K = 1 to 16
static const uint8_t shuffle_masks[16][16] = {
    {0x80, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 0x80, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 0x80, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 0x80, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 0x80, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 0x80, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 0x80, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 0x80, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 0x80, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 0x80, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x80, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0x80, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x80, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0x80, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0x80, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0x80}};
/**
 * Insert a line feed character in the 64-byte input at index K in [0,16).
 */
inline __m128i insert_line_feed16(__m128i input, int K) {
  // Prepare a vector with '\n' (0x0A)
  __m128i line_feed_vector = _mm_set1_epi8('\n');

  // Load the precomputed shuffle mask for K (index K-1)
  __m128i mask = _mm_loadu_si128((__m128i *)shuffle_masks[K]);
  __m128i lf_pos = _mm_cmpeq_epi8(mask, _mm_set1_epi8(0x80));

  // Perform the shuffle to reposition the K bytes
  __m128i shuffled = _mm_shuffle_epi8(input, mask);

  // Blend with line_feed_vector to insert '\n' at the appropriate positions
  __m128i result = _mm_blendv_epi8(shuffled, line_feed_vector, lf_pos);

  return result;
}

/**
 * Insert line feeds every K bytes the input data (buffer, length).
 * The output is written to the output buffer (which must be large enough).
 */
inline void insert_line_feed16(const char *buffer, size_t length, int K,
                        char *output) {
  if (K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  size_t next_line_feed = K;
  if (K >= 15) {
    while (input_pos + 15 <= length) {
      __m128i chunk = _mm_loadu_si128((__m128i *)(buffer + input_pos));
      if (next_line_feed >= 16) {
        _mm_storeu_si128((__m128i *)(output), chunk);
        output += 16;
        next_line_feed -= 16;
        input_pos += 16;
      } else {
        // we write next_line_feed bytes, then a line feed, then the rest (31 -
        // next_line_feed bytes)
        chunk = insert_line_feed16(chunk, next_line_feed);
        _mm_storeu_si128((__m128i *)(output), chunk);
        output += 16;
        next_line_feed = K - (15 - next_line_feed);
        input_pos += 15;
      }
    }
  }
  while (input_pos < length) {
    output[0] = buffer[input_pos];
    output++;
    input_pos++;
    next_line_feed--;
    if (next_line_feed == 0) {
      output[0] = '\n';
      output++;
      next_line_feed = K;
    }
  }
}

#endif // __SSSE3__

#endif // SSSE3_H