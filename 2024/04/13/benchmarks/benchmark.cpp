
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>

#include "gcd.h"

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

bool assert_me(bool condition) {
  if (!condition) {
    printf("assertion failed\n");
    exit(1);
  }
  return true;
}

int main(int argc, char **argv) {
  const size_t N = 2048;
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max() / 16);

  std::vector<uint64_t> vector1;
  std::vector<uint64_t> vector2;
  auto generator = [&dis, &gen]() { return dis(gen); };

  // Add some example data
  std::vector<std::pair<uint64_t, uint64_t>> example_data {
    {212, 31},
    {1100087778366101931, 679891637638612258} // 88th and 87th Fibonacci number
  };
  for (auto [x, y] : example_data) {
    auto b = extended_gcd(x, y);
    assert_me(b.x * x + b.y * y == b.gcd);
    auto be = binary_extended_gcd(x, y);
    assert_me(be.x * x + be.y * y == be.gcd);
    assert_me(b.gcd == be.gcd);
  }

  // Generate random values and append them to the vector
  std::generate_n(std::back_inserter(vector1), N, generator);
  std::generate_n(std::back_inserter(vector2), N, generator);
  size_t total = 0;
  size_t coprime = 0;
  uint64_t largest = 0;
  bool is_binary_extended_correct = true;
  bool is_extended_correct = true;

  for (uint64_t x : vector1) {
    for (uint64_t y : vector2) {

      auto be1 = extended_gcd(x, y);
      assert_me(be1.x * x + be1.y * y == be1.gcd);
      if((be1.x * x + be1.y * y != be1.gcd) && is_extended_correct) {
        is_extended_correct = false;
        printf("# extended_gcd is incorrect\n");
        printf("#   x: %llu, y: %llu\n", (long long unsigned)x, (long long unsigned)y);
        printf("#   bezout.x: %llu, bezout.y: %llu, gcd: %llu\n", (long long unsigned)(be1.x), (long long unsigned)(be1.y), (long long unsigned)(be1.gcd));
      }
      auto be2 = binary_extended_gcd(x, y);
      if((be2.x * x + be2.y * y != be2.gcd) && is_binary_extended_correct) {
        is_binary_extended_correct = false;
        printf("# binary_extended_gcd is incorrect\n");
        printf("#   x: %llu, y: %llu\n", (long long unsigned)x, (long long unsigned)y);
        printf("#   bezout.x: %llu, bezout.y: %llu, gcd: %llu\n", (long long unsigned)(be2.x), (long long unsigned)(be2.y), (long long unsigned)(be2.gcd));
      }
      assert_me(binary_extended_gcd(x, y).gcd == extended_gcd(x, y).gcd);
      assert_me(std::gcd(x, y) == extended_gcd(x, y).gcd);
      assert_me(std::gcd(x, y) == binary_gcd(x, y));
      assert_me(binary_gcd_noswap(x, y) == binary_gcd(x, y));
      if(extended_gcd(x, y).gcd > largest) {
        largest = extended_gcd(x, y).gcd;
      }
      if(extended_gcd(x, y).gcd == 1) {
        coprime++;
      }
      total++;
    }
  }
  printf("coprime ratio: %f\n", (double)coprime / total);
  printf("largest gcd: %lu\n", (unsigned long)largest);

  size_t volume = (N - 1) * (N - 1);
  volatile uint64_t counter = 0;
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "extended_gcd",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + extended_gcd(x, y).gcd;
                   }
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "binary_extended_gcd",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + binary_extended_gcd(x, y).gcd;
                   }
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "std::gcd",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + std::gcd(x, y);
                   }
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "binary_gcd",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + binary_gcd(x, y);
                   }
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "binary_gcd_noswap",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + binary_gcd_noswap(x, y);
                   }
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t) * 2, "naive_gcd",
               bench([&counter, &vector1, &vector2]() {
                 for (uint64_t x : vector1) {
                   for (uint64_t y : vector2) {
                     counter = counter + naive_gcd(x, y);
                   }
                 }
               }));
}
