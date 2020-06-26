
#include <fenv.h>
#include <float.h>
#include <iostream>

#pragma STDC FENV_ACCESS ON

#include <stdio.h>
int main() {
  double x = 50178230318.0;
  double y = 100000000000.0;
  double ratio = x/y;
  printf("x/y                          = %18.18f\n", x / y);
  printf("expected x/y (round to even) = %18.18f\n", 0.501782303180000055);
  printf("                              ");
  bool is_correct = (50178230318.0 / 100000000000.0 == 0.501782303180000055);
  std::cout << (is_correct ? " Equal" : "Not equal") << std::endl;
  printf("                              ");
  is_correct = (ratio == 0.501782303180000055);
  std::cout << (is_correct ? " Equal" : "Not equal") << std::endl;
  fesetround(FE_DOWNWARD);
  printf("FE_DOWNWARD  : %18.18f\n", x / y);

  fesetround(FE_TONEAREST);
  printf("FE_TONEAREST : %18.18f\n", x / y);

  fesetround(FE_TOWARDZERO);
  printf("FE_TOWARDZERO: %18.18f\n", x / y);

  fesetround(FE_UPWARD);
  printf("FE_UPWARD    : %18.18f\n", x / y);


  fesetround(FE_TONEAREST);
  printf("FLT_EVAL_METHOD = %d \n", FLT_EVAL_METHOD);
  return 0;
}
