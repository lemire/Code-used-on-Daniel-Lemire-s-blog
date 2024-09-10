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


std::pair<std::array<char, 382106>, std::array<uint32_t, 65537>>
precompute_string_really_fast() {
  std::array<char, 382106> str;
  std::array<uint32_t, 65537> off;
  char buffer[6] = {'0', '0', '0', '0', '0', ','}; // 5 digits + ','
  size_t digit_length = 2;
  off[0] = 0;
  char *p = &str[0];
  for (int i = 0; i < 65536; ++i) {
    memcpy(p, buffer + 6 - digit_length, digit_length);
    p += digit_length;
    off[i + 1] = off[i] + digit_length;
    buffer[4] += 1;
    if(buffer[4] > '9') {
      if(digit_length == 2) { digit_length++;}
      buffer[4] = '0';
      buffer[3] += 1;
      if(buffer[3] > '9') {
        if(digit_length == 3) { digit_length++;}
        buffer[3] = '0';
        buffer[2] += 1;
        if(buffer[2] > '9') {
          if(digit_length == 4) { digit_length++;}
          buffer[2] = '0';
          buffer[1] += 1;
          if(buffer[1] > '9') {
            if(digit_length == 5) { digit_length++;}
            buffer[1] = '0';
            buffer[0] += 1;
          }
        }
      }
    }
  }
  return {str, off};
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
