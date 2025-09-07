#ifndef SCALAR_H
#define SCALAR_H

#include <cstdint>
#include <cstring>

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
 * Insert line feeds every K bytes the input data (buffer, length).
 * The output is written to the output buffer (which must be large enough).
 */
void insert_line_feed(const char *buffer, size_t length, int K, char *output) {
  if (K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  size_t next_line_feed = K;
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

void insert_line_feed_memcpy(const char *buffer, size_t length, int K,
                             char *output) {
  if (K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  while (input_pos + K < length) {
    std::memcpy(output, buffer + input_pos, K);
    output += K;
    input_pos += K;
    output[0] = '\n';
    output++;
  }
  std::memcpy(output, buffer + input_pos, length - input_pos);
}

#endif // SCALAR_H