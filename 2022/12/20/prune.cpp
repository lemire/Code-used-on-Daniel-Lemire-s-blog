#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include "linux-perf-events.h"
#include <arm_sve.h>
#include <iostream>
#include <random>
#include <vector>

size_t trimspaces(const char *s, size_t len, char *out) {
  char * init_out{out};
  for(size_t i = 0; i < len; i++) {
    *out = s[i];
    out += (s[i] != ' ');
  }
  return out - init_out;
}

size_t sve_trimspaces(const char *s, size_t len, char *out) {
  uint8_t *out8 = reinterpret_cast<uint8_t *>(out);
  size_t i = 0;
  for (; i + svcntw() <= len; i += svcntw()) {
    svuint32_t input = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(svptrue_b32(), input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(svptrue_b32(), out8, compressed);
    out8 += svcntp_b32(svptrue_b32(), matches);
  }
  if (i < len) {
    svbool_t read_mask = svwhilelt_b32(i, len);
    svuint32_t input = svld1sb_u32(read_mask, (const int8_t *)s + i);
    svbool_t matches = svcmpeq_n_u32(read_mask, input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(read_mask, out8, compressed);
    out8 += svcntp_b32(read_mask, matches);
  }
  return out8 - reinterpret_cast<uint8_t *>(out);
}
int main() {
  size_t N = 10000;
  char * input = new char[N];
  char * output = new char[N];
  for(size_t i = 0; i < N; i++) {
    input[i] = rand();
  }
  LinuxEvents<PERF_TYPE_HARDWARE> linux_events(std::vector<int>{
      PERF_COUNT_HW_CPU_CYCLES,
      PERF_COUNT_HW_INSTRUCTIONS,
  });
  volatile size_t len{0};
  std::cout << "scalar" << std::endl;
  std::vector<unsigned long long> results(2);

  linux_events.start();
  len = trimspaces(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;
  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;


  std::cout << "sve" << std::endl;

  linux_events.start();
  len = sve_trimspaces(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;


  return EXIT_SUCCESS;
}
