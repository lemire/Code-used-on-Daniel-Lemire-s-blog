
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
  const size_t N = 128;
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis;

  std::vector<uint64_t> vector1;
  std::vector<uint64_t> vector2;
  auto generator = [&dis, &gen]() { return dis(gen); };

  // Generate random values and append them to the vector
  std::generate_n(std::back_inserter(vector1), N, generator);
  std::generate_n(std::back_inserter(vector2), N, generator);
  size_t total = 0;
  size_t coprime = 0;
  uint64_t largest = 0;
  for (uint64_t x : vector1) {
    for (uint64_t y : vector2) {
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
