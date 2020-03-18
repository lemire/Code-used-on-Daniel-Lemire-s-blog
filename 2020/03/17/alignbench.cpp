
#ifndef __linux__
#warning "We require Linux."
#else
#include "linux-perf-events.h"
#endif

#include <chrono>
#include <cstring>
#include <iostream>
#include <x86intrin.h>

#include "align_alloc.h"

void demo() {
  // allocate cache-page aligned block
  size_t N = 256 * 1024;
  char *buffer = (char *)aligned_malloc(64, N);
  memset(buffer, 1, N);
  size_t hits = (N - 64) / 64;
  std::vector<int> evts;
  evts.push_back(PERF_COUNT_HW_CPU_CYCLES);
  evts.push_back(PERF_COUNT_HW_INSTRUCTIONS);
  LinuxEvents<PERF_TYPE_HARDWARE> unified(evts);
  std::vector<unsigned long long> results;
  results.resize(evts.size());
  // if you hit unlucky, unlucky + 64,
  // you are exactly 16 bytes away from the
  // cache-line boundary
  char * const unlucky = buffer + 48;
  __m256i vec = _mm256_setzero_si256();
  std::chrono::time_point<std::chrono::steady_clock> start_clock, end_clock;
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::max();
  size_t maxrepeat = 500;
  size_t bestcycles = 10000000000000;
  for (int time = 0; time < maxrepeat; time++) {
    asm volatile("" : : : "memory");
    start_clock = std::chrono::steady_clock::now();
    unified.start();
    for (size_t i = 0; i < hits; i++) {
      _mm256_storeu_si256((__m256i *)(unlucky + i * 64), vec);
    }
    unified.end(results);
    end_clock = std::chrono::steady_clock::now();
    elapsed =
        end_clock - start_clock < elapsed ? end_clock - start_clock : elapsed;
    bestcycles = results[0] < bestcycles ? results[0] : bestcycles;
  }
  std::cout << "_mm256_storeu_si256      : "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed)
                       .count() /
                   double(hits) << " ns" << std::endl;
  std::cout << "cycles: " << double(bestcycles) / double(hits) << std::endl;
  std::chrono::duration<double> elapsed128 =
      std::chrono::duration<double>::max();
  __m128i vec128 = _mm_setzero_si128();
  for (int time = 0; time < maxrepeat; time++) {

    asm volatile("" : : : "memory");
    start_clock = std::chrono::steady_clock::now();
    unified.start();
    for (size_t i = 0; i < hits; i++) {
      _mm_storeu_si128((__m128i *)(unlucky + i * 64), vec128);
      _mm_storeu_si128((__m128i *)(unlucky + i * 64 + 16), vec128);
    }
    unified.end(results);
 
    end_clock = std::chrono::steady_clock::now();
    elapsed128 = end_clock - start_clock < elapsed128 ? end_clock - start_clock
                                                      : elapsed128;
    bestcycles = results[0] < bestcycles ? results[0] : bestcycles;
   }
  std::cout << "2 * _mm_storeu_si128      : "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed128)
                       .count() /
                   double(hits) << " ns" << std::endl;
  std::cout << "cycles: " << double(bestcycles) / double(hits) << std::endl;
  std::cout << "ratio "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed)
                       .count() *
                   1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(
                             elapsed128).count() << std::endl;
  aligned_free(buffer);
}

int main() {
  for (int k = 0; k < 10; k++) {
    std::cout << "trial " << k << std::endl;
    demo();
  }
  return EXIT_SUCCESS;
}
