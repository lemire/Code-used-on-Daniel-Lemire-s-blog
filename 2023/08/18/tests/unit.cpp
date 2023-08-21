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
    basic.resize(rand() % 32, '0');
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

    std::string output3;
    output3.resize(basic.size() * 2);
    size_t r3 = latin1_to_utf8_avx512_InstLatX64(basic.data(), basic.size(), output3.data());
    output3.resize(r3);
    if (output1 != output3) {
      if(r1 != r3) {
        printf("lengths do not match %zu %zu\n", r1, r3);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%02x \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%02x %02x %s \n", (unsigned)output1[i]&0xff, (unsigned)output3[i]&0xff, output1[i]!=output3[i]?"<==":"") ;
      }
      abort();
    }


    std::string output4;
    output4.resize(basic.size() * 2);
    size_t r4 = latin1_to_utf8_avx512_my_branch1(basic.data(), basic.size(), output4.data());
    output4.resize(r4);
    if (output1 != output4) {
      if(r1 != r4) {
        printf("lengths do not match %zu %zu\n", r1, r4);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%02x \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%02x %02x %s \n", (unsigned)output1[i]&0xff, (unsigned)output4[i]&0xff, output1[i]!=output4[i]?"<==":"") ;
      }
      printf("problem with latin1_to_utf8_avx512_my_branch1\n");
      abort();
    }


    std::string output5;
    output5.resize(basic.size() * 2);
    size_t r5 = latin1_to_utf8_avx512_my_branch0(basic.data(), basic.size(), output5.data());
    output5.resize(r5);
    if (output1 != output5) {
      if(r1 != r5) {
        printf("lengths do not match %zu %zu\n", r1, r5);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%02x \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%02x %02x %s \n", (unsigned)output1[i]&0xff, (unsigned)output5[i]&0xff, output1[i]!=output5[i]?"<==":"") ;
      }
      printf("problem with latin1_to_utf8_avx512_my_branch0\n");
      abort();
    }


    std::string output6;
    output6.resize(basic.size() * 2);
    size_t r6 = latin1_to_utf8_avx512_my_nobranch(basic.data(), basic.size(), output6.data());
    output6.resize(r6);
    if (output1 != output6) {
      if(r1 != r6) {
        printf("lengths do not match %zu %zu\n", r1, r6);
        abort();
      }
      for(size_t i = 0; i < basic.size(); i++) {
        printf("%02x \n", (unsigned)basic[i]&0xff);
      }
      for(size_t i = 0; i < r1; i++) {
        printf("%02x %02x %s \n", (unsigned)output1[i]&0xff, (unsigned)output6[i]&0xff, output1[i]!=output6[i]?"<==":"") ;
      }
      printf("problem with latin1_to_utf8_avx512_my_nobranch\n");
      abort();
    }

  }
  printf("\nSUCCESS\n");
  return true; 
}

int main() {
 return (random_test()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
