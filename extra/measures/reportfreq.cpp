#include <chrono>
#include <cstdint>
#include <iostream>

void measure_frequency() {
  const size_t test_duration_in_cycles =
      1000 * 1000; // we are going to use 1 million cycles
  size_t cycles = test_duration_in_cycles;
  auto begin = std::chrono::high_resolution_clock::now();
#ifdef __x86_64__
  __asm volatile("cyclemeasure:\n dec %[counter] \n jnz cyclemeasure \n"
                 : /* read/write reg */ [counter] "+r"(cycles));
#elif defined(__aarch64__)
  __asm volatile(
      "cyclemeasure:\nsubs %[counter],%[counter],#1\nbne cyclemeasure\n "
      : /* read/write reg */ [counter] "+r"(cycles));
#endif
  auto end = std::chrono::high_resolution_clock::now();
  double nanoseconds =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
  std::cout << nanoseconds << "ns" << std::endl;
  std::cout << test_duration_in_cycles << "cycles" << std::endl;

  double frequency = double(test_duration_in_cycles) / nanoseconds;
  std::cout << frequency << "GHz" << std::endl;
}

int main() { measure_frequency(); }
