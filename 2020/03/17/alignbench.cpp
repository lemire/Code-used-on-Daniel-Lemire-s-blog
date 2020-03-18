#include <chrono>
#include <cstring>
#include <iostream>
#include <x86intrin.h>

#include "align_alloc.h"

void demo() {
  // allocate an 8kB, cache-page aligned block
  size_t N = 8192;
  char *buffer = (char *)aligned_malloc(64, N);
  memset(buffer, 1, N);
  // we are going to

  // if you hit unlucky, unlucky + 64,
  // you are exactly 16 bytes away from the
  // cache-line boundary
  char *unlucky = buffer + 48;
  __m256i vec = _mm256_setzero_si256();
  std::chrono::time_point<std::chrono::steady_clock> start_clock, end_clock;
  std::chrono::duration<double> elapsed = std::chrono::duration<double>::max();
  size_t maxrepeat = 500;
  for (int time = 0; time < maxrepeat; time++) {
    asm volatile("" : : : "memory");
    start_clock = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 127; i++) {
      _mm256_storeu_si256((__m256i *)(unlucky + i * 64), vec);
    }
    end_clock = std::chrono::steady_clock::now();
    elapsed =
        end_clock - start_clock < elapsed ? end_clock - start_clock : elapsed;
  }
  std::cout << "_mm256_storeu_si256      : "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed)
                       .count() /
                   127.0 << " ns" << std::endl;
  std::chrono::duration<double> elapsed128 =
      std::chrono::duration<double>::max();
  __m128i vec128 = _mm_setzero_si128();
  for (int time = 0; time < maxrepeat; time++) {

    asm volatile("" : : : "memory");
    start_clock = std::chrono::steady_clock::now();
    for (size_t i = 0; i < 127; i++) {
      _mm_storeu_si128((__m128i *)(unlucky + i * 64), vec128);
      _mm_storeu_si128((__m128i *)(unlucky + i * 64 + 16), vec128);
    }
    end_clock = std::chrono::steady_clock::now();
    elapsed128 = end_clock - start_clock < elapsed128 ? end_clock - start_clock
                                                      : elapsed128;
  }
  std::cout << "2 * _mm_storeu_si128      : "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed128)
                       .count() /
                   127.0 << " ns" << std::endl;
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
