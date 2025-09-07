#ifndef AVX512VBMI2_H
#define AVX512VBMI2_H

#ifdef __AVX512VBMI2__
#define HAVE64

#include <cstdint>
#include <cstring>
#include <x86intrin.h>

#include "scalar.h"
/**
 * Insert a line feed character in the 64-byte input at index K in [0,64).
 */
/*__m512i insert_line_feed64(__m512i input, int K) {
    // Prepare a vector with '\n' (0x0A)
    __m512i line_feed_vector = _mm512_set1_epi8('\n');
    return _mm512_mask_expand_epi8(line_feed_vector, ~uint64_t(1ULL<<K), input);
}*/

/**
 * Insert a line feed character in the 64-byte input at index K in [0,32).
 */
__m256i insert_line_feed64_32(__m256i input, int K) {
  // Prepare a vector with '\n' (0x0A)
  __m256i line_feed_vector = _mm256_set1_epi8('\n');
  return _mm256_mask_expand_epi8(line_feed_vector, ~uint32_t(1ULL << K), input);
}

/**
 * Insert line feeds every K bytes the input data (buffer, length).
 * The output is written to the output buffer (which must be large enough).
 */
void insert_line_feed64(const char *buffer, size_t length, int K,
                        char *output) {
  if (K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  size_t next_line_feed = K;
  if (K >= 31) {
    while (input_pos + 32 <= length) {
      __m256i chunk = _mm256_loadu_si256((__m256i *)(buffer + input_pos));
      if (next_line_feed >= 32) {
        _mm256_storeu_si256((__m256i *)(output), chunk);
        output += 32;
        next_line_feed -= 32;
        input_pos += 32;
      } else {
        chunk = insert_line_feed64_32(chunk, next_line_feed);
        _mm256_storeu_si256((__m256i *)(output), chunk);
        output += 32;
        next_line_feed = K - (31 - next_line_feed);
        input_pos += 31;
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
#endif // __AVX512VBMI2__

#endif // AVX512VBMI2_H