#include <iostream>
#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include <arm_sve.h>
#include <iostream>

int main() {
  std::cout << "detected vector register size (SVE): " << svcntb() << " bytes" << std::endl;
  return EXIT_SUCCESS;
}
