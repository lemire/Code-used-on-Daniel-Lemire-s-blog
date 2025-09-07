#include "scalar.h"




void insert_line_feed_memcpy_offline(const char *buffer, size_t length, int K,
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
