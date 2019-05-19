#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

// tries to estimate the frequency, returns false on failure
 __attribute__ ((noinline))
double measure_frequency() {
  const size_t test_duration_in_cycles =
     65536;
  static_assert(test_duration_in_cycles / 8 * 8 == test_duration_in_cycles, "instruction counts needs to be a multiple of 8");
  // travis feels strongly about the measure-twice-and-subtract trick.
  auto begin1 = std::chrono::high_resolution_clock::now();
  size_t cycles = 2 * test_duration_in_cycles;
#ifdef __x86_64__
   // dec/jnz macro-fuse into a single uop on recent Intel processors
  __asm volatile("cyclemeasure1:\n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n jnz cyclemeasure1 \n"
                 : /* read/write reg */ [counter] "+r"(cycles));
#elif defined(__aarch64__)
  __asm volatile(
      ".align:4\ncyclemeasure1:\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsubs %[counter],%[counter],#1\nbne cyclemeasure1\n "
      : /* read/write reg */ [counter] "+r"(cycles));
#endif
  auto end1 = std::chrono::high_resolution_clock::now();
  double nanoseconds1 =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - begin1)
          .count();
  auto begin2 = std::chrono::high_resolution_clock::now();
  cycles = test_duration_in_cycles;
#ifdef __x86_64__
  // dec/jnz macro-fuse into a single uop on recent Intel processors
  __asm volatile("cyclemeasure2:\n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n dec %[counter] \n jnz cyclemeasure2 \n"
                 : /* read/write reg */ [counter] "+r"(cycles));
#elif defined(__aarch64__)
  __asm volatile(
      ".align:4\ncyclemeasure2:\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsub %[counter],%[counter],#1\nsubs %[counter],%[counter],#1\nbne cyclemeasure2\n "
      : /* read/write reg */ [counter] "+r"(cycles));
#endif
  auto end2 = std::chrono::high_resolution_clock::now();
  double nanoseconds2 =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - begin2)
          .count();
  double nanoseconds = (nanoseconds1 - nanoseconds2);
  if ((fabs(nanoseconds - nanoseconds1 / 2) > 0.05 * nanoseconds) or
      (fabs(nanoseconds - nanoseconds2) > 0.05 * nanoseconds)) {
    return 0;
  }

  double frequency = double(test_duration_in_cycles) / nanoseconds;
#ifdef __aarch64__
  frequency *= 2; // Many ARM processors need 2 cycles per iteration due to lack or flag renaming and fusion
#endif
  return frequency;
}

int main(int argc,char** argv) {
  int attempt = 10000;
  if(argc > 1) {
      attempt = atoi(argv[1]);
  }
  if(attempt <= 0) attempt = 1;
  std::vector<double> freqs;
  for (int i = 0; i < attempt; i++) {
    double freq = measure_frequency();
    if(freq > 0) freqs.push_back(freq);
  }
  if(freqs.size() == 0) {
    std::cout << "could not collect a measure" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Got " << freqs.size() << " measures." << std::endl;
  std::sort(freqs.begin(),freqs.end());
  std::cout << "Median frequency detected: " << freqs[freqs.size() / 2] << " GHz" << std::endl;
}
