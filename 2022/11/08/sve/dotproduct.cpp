#include <iostream>
#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include <arm_sve.h>

float dotsve_simple(float *x1, float *x2, int64_t length) {
  int64_t i = 0;
  svfloat32_t sum = svdup_n_f32(0);
  svbool_t while_mask = svwhilelt_b32(i, length);
  do {
    svfloat32_t in1 = svld1_f32(while_mask, x1 + i);
    svfloat32_t in2 = svld1_f32(while_mask, x2 + i);
    sum = svmad_f32_m(svptrue_b32(), in1, in2, sum);
    i += svcntw(); // can use svqincw_n_s64(i, 1);
    while_mask = svwhilelt_b32(i, length);
  } while (svptest_any(svptrue_b32(), while_mask));

  return svaddv_f32(svptrue_b32(),sum);
}


float dotsve(float *x1, float *x2, int64_t length) {
  int64_t i = 0;
  svfloat32_t sum = svdup_n_f32(0);
  while(i + svcntw() <= length) {
    svfloat32_t in1 = svld1_f32(svptrue_b32(), x1 + i);
    svfloat32_t in2 = svld1_f32(svptrue_b32(), x2 + i);
    sum = svmad_f32_m(svptrue_b32(), in1, in2, sum);
    i += svcntw(); // can use svqincw_n_s64(i, 1);
  }
  svbool_t while_mask = svwhilelt_b32(i, length);
  do {
    svfloat32_t in1 = svld1_f32(while_mask, x1 + i);
    svfloat32_t in2 = svld1_f32(while_mask, x2 + i);
    sum = svmad_f32_m(svptrue_b32(), in1, in2, sum);
    i += svcntw(); // can use svqincw_n_s64(i, 1);
    while_mask = svwhilelt_b32(i, length);
  } while (svptest_any(svptrue_b32(), while_mask));

  return svaddv_f32(svptrue_b32(),sum);
}
int main() {
  float x1[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  float x2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::cout << dotsve(x1, x2, 29) << std::endl;
}