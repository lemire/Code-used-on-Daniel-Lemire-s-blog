#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <x86intrin.h>

extern "C" {
#include "name_to_dnswire.h"
}


bool simple_test() {
  std::string basic = "lemire.me";
  printf("input:\n");
  for(size_t i = 0; i < basic.size(); i++) {
    printf("%02x ", basic[i]);
  }
  printf("\n");
  basic.resize(32);
  std::vector<uint8_t> out;
  out.resize(basic.size() + 32);
  size_t read = name_to_dnswire_simd(basic.c_str(), out.data());
  read += 2;
  printf("output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  read = name_to_dnswire(basic.c_str(), out.data());
  read += 2;
  printf("output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("SUCCESS\n");
  return true;
}
int main() {
  return (simple_test()) ? EXIT_SUCCESS
                          : EXIT_FAILURE;
}
