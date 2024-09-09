#ifndef PRECOMP_H
#define PRECOMP_H

#include <array>
#include <cstddef>
#include <cstdint>

std::vector<std::string> precompute_string() {
  size_t size = 1 << 16;
  std::vector<std::string> code_table(size);
  for (size_t i = 0; i < size; ++i) {
    code_table[i] = std::to_string(i) + ',';
  }
  return code_table;
}

std::pair<std::array<char, 382106>, std::array<uint32_t, 65537>>
precompute_string_fast() {
  std::array<char, 382106> str;
  std::array<uint32_t, 65537> off;
  off[0] = 0;
  char *p = &str[0];
  constexpr auto const_int_to_str = [](uint16_t value, char *s) -> uint32_t {
    uint32_t index = 0;
    do {
      s[index++] = '0' + (value % 10);
      value /= 10;
    } while (value != 0);

    for (uint32_t i = 0; i < index / 2; ++i) {
      char temp = s[i];
      s[i] = s[index - i - 1];
      s[index - i - 1] = temp;
    }
    s[index] = ',';
    return index + 1;
  };
  for (int i = 0; i < 65536; ++i) {
    size_t offset = const_int_to_str(i, p);
    p += offset;
    off[i + 1] = off[i] + offset;
  }
  return {str, off};
}

#endif
