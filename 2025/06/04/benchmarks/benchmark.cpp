
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <unordered_map>
using std::literals::string_literals::operator""s;
#include "champagne_lemire.h"
#include "dragonbox.h"

#include <vector>

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-50s : ", name.c_str());
  // printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  // printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    // printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    // printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

template <typename T> struct float_number_generator {
  virtual T new_float() = 0;
  virtual std::string describe() = 0;
  virtual ~float_number_generator() = default;
};
template <typename T> struct uniform_generator : float_number_generator<T> {
  std::random_device rd;
  std::mt19937_64 gen;
  std::uniform_real_distribution<T> dis;
  explicit uniform_generator(T a = 0.0, T b = 1.0)
      : rd(), gen(rd()), dis(a, b) {}
  std::string describe() override {
    return "generate random numbers uniformly in the interval [" +
           std::to_string((dis.min)()) + std::string(",") +
           std::to_string((dis.max)()) + std::string("]");
  }
  T new_float() override { return dis(gen); }
};

std::vector<IEEE754d> generate_large_set(size_t count = 10'000) {
  std::vector<IEEE754d> result;
  uniform_generator<double> gen(-1e10, 1e10);
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(decode_ieee754(gen.new_float()));
  }

  return result;
}

int main(int argc, char **argv) {
  auto data = generate_large_set();
  size_t volume = data.size();
  volatile uint64_t counter = 0;
  char buffer[64];
  for (size_t i = 0; i < 4; i++) {
    printf("Run %zu\n", i + 1);
    pretty_print(data.size(), volume, "champagne_lemire",
                 bench([&data, &counter, &buffer]() {
                   for (size_t i = 0; i < data.size(); ++i) {
                     counter =
                         counter + to_chars(data[i].mantissa, data[i].exponent,
                                            data[i].sign, buffer);
                   }
                 }));
    pretty_print(data.size(), volume, "dragonbox",
                 bench([&data, &counter, &buffer]() {
                   for (size_t i = 0; i < data.size(); ++i) {

                     char *start = buffer;
                     if (data[i].sign) {
                       buffer[0] = '-';
                       start++;
                     }
                     counter = counter +
                               (jkj::dragonbox::detail::to_chars(
                                    data[i].mantissa, data[i].exponent, start) -
                                buffer);
                   }
                 }));
  }
}

/**

int to_chars(T mantissa, int32_t exponent, bool sign, char *const result) {

       template <>
            char*
            to_chars(stdr::uint_least64_t const significand,
                                                             int exponent, char*
buffer) noexcept {

*/