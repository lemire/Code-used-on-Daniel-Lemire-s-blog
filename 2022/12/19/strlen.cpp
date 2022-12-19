#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include "linux-perf-events.h"
#include <arm_sve.h>
#include <iostream>
#include <random>
#include <vector>

size_t sve_strlen(const char *s) {
  /*if(svcntb() > 256) {
    // do something here because we assume that our
    // vectors have no more than 256.
  }*/
  size_t len = 0;
  while (true) {
    svuint8_t input = svldff1_u8(svptrue_b8(), (const uint8_t *)s + len);
    svbool_t matches = svcmpeq_n_u8(svptrue_b8(), input, 0);
    if (svptest_any(svptrue_b8(), matches)) {
      return len + svlastb_u8(svbrka_z(matches, matches), svindex_u8(0, 1));
    }
    len += svcntb();
  }
}
void demo(size_t size) {
  std::cout << "size = " << size << std::endl;
  std::vector<char> x;
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<char> distr(1, 100);
  for (size_t i = 0; i < size; i++) {
    x.push_back(distr(generator));
  }
  x[size - 1] = 0;
  LinuxEvents<PERF_TYPE_HARDWARE> linux_events(std::vector<int>{
      PERF_COUNT_HW_CPU_CYCLES,
      PERF_COUNT_HW_INSTRUCTIONS,
  });
  volatile size_t len;
  std::cout << "scalar" << std::endl;
  std::vector<unsigned long long> results(2);
  linux_events.start();
  len = strlen(x.data());
  linux_events.end(results);
  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << "sve" << std::endl;

  linux_events.start();
  len = sve_strlen(x.data());
  linux_events.end(results);
  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;
}
int main() {
  demo(10);
  demo(100);
  for(size_t i = 0; i <10; i++) {
  demo(rand()%2000);

  }

  return EXIT_SUCCESS;
}
