
#pragma once
#include <cstdint>
#include <cstring>

// Converts an UTF-8 input into punycode.
//
// This function is non-allocating and it does not throw.
//
// Parameters:
//
// 'input' should be made of 'input_length' bytes representing a valid UTF-8
// sequence.
//
// 'input_length' should be at most 63.
//
// output should have capacity for at least 63 bytes. It should not overlap with
// input.
//
// We return how many bytes are written to 'output' or a negative integers in
// case of error. The errors include: an invalid UTF-8 input, a punycode
// overflow (unlikely) or an output that might exceed 63 bytes.
int utf8_to_punycode(const char *input, size_t input_length,
                     char *output) noexcept {
  if (input_length > 62) {
    return -1;
  } // unallowed.

  // Handle the basic code points
  unsigned char *char_pointer = (unsigned char *)input;
  unsigned char *const end_char_pointer = char_pointer + input_length;

  // first check if it is all ASCII !!!
  bool is_all_ascii = true;
  {
    uint64_t pos = 0;
    for (; pos + 9 < input_length; pos += 8) {
      uint64_t v;
      std::memcpy(&v, char_pointer + pos, sizeof(uint64_t));
      if ((v & 0x8080808080808080) != 0) {
        is_all_ascii = false;
        break;
      }
    }
    // process the tail byte-by-byte
    for (; is_all_ascii && pos < input_length; pos++) {
      if (char_pointer[pos] >= 0b10000000) {
        is_all_ascii = false;
        break;
      }
    }
  }
  if (is_all_ascii) {
    std::memcpy(output, input, input_length);
    output[input_length] = '-';
    return input_length + 1;
  }
  std::memcpy(output, "xn--", 4);
  output += 4;
  const char *const init_output = output;

  uint32_t all_buffer[64];
  uint32_t *all{all_buffer};
  uint32_t non_basic_buffer[64];
  uint32_t *non_basic{non_basic_buffer};

  while (char_pointer < end_char_pointer) {
    unsigned char c = *char_pointer;
    if (c >= 0b10000000) {
      size_t lookahead = size_t(char_pointer - end_char_pointer);
      uint32_t code_point;
      uint32_t leading_byte = c;

      if ((leading_byte & 0b11100000) == 0b11000000) {
        // We have a two-byte UTF-8
        if (lookahead < 2) {
          return -1;
        }

        if ((char_pointer[1] & 0b11000000) != 0b10000000) {
          return -1;
        }
        // range check
        code_point =
            (leading_byte & 0b00011111) << 6 | (char_pointer[1] & 0b00111111);
        if (code_point < 0x80 || 0x7ff < code_point) {
          return -1;
        }
        char_pointer += 2;
      } else if ((leading_byte & 0b11110000) == 0b11100000) {
        // We have a three-byte UTF-8
        if (lookahead < 3) {
          return -1;
        }
        if ((char_pointer[1] & 0b11000000) != 0b10000000) {
          return -1;
        }
        if ((char_pointer[2] & 0b11000000) != 0b10000000) {
          return -1;
        }
        // range check
        code_point = (leading_byte & 0b00001111) << 12 |
                     (char_pointer[1] & 0b00111111) << 6 |
                     (char_pointer[2] & 0b00111111);
        if (code_point < 0x800 || 0xffff < code_point ||
            (0xd7ff < code_point && code_point < 0xe000)) {
          return -1;
        }
        char_pointer += 3;
      } else if ((leading_byte & 0b11111000) == 0b11110000) { // 0b11110000
        // we have a 4-byte UTF-8 word.
        if (lookahead < 4) {
          return -1;
        }

        if ((char_pointer[1] & 0b11000000) != 0b10000000) {
          return -1;
        }
        if ((char_pointer[2] & 0b11000000) != 0b10000000) {
          return -1;
        }
        if ((char_pointer[3] & 0b11000000) != 0b10000000) {
          return -1;
        }

        // range check
        code_point = (leading_byte & 0b00000111) << 18 |
                     (char_pointer[1] & 0b00111111) << 12 |
                     (char_pointer[2] & 0b00111111) << 6 |
                     (char_pointer[3] & 0b00111111);
        if (code_point <= 0xffff || 0x10ffff < code_point) {
          return -1;
        }
        char_pointer += 4;
      } else {
        // continuation byte
        return -1;
      }
      *non_basic++ = code_point;
      *all++ = code_point;
      continue;
    }
    // ASCII !!!
    *output++ = c;
    *all++ = c;
    char_pointer++;
    continue;
  }

  size_t number_of_chars(all - all_buffer);
  size_t basic_count = output - init_output;
  if (basic_count > 0) {
    *output++ = '-';
  }
  uint32_t n = 128;
  uint32_t bias = 72;
  uint32_t delta = 0;
  auto sort_unique_values = [](uint32_t array[], size_t size) {
    size_t duplicates = 0;
    for (size_t k = 1; k < size; k++) {
      size_t z = k - duplicates;
      uint32_t key = array[k];
      for (; (z >= 1) && (array[z - 1] >= key); z--) {
      }
      if (z == k) {
        // nothing to do!
      } else if ((array[z] > key)) {
        std::memmove(array + z + 1, array + z,
                     (k - duplicates - z) * sizeof(uint32_t));
        array[z] = key;
      } else if (array[z] == key) {
        duplicates++;
      } else {
        array[z] = key;
      }
    }
  };

  sort_unique_values(non_basic_buffer, non_basic - non_basic_buffer);
  non_basic = non_basic_buffer;

  auto adapt = [](uint32_t delta, uint32_t n_points, bool is_first) {
    delta /= is_first ? 700 : 2;
    delta += delta / n_points;

    uint32_t s = 36 - 1;
    uint32_t t = (s * 26) / 2;

    uint32_t k = 0;
    for (; delta > t; k += 36) {
      delta /= s;
    }

    uint32_t a = (36 - 1 + 1) * delta;
    uint32_t b = (delta + 38);

    return k + (a / b);
  };
  for (size_t processed = basic_count; processed < number_of_chars;
       ++n, ++delta) {
    uint32_t non_ascii_code_point = *non_basic++;
    delta += (non_ascii_code_point - n) * (processed + 1);
    n = non_ascii_code_point;
    for (size_t i = 0; i < number_of_chars; i++) {
      uint32_t c = all_buffer[i];
      if (c < n && (++delta == 0)) { // overflow
        return -1;
      }
      if (c == n) {
        for (uint32_t q = delta, k = 36;; k += 36) {
          if (output >= init_output + 63) {
            return -1;
          } // too long !!!
          uint32_t t = k <= bias ? 1 : (k >= bias + 26 ? 26 : k - bias);
          if (q < t) {
            *output++ = q < 26 ? q + 97 : q + 22;
            break;
          }
          uint32_t char_value = t + (q - t) % (36 - t); // unfortunate division
          *output++ = char_value < 26 ? char_value + 97 : char_value + 22;
          q = (q - t) / (36 - t);
        }

        bias = adapt(delta, processed + 1, basic_count == processed);
        delta = 0;
        processed++;
      }
    }
  }
  return output - init_output + 4;
}
