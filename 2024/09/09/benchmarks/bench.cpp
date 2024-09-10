
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <bit>
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
  pretty_print(volume, volume * sizeof(uint64_t), "precompute_string_really_fast",
               bench([]() { precompute_string_really_fast(); }));
  pretty_print(volume, volume * sizeof(uint64_t), "precompute_string_fast_slim",
               bench([]() { precompute_string_fast_slim(); }));
}

int fast_offset(uint32_t x) {
  // x should be in [0, 65536].
  static uint64_t table[] = {4294967296,  8589934582,  8589934582,  8589934582,
                             12884901788, 12884901788, 12884901788, 17179868184,
                             17179868184, 17179868184, 21474826480, 21474826480,
                             21474826480, 21474826480, 25769703776, 25769703776,
                             25769703776};
  uint64_t digits =
      1 +
      ((x + table[31 - std::countl_zero(static_cast<uint32_t>(x) | 1)]) >> 32);
  static uint64_t offsets[] = {0, 0, 0, 10, 110, 1110, 11110};
  return x * digits - offsets[digits];
}

std::pair<uint64_t,uint64_t> fast_offset_size(uint32_t x) {
  // x should be in [0, 65536].
  static uint64_t table[] = {4294967296,  8589934582,  8589934582,  8589934582,
                             12884901788, 12884901788, 12884901788, 17179868184,
                             17179868184, 17179868184, 21474826480, 21474826480,
                             21474826480, 21474826480, 25769703776, 25769703776,
                             25769703776};
  uint64_t digits =
      1 +
      ((x + table[31 - std::countl_zero(static_cast<uint32_t>(x) | 1)]) >> 32);
  static uint64_t offsets[] = {0, 0, 0, 10, 110, 1110, 11110};
  return {x * digits - offsets[digits], digits};
}
void bench_query() {
  size_t volume = 1 << 16;
  auto simple_table = precompute_string();
  auto [fast_table, offsets] = precompute_string_fast();
  auto [fast_table_r, offsets_r] = precompute_string_really_fast();
  for(size_t i = 0; i < fast_table_r.size(); i++) {
    if(fast_table[i] != fast_table_r[i]) {
      std::cerr << "String Error: " << i << " '" << fast_table[i] << "' '" << fast_table_r[i] << "'" << std::endl;
      abort();
    }
  }
  for(size_t i = 0; i < offsets_r.size(); i++) {
    if(offsets[i] != offsets_r[i]) {
      std::cerr << "Offset Error: " << i << " " << offsets[i] << " " << offsets_r[i] << std::endl;
      abort();
    }
  }
  for (size_t i = 0; i <= volume; i++) {
    if (fast_offset(i) != offsets[i]) {
      std::cerr << "Error: " << i << " " << fast_offset(i) << " " << offsets[i]
                << std::endl;
      abort();
    }
  }

  auto GetCodeFast = [&fast_table, &offsets](uint16_t index) {
    return std::string_view(&fast_table[offsets[index]],
                            offsets[index + 1] - offsets[index]);
  };
  auto GetCodeFastSlim = [&fast_table](uint16_t index) {
    auto [offset,size] = fast_offset_size(index);
    return std::string_view(&fast_table[offset], size);
  };
  auto GetCodeSimple = [&simple_table](uint16_t index) {
    return simple_table[index];
  };
  for(size_t i = 0; i < (1<<16); i++) {
    if(GetCodeFastSlim(i) != GetCodeSimple(i)) {
      std::cerr << "Error: " << i << " " << GetCodeFast(i) << " " << GetCodeFastSlim(i) << std::endl;
      abort();
    }
  }

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
  pretty_print(volume, volume * sizeof(uint64_t), "GetCodeFastSlim",
               bench([&GetCodeFastSlim, &b, &vec]() {
                 for (auto &num : vec) {
                   b = b + GetCodeFastSlim(num).size();
                 }
               }));
}

int main(int, char **) {
  bench_query();
  bench();
  return EXIT_SUCCESS;
}
