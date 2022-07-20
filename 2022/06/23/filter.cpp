#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include "linux-perf-events.h"
#include <arm_sve.h>
#include <iostream>
#include <vector>
#include <random>
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


void remove_negatives_unrolled(const int32_t *input, int64_t count, int32_t *output) {
  int64_t j = 0;
  const int32_t* endPtr = input + count;
  const uint64_t vl_u32 = svcntw();

  svbool_t all_mask = svptrue_b32();
  while(input <= endPtr - (4*vl_u32))
  {
      svint32_t in0 = svld1_s32(all_mask, input + 0*vl_u32);
      svint32_t in1 = svld1_s32(all_mask, input + 1*vl_u32);
      svint32_t in2 = svld1_s32(all_mask, input + 2*vl_u32);
      svint32_t in3 = svld1_s32(all_mask, input + 3*vl_u32);

      svbool_t pos0 = svcmpge_n_s32(all_mask, in0, 0);
      svbool_t pos1 = svcmpge_n_s32(all_mask, in1, 0);
      svbool_t pos2 = svcmpge_n_s32(all_mask, in2, 0);
      svbool_t pos3 = svcmpge_n_s32(all_mask, in3, 0);

      in0 = svcompact_s32(pos0, in0);
      in1 = svcompact_s32(pos1, in1);
      in2 = svcompact_s32(pos2, in2);
      in3 = svcompact_s32(pos3, in3);

      svst1_s32(all_mask, output + j, in0);
      j += svcntp_b32(all_mask, pos0);
      svst1_s32(all_mask, output + j, in1);
      j += svcntp_b32(all_mask, pos1);
      svst1_s32(all_mask, output + j, in2);
      j += svcntp_b32(all_mask, pos2);
      svst1_s32(all_mask, output + j, in3);
      j += svcntp_b32(all_mask, pos3);

      input += 4*vl_u32;
  }

  int64_t i = 0;
  count = endPtr - input;

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

void remove_negatives_scalar_branchless(const int32_t *input, int64_t count, int32_t *output) {
  int64_t i = 0;
  int64_t j = 0;
  for(; i < count; i++) {
    output[j] = input[i];
    j += (input[i] >= 0);
  }
}
int main() {
  std::vector<int32_t> x;
  size_t size = 50000;
  std::random_device                  rand_dev;
  std::mt19937                        generator(rand_dev());
  std::uniform_int_distribution<int32_t>  distr(-1000,1000);
  for (size_t i = 0; i < size; i++) {
    x.push_back(distr(generator));
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
    std::cout << "cycles/int " << double(results[0]) / size << " ";
    std::cout << "instructions/int " << double(results[1]) / size << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
  std::cout << "scalar_branchless" << std::endl;
  for (size_t i = 0; i < 10; i++) {
    linux_events.start();
    remove_negatives_scalar_branchless(x.data(), x.size(), y.data());
    linux_events.end(results);
    std::cout << "cycles/int " << double(results[0]) / size << " ";
    std::cout << "instructions/int " << double(results[1]) / size << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
  std::cout << "sve" << std::endl; 
  for (size_t i = 0; i < 10; i++) {
    linux_events.start();
    remove_negatives(x.data(), x.size(), y.data());
    linux_events.end(results);
    std::cout << "cycles/int " << double(results[0]) / size << " ";
    std::cout << "instructions/int " << double(results[1]) / size << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
  std::cout << "sve unrolled" << std::endl; 
  for (size_t i = 0; i < 10; i++) {
    linux_events.start();
    remove_negatives_unrolled(x.data(), x.size(), y.data());
    linux_events.end(results);
    std::cout << "cycles/int " << double(results[0]) / size << " ";
    std::cout << "instructions/int " << double(results[1]) / size << " ";
    std::cout << "instructions/cycle " << double(results[1]) / results[0]
              << std::endl;
  }
   return EXIT_SUCCESS;
}
