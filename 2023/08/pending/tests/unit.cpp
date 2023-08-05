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
  printf("===\n");
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
  printf("name_to_dnswire_simd output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  read = name_to_dnswire_simd_fast(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire_simd_fast output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  read = name_to_dnswire(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");

  read = name_to_dnswire_simd_full(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire_simd_full output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\nSUCCESS\n");
  return true;
}


bool longer_test() {
  printf("===\n");
  std::string basic = "00000000000000.000000000000000000000000000.00000000.0000000000000000.000000000000000000000.000000000000000000000000.000.000000.000000000000000000.000000000000000000.0000000.00000.00.0000000000"; //"000.00.000000.00000.00000.00000.00.00000.00";
  printf("input:\n");
  for(size_t i = 0; i < basic.size(); i++) {
    printf("%02x ", basic[i]);
  }
  printf("\n");
  basic.reserve(basic.size() + 32);
  std::vector<uint8_t> out;
  out.resize(basic.size() + 32);
  size_t read = name_to_dnswire(basic.c_str(), out.data());
  read += 2;
  printf("name_to_dnswire output:\n");
  for(size_t i = 0; i < read; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  std::vector<uint8_t> out2;
  out2.resize(basic.size() + 32);
  size_t read2 = name_to_dnswire_simd_full(basic.c_str(), out2.data());
  read2 += 2;
  printf("name_to_dnswire_simd_full output:\n");
  for(size_t i = 0; i < read2; i++) {
    printf("%02x ", out2[i]);
  }
  if(read != read2) { printf("lengths don't match\n"); return false; }
  bool match = true;
  for(size_t i = 0; i < read; i++) {
    printf("%02x %02x %s\n", out[i], out2[i], out[i] == out2[i] ? "" : "<===");
    if(out[i] != out2[i]) { match = false; }
  }

  if(match) { printf("\nSUCCESS\n"); }
  return match;
}

bool random_test() {
  for(size_t i = 0; i < 100; i++) {
    std::string basic;
    basic.resize(rand()%255, '0');
    basic.reserve(basic.size() + 32);
    size_t number_of_dots = rand() % basic.size();
    for(size_t k = 0; k < number_of_dots; k++) {
      size_t location = rand() % basic.size();
      // check that it is valid
      if(location > 0 && basic[location - 1] == '.') { continue; }
      if(location + 1 < basic.size() && basic[location + 1] == '.') { continue; }
      basic[location] = '.';
    }
    std::cout << basic << std::endl;
    std::vector<uint8_t> out1;
    out1.resize(basic.size() + 32);
    size_t read1 = name_to_dnswire(basic.c_str(), out1.data());
    if(read1 == 0) { continue; }
    out1.resize(read1);
    std::vector<uint8_t> out2;
    out2.resize(basic.size() + 32);
    size_t read2 = name_to_dnswire_simd_full(basic.c_str(), out2.data());
    out2.resize(read2);
    if(out1 != out2) {
      std::cout << basic << std::endl; 
      abort();
    }
  }
  printf("\nSUCCESS\n");
  return true;
}
//&& random_test()
int main() {
  return (simple_test() && longer_test()  ) ? EXIT_SUCCESS
                          : EXIT_FAILURE;
}
