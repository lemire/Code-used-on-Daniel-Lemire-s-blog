
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string_view>
#include <vector>

#include "precomptables.h"

double pretty_print(size_t volume, size_t bytes, std::string name,
                    event_aggregate agg) {
  printf("\t%-45s: %5.3f ms %5.1f GB/s %5.1f billion floats/s \n ",
         name.c_str(), agg.elapsed_ns() / 1000000, bytes / agg.elapsed_ns(),
         volume / agg.elapsed_ns());
  return agg.elapsed_ns() / volume;
}

void bench() {
  size_t volume = 1 << 16;
  size_t bytes = 382106;
  pretty_print(volume, bytes, "precompute_string",
               bench([]() { precompute_string(); }));
  pretty_print(volume, volume * sizeof(uint64_t), "precompute_string_fast",
               bench([]() { precompute_string_fast(); }));
}

void bench_query() {
  size_t volume = 1 << 16;
  auto simple_table = precompute_string();
  auto [fast_table, offsets] = precompute_string_fast();
  auto GetCodeFast = [&fast_table, &offsets](uint16_t index) {
    return std::string_view(&fast_table[offsets[index]],
                            offsets[index + 1] - offsets[index]);
  };
  auto GetCodeSimple = [&simple_table](uint16_t index) {
    return simple_table[index];
  };

  std::vector<uint16_t> vec(volume);
  std::random_device rd;
  std::mt19937 gen(rd()); // Mersenne Twister 19937 generator
  std::uniform_int_distribution<uint16_t> dis(
      std::numeric_limits<uint16_t>::min(),
      std::numeric_limits<uint16_t>::max());
  size_t bytes = 0;
  for (auto &num : vec) {
    num = dis(gen);
    bytes += GetCodeFast(num).size();
  }
  volatile size_t b = 0;

  pretty_print(volume, bytes, "GetCode", bench([&GetCodeSimple, &b, &vec]() {
                 for (auto &num : vec) {
                   b = b + GetCodeSimple(num).size();
                 }
               }));
  pretty_print(volume, volume * sizeof(uint64_t), "GetCodeFast",
               bench([&GetCodeFast, &b, &vec]() {
                 for (auto &num : vec) {
                   b = b + GetCodeFast(num).size();
                 }
               }));
}

int main(int, char **) {
  bench_query();
  bench();
  return EXIT_SUCCESS;
}
