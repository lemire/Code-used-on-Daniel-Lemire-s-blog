#include "m1cycles.h"
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sys/time.h>

/***
 * Fast Splittable Pseudorandom Number Generators
 * Steele Jr, Guy L., Doug Lea, and Christine H. Flood. "Fast splittable
 * pseudorandom number splitmix64_xgenerators."
 * ACM SIGPLAN Notices 49.10 (2014): 453-472.
 * ***/
struct splitmix_generator {
  uint64_t state;

  inline uint64_t operator()() {
    uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
  }
};

struct wyrng_generator {
  uint64_t state;

  inline uint64_t operator()() {
    state += 0x60bee2bee120fc15ull;
    __uint128_t tmp = (__uint128_t)(state)*0xa3b195354a39b70dull;
    uint64_t m1 = (tmp >> 64) ^ tmp;
    tmp = (__uint128_t)m1 * 0x1b03738712fad5c9ull;
    return (tmp >> 64) ^ tmp;
  }
};

template <class generator>
__attribute__((noinline)) void populate(uint64_t *out, size_t N,
                                        uint64_t seed) {
  generator g;
  g.state = seed;
#pragma clang loop unroll(disable)
  for (size_t i = 0; i < N; i++) {
    out[i] = g();
  }
}

volatile uint64_t hole{0};

template <class generator>
__attribute__((noinline)) std::pair<performance_counters, performance_counters>
time_populate(uint64_t *out, size_t N, size_t trials) {
  performance_counters min(1e300);
  performance_counters avg(0);
  for (size_t i = 0; i < trials; i++) {
    performance_counters start = get_counters();
    populate<generator>(out, N, 12345 + i);
    hole = out[N - 1];
    performance_counters end = get_counters();
    performance_counters time = (end - start);

    avg += time;
    min.min(time);
  }
  avg /= trials;
  min /= N;
  avg /= N;
  return std::make_pair(min, avg);
}

void print(std::pair<performance_counters, performance_counters> val) {
  performance_counters a = val.first;
  performance_counters b = val.second;
  std::cout << std::fixed;
  std::cout << std::setprecision(4) << std::setfill('0') << a.cycles
            << " cycles/value (" << std::setprecision(2) << std::setfill('0')
            << (b.cycles - a.cycles) / a.cycles * 100.0 << " %)\t";
  std::cout << std::setprecision(4) << std::setfill('0') << a.branches
            << " branches/value (" << std::setprecision(2) << std::setfill('0')
            << (b.branches - a.branches) / a.branches * 100.0 << " %)\t";
  std::cout << std::setprecision(4) << std::setfill('0') << a.missed_branches
            << " missed_branches/value (" << std::setprecision(2)
            << std::setfill('0')
            << (b.missed_branches - a.missed_branches) / a.missed_branches *
                   100.0
            << " %)\t";
  std::cout << std::setprecision(4) << std::setfill('0') << a.instructions
            << " instructions/value (" << std::setprecision(2)
            << std::setfill('0')
            << (b.instructions - a.instructions) / a.instructions * 100.0
            << " %)";
}

using namespace std;
int main(void) {
  setup_performance_counters();
  constexpr size_t N = 20000;
  size_t trials = 40000;
  uint64_t recipient[N];
  std::cout << "buffer size " << N * sizeof(uint64_t) / 1024 << " kB"
            << std::endl;
  time_populate<splitmix_generator>(recipient, N, trials);
  time_populate<wyrng_generator>(recipient, N, trials);
  for (size_t ti = 0; ti < 4; ti++) {
    std::cout << "splitmix ";
    print(time_populate<splitmix_generator>(recipient, N, trials));
    std::cout << std::endl;
    std::cout << "wyrng    ";
    print(time_populate<wyrng_generator>(recipient, N, trials));
    std::cout << std::endl;
    std::cout << "===========\n";
  }
  return EXIT_SUCCESS;
}
