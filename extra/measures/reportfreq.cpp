#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
void measure_frequency() {
  const size_t test_duration_in_cycles =
      10 * 1000 * 1000; // we are going to use 10 million cycles
  // travis feels strongly about the measure-twice-and-subtract trick.
  auto begin1 = std::chrono::high_resolution_clock::now();
  size_t cycles = 2 * test_duration_in_cycles;
#ifdef __x86_64__
  __asm volatile("cyclemeasure1:\n dec %[counter] \n jnz cyclemeasure1 \n"
                 : /* read/write reg */ [counter] "+r"(cycles));
#elif defined(__aarch64__)
  __asm volatile(
      "cyclemeasure1:\nsubs %[counter],%[counter],#1\nbne cyclemeasure1\n "
      : /* read/write reg */ [counter] "+r"(cycles));
#endif
  auto end1 = std::chrono::high_resolution_clock::now();
  double nanoseconds1 =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - begin1)
          .count();
  std::cout << "first time measure: " << nanoseconds1 << " ns (2x) "
            << std::endl;

  auto begin2 = std::chrono::high_resolution_clock::now();
  cycles = test_duration_in_cycles;
#ifdef __x86_64__
  __asm volatile("cyclemeasure2:\n dec %[counter] \n jnz cyclemeasure2 \n"
                 : /* read/write reg */ [counter] "+r"(cycles));
#elif defined(__aarch64__)
  __asm volatile(
      "cyclemeasure2:\nsubs %[counter],%[counter],#1\nbne cyclemeasure2\n "
      : /* read/write reg */ [counter] "+r"(cycles));
#endif
  auto end2 = std::chrono::high_resolution_clock::now();
  double nanoseconds2 =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - begin2)
          .count();
  std::cout << "second time measure: " << nanoseconds2 << " ns " << std::endl;
  double nanoseconds = (nanoseconds1 - nanoseconds2);
  std::cout << "difference: " << nanoseconds << std::endl;
  if ((fabs(nanoseconds - nanoseconds1 / 2) > 0.01 * nanoseconds) or
      (fabs(nanoseconds - nanoseconds2) > 0.01 * nanoseconds)) {
    std::cout << "Suspicious overhead, aborting measure." << std::endl;
    return;
  }
  std::cout << nanoseconds << " ns" << std::endl;
  std::cout << test_duration_in_cycles << " cycles" << std::endl;

  double frequency = double(test_duration_in_cycles) / nanoseconds;
  std::cout << frequency << "GHz" << std::endl;
}

int main() {
  for (int i = 0; i < 10; i++) {
    std::cout << "test " << i << std::endl;
    measure_frequency();
    std::cout << std::endl;
  }
}
