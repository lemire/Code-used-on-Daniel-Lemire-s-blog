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
#include "utf8_to_latin1.h"
}

bool random_test() {
  printf("random_test\n");
  for (size_t i = 0; i < 100000; i++) {
    std::string basic;
    basic.resize(rand() % 255, '0');
    for (size_t k = 0; k < basic.size(); k++) {
      basic[k] = rand();
    }
    std::string output1;
    output1.resize(basic.size());
    std::string output2;
    output2.resize(basic.size());
    size_t r1 = utf8_to_latin1(basic.data(), basic.size(), output1.data());
    if(r1 == 0) { continue; }
    output1.resize(r1);
    size_t r2 = utf8_to_latin1_avx512(basic.data(), basic.size(), output2.data());
    output2.resize(r2);
    if (output1 != output2) {
      if(r1 != r2) {
        printf("lengths do not match %zu %zu\n", r1, r2);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%u \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%u %u \n", (unsigned)output1[i]&0xff, (unsigned)output2[i]&0xff);
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
