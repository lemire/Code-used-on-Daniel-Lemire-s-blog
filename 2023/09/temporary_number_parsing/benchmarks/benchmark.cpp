
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

#include "parse_integer.h"

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

int main(int argc, char **argv) {
  std::vector<std::string> input;
  size_t numbers = 10000;
  size_t volume = 0;
  volatile uint64_t sum = 0;
  for (size_t i = 0; i < numbers; i++) {
    input.push_back(std::to_string(rand()));
    volume += input.back().size();
  }
  pretty_print(numbers, volume, "parse_unsigned_avx512",
               bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   sum += *parse_unsigned_avx512(
                       s.data(), s.data() + s.size());
                 }
               }));

  pretty_print(numbers, volume, "std::from_chars", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   uint64_t val;

                   auto [ptr, ec] =
                       std::from_chars(s.data(), s.data() + s.size(), val);

                   if (ec == std::errc()) {
                     sum += val;
                   }
                 }
               }));
  pretty_print(numbers, volume, "parse_unsigned", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   sum += *parse_unsigned(s.c_str());
                 }
               }));

  pretty_print(
      numbers, volume, "parse_unsigned_bounded", bench([&input, &sum]() {
        for (const std::string &s : input) {
          sum += *parse_unsigned_bounded(s.data(), s.data() + s.size());
        }
      }));
}
