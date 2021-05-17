
#include "m1cycles.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>

// The pointer c should be at then end of a region containing at least 10 bytes.
// The function returns a pointer to the start of the integer in ASCII
// written over the passed buffer.
// see https://github.com/cassioneri/calendar
__attribute__((noinline)) char *itoa_neri(char *c, uint32_t n) {
  uint64_t constexpr p32 = uint64_t(1) << 32;
  while (n >= 10) {
    uint64_t const u = uint64_t(429496730) * n;
    uint32_t const p = uint32_t(u / p32);
    uint32_t const r = uint32_t(u % p32) / 429496730;
    n = p;
    c--;
    *c = '0' + char(r);
  }
  c--;
  *c = '0' + char(n);
  return c;
}

// The pointer c should be at then end of a region containing at least 10 bytes.
// The function returns a pointer to the start of the integer in ASCII
// written over the passed buffer.
__attribute__((noinline)) char *itoa_std(char *c, uint32_t n) {
    size_t count{0};
  while (n >= 10) {
    uint32_t const p = n / 10;
    uint32_t const r = n % 10;
    n = p;
    c--;
    *c = '0' + char(r);
  }
  c--;
  *c = '0' + char(n);
  return c;
}

__attribute__((noinline)) char *fake_itoa_std(char *c, uint32_t n) {
  while (n >= 10) {
    n /= 10;
    c--;
  }
  c--;
  *c = '0' + char(n);
  return c;
}

class CPUBenchmark {
public:
  CPUBenchmark() : ticktime() { start(); }

  std::chrono::high_resolution_clock::time_point ticktime;

  void start() { ticktime = std::chrono::high_resolution_clock::now(); }

  unsigned long long stop() {
    auto finish = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(finish -
                                                                ticktime)
        .count();
  }
};

char buffer[11];

void run_std(uint32_t N) {
  // We go from 20 * 1000 * 1000 to 40 * 1000 * 1000
  // so that we have a flat number of digits (to ease analysis)
  uint8_t seed = 0;
  for (uint32_t i = N; i < 2 * N; i++) {
    seed = *itoa_std(buffer + 10, i+seed);
  }
}

void run_fake(uint32_t N) {
  uint8_t seed = 0;
  for (uint32_t i = N; i < 2 * N; i++) {
    seed = *fake_itoa_std(buffer + 10, i+seed);
  }
}
void run_neri(uint32_t N) {
  uint8_t seed = 0;
  for (uint32_t i = N; i < 2 * N; i++) {
    seed = *itoa_neri(buffer + 10, i+seed);
  }
}

void demo() {
  CPUBenchmark time;
  const uint32_t N = 20 * 1000 * 1000;
  double x, y;
  size_t trials = 10;

  std::cout.precision(3);
  std::cout << " report speed indicator in nanoseconds per integer"
            << std::endl;
  x = 1e300;
  y = 0;
  {
#ifdef __APPLE__
    performance_counters agg_min{1e300};
    performance_counters agg_avg{0.0};
#endif
    for (size_t i = 0; i < trials; i++) {
#ifdef __APPLE__
      performance_counters start = get_counters();
#endif
      time.start();
      run_std(N);
      auto result = time.stop() * 1.0 / N;
#ifdef __APPLE__
      performance_counters end = get_counters();
      performance_counters diff = end - start;
      agg_min = agg_min.min(diff);
      agg_avg += diff;
#endif
      y += result;
      if (result < x) {
        x = result;
      }
    }
#ifdef __APPLE__
    agg_avg /= N * trials;
    agg_min /= N;
#endif
    y = y / trials;
#ifdef __APPLE__
    std::cout << "with std     : " << x << " (" << (y - x) * 100 / x << " %)"
              << std::endl;
    printf(" %8.2f instructions/int (+/- %3.1f %%) ", agg_min.instructions,
           (agg_avg.instructions - agg_min.instructions) * 100.0 /
               agg_min.instructions);
    printf("\n");
    printf(" %8.2f cycles/int (+/- %3.1f %%) ", agg_min.cycles,
           (agg_avg.cycles - agg_min.cycles) * 100.0 / agg_min.cycles);
    printf("\n");
#endif
  }

  x = 1e300;
  y = 0;
  {
#ifdef __APPLE__
    performance_counters agg_min{1e300};
    performance_counters agg_avg{0.0};
#endif
    for (size_t i = 0; i < trials; i++) {
#ifdef __APPLE__
      performance_counters start = get_counters();
#endif
      time.start();
      run_fake(N);
      auto result = time.stop() * 1.0 / N;
#ifdef __APPLE__
      performance_counters end = get_counters();
      performance_counters diff = end - start;
      agg_min = agg_min.min(diff);
      agg_avg += diff;
#endif
      y += result;
      if (result < x) {
        x = result;
      }
    }
#ifdef __APPLE__
    agg_avg /= N * trials;
    agg_min /= N;
#endif
    y = y / trials;
#ifdef __APPLE__
    std::cout << "fake         : " << x << " (" << (y - x) * 100 / x << " %)"
              << std::endl;
    printf(" %8.2f instructions/int (+/- %3.1f %%) ", agg_min.instructions,
           (agg_avg.instructions - agg_min.instructions) * 100.0 /
               agg_min.instructions);
    printf("\n");
    printf(" %8.2f cycles/int (+/- %3.1f %%) ", agg_min.cycles,
           (agg_avg.cycles - agg_min.cycles) * 100.0 / agg_min.cycles);
    printf("\n");
#endif
  }

  x = 1e300;
  y = 0;
  for (size_t i = 0; i < trials; i++) {
    time.start();
    run_neri(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
  }
  y = y / trials;
  std::cout << "neri         : " << x << " (" << (y - x) * 100 / x << " %)"
            << std::endl;
}

int main() {
  setup_performance_counters();
  demo();
  demo();
}
