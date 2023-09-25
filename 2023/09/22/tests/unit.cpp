#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "parse_integer.h"

bool basic_test() {
  printf("basic_test\n");
  uint64_t values[] = {9223372036854775808ULL, 9223372036854775807ULL,
                       100000000, 1};
  for (uint64_t target : values) {
    printf("target: %lu\n", target);
    uint64_t value;
    std::string input = std::to_string(target) + ".";
    auto r = parse_unsigned(input.data(), value);
    if (!r) {
      printf("parse_unsigned rejected\n");
      return false;
    }
    if (value != target) {
      printf("parse_unsigned bad %zu\n", value);

      return false;
    }

    r = parse_unsigned_bounded(input.data(), input.data() + input.size() - 1,
                               value);
    if (!r) {
      printf("parse_unsigned_bounded rejected\n");
      return false;
    }
    if (value != target) {
      printf("parse_unsigned_bounded bad %zu\n", value);

      return false;
    }

    r = parse_unsigned_bounded_reverse(input.data(),
                                       input.data() + input.size() - 1, value);
    if (!r) {
      printf("parse_unsigned_bounded_reverse rejected\n");
      return false;
    }
    if (value != target) {
      printf("parse_unsigned_bounded_reverse bad %zu\n", value);
      return false;
    }

    r = parse_unsigned_avx512(input.data(), input.data() + input.size() - 1,
                              value);
    if (!r) {
      printf("parse_unsigned_avx512 rejected\n");
      return false;
    }
    if (value != target) {
      printf("bad %zu\n", value);
      return false;
    }

    r = parse_unsigned_avx512_perforated(input.data(), input.data() + input.size() - 1,
                              value);
    if (!r) {
      printf("parse_unsigned_avx512_perforated rejected\n");
      return false;
    }
    if (value != target) {
      printf("bad %zu\n", value);
      return false;
    }

  }

  printf("SUCCESS\n");
  return true;
}

int main() { return (basic_test()) ? EXIT_SUCCESS : EXIT_FAILURE; }
