#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <omp.h>
#include <string>
#include <string_view>
#include <vector>
#include <x86intrin.h>

extern "C" {
#include "name_to_dnswire.h"
}

bool simple_test() {
  printf("===\n");
  std::string basic = "lemire.me";
  printf("input:\n");
  for (size_t i = 0; i < basic.size(); i++) {
    printf("%02x ", basic[i]);
  }
  printf("\n");
  basic.resize(32);
  std::vector<uint8_t> out;
  out.resize(basic.size() + 32);
  size_t read = name_to_dnswire_simd(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire_simd output:\n");
  for (size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  read = name_to_dnswire(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire output:\n");
  for (size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");

  printf("\nSUCCESS\n");
  return true;
}

bool longer_test() {
  printf("===\n");
  std::string basic =
      "00";
  printf("input:\n");
  for (size_t i = 0; i < basic.size(); i++) {
    printf("%02x ", basic[i]);
  }
  printf("\n");
  basic.reserve(basic.size() + 32);
  std::vector<uint8_t> out;
  out.resize(basic.size() + 32);
  size_t read = name_to_dnswire(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire output:\n");
  for (size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  std::vector<uint8_t> out2;
  out2.resize(basic.size() + 32);
  size_t read2 = name_to_dnswire_simd(basic.c_str(), out2.data());
  read2 += 2;
  printf("name_to_dnswire_simd output:\n");
  for (size_t i = 0; i < read2; i++) {
    printf("%02x ", out2[i]);
  }
  printf("\n");
  if (read != read2) {
    printf("lengths don't match\n");
    return false;
  }
  bool match = true;
  size_t last_diff = -1;
  for (size_t i = 0; i < read; i++) {
    if (out[i] != out2[i]) {
      printf("%02x %02x %s\n", out[i], out2[i],
             out[i] == out2[i] ? "" : "<===");
      match = false;
      last_diff = i;
    }
  }

  std::vector<uint8_t> out3;
  out3.resize(basic.size() + 32);
  size_t read3 = name_to_dnswire_avx(basic.c_str(), out3.data());
  read3 += 2;
  printf("name_to_dnswire_avx output:\n");
  for (size_t i = 0; i < read3; i++) {
    printf("%02x ", out3[i]);
  }
  printf("\n");
  if (read != read3) {
    printf("lengths don't match\n");
    return false;
  }
  for (size_t i = 0; i < read; i++) {
    if (out[i] != out3[i]) {
      printf("%02x %02x %s\n", out[i], out3[i],
             out[i] == out2[i] ? "" : "<===");
      match = false;
      last_diff = i;
    }
  }
  if (match) {
    printf("\nSUCCESS\n");
  } else {
    printf("They differ at index %zu \n", last_diff);
  }
  return match;
}

bool random_test() {
  for (size_t i = 0; i < 100000; i++) {
    std::string basic;
    basic.resize(rand() % 255, '0');
    if (basic.size() <= 1) {
      basic.resize(2, '0');
    }
    basic.reserve(basic.size() + 32);
    size_t number_of_dots = rand() % basic.size();
    for (size_t k = 0; k < number_of_dots; k++) {
      size_t location = rand() % basic.size();
      // check that it is valid
      if (location == 0) {
        continue;
      }
      if (location > 0 && basic[location - 1] == '.') {
        continue;
      }
      if (location + 1 < basic.size() && basic[location + 1] == '.') {
        continue;
      }
      basic[location] = '.';
    }
    std::vector<uint8_t> out1;
    out1.resize(basic.size() + 32);
    size_t read1 = name_to_dnswire(basic.c_str(), out1.data());
    if (read1 == 0) {
      continue;
    }
    out1.resize(read1);
    std::vector<uint8_t> out2;
    out2.resize(basic.size() + 32);
    size_t read2 = name_to_dnswire_simd(basic.c_str(), out2.data());
    out2.resize(read2);
    if (out1 != out2) {
      std::cout << basic << std::endl;
      abort();
    }

    std::vector<uint8_t> out3;
    out3.resize(basic.size() + 32);
    size_t read3 = name_to_dnswire_avx(basic.c_str(), out3.data());
    out3.resize(read3);
    if (out1 != out3) {
      std::cout << basic << std::endl;
      abort();
    }
  }
  printf("\nSUCCESS\n");
  return true;
}

int main() {
  return (random_test()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
