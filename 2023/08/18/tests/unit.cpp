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
#include "latin1_to_utf8.h"
}

bool random_test() {
  printf("random_test\n");
  for (size_t i = 0; i < 1000000; i++) {
    std::string basic;
    basic.resize(rand() % 512, '0');
    for (size_t k = 0; k < basic.size(); k++) {
      basic[k] = rand();
    }
    std::string output1;
    output1.resize(basic.size() * 2);
    std::string output2;
    output2.resize(basic.size() * 2);
    size_t r1 = latin1_to_utf8(basic.data(), basic.size(), output1.data());
    if(r1 == 0) { continue; }
    output1.resize(r1);
    size_t r2 = latin1_to_utf8_avx512(basic.data(), basic.size(), output2.data());
    output2.resize(r2);
    if (output1 != output2) {
      if(r1 != r2) {
        printf("lengths do not match %zu %zu\n", r1, r2);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%02x \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%02x %02x %s \n", (unsigned)output1[i]&0xff, (unsigned)output2[i]&0xff, output1[i]!=output2[i]?"<==":"") ;
      }
      abort();
    }

  }
  printf("\nSUCCESS\n");
  return true; 
}

int main() {
 return (random_test()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
