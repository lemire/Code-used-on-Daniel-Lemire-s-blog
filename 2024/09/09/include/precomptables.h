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

std::array<char, 6 * 65536> precompute_string_really_fast() {
  std::array<char, 6 * 65536> str;
  char *p = &str[0];
  for (int i = 0; i < 65536; ++i) {
    p[0] = '0' + (i / 10000);
    p[1] = '0' + (i / 1000) % 10;
    p[2] = '0' + (i / 100) % 10;
    p[3] = '0' + (i / 10) % 10;
    p[4] = '0' + i % 10;
    p[5] = ',';
    p += 6;
  }
  return str;
}

std::array<char, 8 * 65536> precompute_string_really_really_fast() {
  std::array<char, 8 * 65536> str;
  char *p = &str[0];
  for (int i = 0; i < 10; ++i) {
    p[0] = 2;
    p[1] = 0;
    p[2] = '0';
    p[3] = '0';
    p[4] = '0';
    p[5] = '0';
    p[6] = '0' + i % 10;
    p[7] = ',';
    p += 6;
  }
  for (int i = 10; i < 100; ++i) {
    p[0] = 3;
    p[1] = 0;
    p[2] = '0';
    p[3] = '0';
    p[4] = '0';
    p[5] = '0' + (i / 10) % 10;
    p[6] = '0' + i % 10;
    p[7] = ',';
    p += 8;
  }
  for (int i = 100; i < 1000; ++i) {
    p[0] = 4;
    p[1] = 0;
    p[2] = '0';
    p[3] = '0';
    p[4] = '0' + (i / 100) % 10;
    p[5] = '0' + (i / 10) % 10;
    p[6] = '0' + i % 10;
    p[7] = ',';
    p += 8;
  }
  for (int i = 1000; i < 10000; ++i) {
    p[0] = 5;
    p[1] = 0;
    p[2] = '0';
    p[3] = '0' + (i / 1000) % 10;
    p[4] = '0' + (i / 100) % 10;
    p[5] = '0' + (i / 10) % 10;
    p[6] = '0' + i % 10;
    p[7] = ',';
    p += 8;
  }
  for (int i = 10000; i < 65536; ++i) {
    p[0] = 6;
    p[1] = 0;
    p[2] = '0' + (i / 10000);
    p[3] = '0' + (i / 1000) % 10;
    p[4] = '0' + (i / 100) % 10;
    p[5] = '0' + (i / 10) % 10;
    p[6] = '0' + i % 10;
    p[7] = ',';
    p += 8;
  }
  return str;
}

std::array<char, 382106> precompute_string_fast_slim() {
  std::array<char, 382106> str;
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
  }
  return str;
}
#endif
