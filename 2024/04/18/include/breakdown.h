
#include <memory.h>

inline void break_lines(char *out, const char *in, size_t length,
                   size_t line_length) {
  size_t j = 0;
  size_t i = 0;
  for (; i + line_length <= length; i += line_length) {
    std::memcpy(out + j, in + i, line_length);
    out[j+line_length] = '\n';
    j += line_length + 1;
  }
  if (i < length) {
    std::memcpy(out + j, in + i, length - i);
  }
}

__attribute__ ((noinline)) 
void break_lines_no_inline(char *out, const char *in, size_t length,
                   size_t line_length) {
  size_t j = 0;
  size_t i = 0;
  for (; i + line_length <= length; i += line_length) {
    std::memcpy(out + j, in + i, line_length);
    out[j+line_length] = '\n';
    j += line_length + 1;
  }
  if (i < length) {
    std::memcpy(out + j, in + i, length - i);
  }
}


template <size_t line_length>
inline void break_lines_template(char *out, const char *in, size_t length) {
  size_t j = 0;
  size_t i = 0;
  // make it a multiple of 16:
  size_t line_length_16 = (line_length + 15) / 16 * 16;
  for (; i + line_length_16 <= length; i += line_length) {
    std::memcpy(out + j, in + i, line_length_16);
    out[j+line_length] = '\n';
    j += line_length + 1;
  }
  for (; i + line_length <= length; i += line_length) {
    std::memcpy(out + j, in + i, line_length);
    out[j+line_length] = '\n';
    j += line_length + 1;
  }
  if (i < length) {
    std::memcpy(out + j, in + i, length - i);
  }
}


inline void break_lines_inplace(char *in, size_t length, size_t line_length) {
  size_t left = length % line_length;
  size_t i = length - left;
  size_t j = length + length / line_length - left;
  std::memmove(in + j, in + i, left);
  while (i >= line_length) {
    i -= line_length;
    j -= line_length + 1;
    std::memmove(in + j, in + i, line_length);
    in[j+line_length] = '\n';
  }
}

__attribute__ ((noinline)) 
void break_lines_inplace_no_inline(char *in, size_t length, size_t line_length) {
  size_t left = length % line_length;
  size_t i = length - left;
  size_t j = length + length / line_length - left;
  std::memmove(in + j, in + i, left);
  while (i >= line_length) {
    i -= line_length;
    j -= line_length + 1;
    std::memmove(in + j, in + i, line_length);
    in[j+line_length] = '\n';
  }
}

