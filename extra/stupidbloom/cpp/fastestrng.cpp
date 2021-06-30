#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sys/time.h>

// will set up to 5 bits to 1.
uint64_t broacast_bits(uint64_t x) {
    return (uint64_t(1) << (x))
    | (uint64_t(1) << ((x>>8)))
    | (uint64_t(1) << ((x>>16)))
    | (uint64_t(1) << ((x>>24)))
    | (uint64_t(1) << ((x>>32))); 
}

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


struct splitmix_generator_nwise {
  uint64_t state;

  inline uint64_t operator()() {
    uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return broacast_bits(z ^ (z >> 31));
  }
};

template <class generator>
__attribute__((noinline)) void populate(uint64_t *out, size_t N,
                                        uint64_t seed) {
  generator g;
  g.state = seed;
  for (size_t i = 0; i < N; i++) {
    out[i] = g();
  }
}

volatile uint64_t hole{0};

template <class generator>
__attribute__((noinline)) std::pair<double, double>
time_populate(uint64_t *out, size_t N, size_t trials) {
  double min = 1e300;
  double avg = 0;
  for (size_t i = 0; i < trials; i++) {
    timeval beg, end;
    gettimeofday(&beg, NULL);
    populate<generator>(out, N, 12345 + i);
    hole = out[N - 1];
    gettimeofday(&end, NULL);
    double time =
        (1e6 * (end.tv_sec - beg.tv_sec) + (end.tv_usec - beg.tv_usec)) * 1000 /
        N;
    avg += time;
    min = min < time ? min : time;
  }
  avg /= trials;
  return std::make_pair(min, avg);
}

void print(std::pair<double, double> val) {
  std::cout << std::setprecision(4);
  std::cout << val.first << " ns/value ("
            << (val.second - val.first) / val.first * 100.0 << " %) \n";
}

using namespace std;
int main(void) {
  constexpr size_t N = 20000;
  size_t trials = 40000;
  uint64_t recipient[N];
  std::cout << "buffer size " << N * sizeof(uint64_t) / 1024 << " kB"
            << std::endl;
  for (size_t ti = 0; ti < 4; ti++) {
    std::cout << "splitmix ";
    print(time_populate<splitmix_generator>(recipient, N, trials));
    std::cout << "splitmix 5-wise ";
    print(time_populate<splitmix_generator_nwise>(recipient, N, trials));
    std::cout << "===========\n";
  }
  return EXIT_SUCCESS;
}
