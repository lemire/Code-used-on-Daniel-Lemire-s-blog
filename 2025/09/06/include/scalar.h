#ifndef SCALAR_H
#define SCALAR_H

#include <cstdint>
#include <cstring>

/**
 * Insert line feeds every K bytes the input data (buffer, length).
 * The output is written to the output buffer (which must be large enough).
 */
inline void insert_line_feed(const char *buffer, size_t length, int K, char *output) {
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

inline void insert_line_feed_memcpy(const char *buffer, size_t length, int K,
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

void insert_line_feed_memcpy_offline(const char *buffer, size_t length, int K,
                             char *output);

#endif // SCALAR_H