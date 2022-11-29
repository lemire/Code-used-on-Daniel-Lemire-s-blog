#include <iostream>
#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include <arm_sve.h>
#include <iostream>

int main() {
  svint8_t bogus;
  std::cout << "detected vector register size (SVE): " << svlen_s8(bogus) << " bytes" << std::endl;
  return EXIT_SUCCESS;
}
