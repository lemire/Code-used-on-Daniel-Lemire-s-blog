
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>

// The pointer c should be at then end of a region containing at least 10 bytes.
// The function returns a pointer to the start of the integer in ASCII
// written over the passed buffer.
// see https://github.com/cassioneri/calendar
__attribute__ ((noinline))
char *itoa_neri(char *c, uint32_t n) {
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
__attribute__ ((noinline))
char *itoa_std(char *c, uint32_t n) {
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
  for (uint32_t i = 0; i < N; i++) {
    itoa_std(buffer + 10, i);
  }
}

void run_neri(uint32_t N) {
  for (uint32_t i = 0; i < N; i++) {
    itoa_neri(buffer + 10, i);
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
  for (size_t i = 0; i < trials; i++) {
    time.start();
    run_std(N);
    auto result = time.stop() * 1.0 / N;
    y += result;
    if (result < x) {
      x = result;
    }
  }
  y = y / trials;
  std::cout << "with std     : " << x << " ("
            << (y - x) * 100 / x << " %)" << std::endl;

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
  std::cout << "neri         : " << x << " (" << (y - x) * 100 / x
            << " %)" << std::endl;
}

int main() { demo();   demo(); }
