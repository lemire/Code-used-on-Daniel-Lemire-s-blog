#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include "linux-perf-events.h"
#include <arm_sve.h>
#include <iostream>
#include <vector>
void remove_negatives(const int32_t *input, int64_t count, int32_t *output) {
  int64_t i = 0;
  int64_t j = 0;
  svbool_t while_mask = svwhilelt_b32(i, count);
  do {
    svint32_t in = svld1_s32(while_mask, input + i);
    svbool_t positive = svcmpge_n_s32(while_mask, in, 0);
    svint32_t in_positive = svcompact_s32(positive, in);
    svst1_s32(while_mask, output + j, in_positive);
    i += svcntw();
    j += svcntp_b32(while_mask, positive);
    while_mask = svwhilelt_b32(i, count);
  } while (svptest_any(svptrue_b32(), while_mask));
}

// the compiler could vectorize
void remove_negatives_scalar(const int32_t *input, int64_t count, int32_t *output) {
  int64_t i = 0;
  int64_t j = 0;
  for(; i < count; i++) {
    if(input[i] >= 0) {
      output[j++] = input[i];
    }
  }
}
int main() {
  std::vector<int32_t> x;
  size_t size = 1000;
  for (size_t i = 0; i < size; i++) {
    x.push_back(1 - i + 100 * (i & 1));
  }
  std::vector<int32_t> y;
  y.resize(x.size());

  LinuxEvents<PERF_TYPE_HARDWARE> linux_events(std::vector<int>{
      PERF_COUNT_HW_CPU_CYCLES,
      PERF_COUNT_HW_INSTRUCTIONS,
  });
  std::cout << "scalar" << std::endl;
  std::vector<unsigned long long> results(2);
  for (size_t i = 0; i < 10; i++) {
    linux_events.start();
    remove_negatives_scalar(x.data(), x.size(), y.data());
    linux_events.end(results);
    std::cout << "cycles/int " << results[0] / 1000. << " ";
    std::cout << "instructions/int " << results[1] / 1000. << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
  std::cout << "sve" << std::endl; 
  for (size_t i = 0; i < 10; i++) {
    linux_events.start();
    remove_negatives(x.data(), x.size(), y.data());
    linux_events.end(results);
    std::cout << "cycles/int " << results[0] / 1000. << " ";
    std::cout << "instructions/int " << results[1] / 1000. << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
  return EXIT_SUCCESS;
}