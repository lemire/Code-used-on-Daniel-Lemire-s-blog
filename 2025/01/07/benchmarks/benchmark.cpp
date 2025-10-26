
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>

#include "digitcount.h"

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.elapsed_ns());
  printf(" %5.2f ns/d ", agg.elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.cycles() / agg.elapsed_ns());
    printf(" %5.2f c/d ", agg.cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.cycles());
  }
  printf("\n");
}

void check() {
  if (digit_count(uint64_t(0ULL)) != alternative_digit_count_two_tables(0)) {
    printf("Error at %llu\n", 0ULL);
    return;
  }
  for (uint64_t i = 1; i <= 1000000000000000000ULL; i *= 10) {
    if (digit_count(i) != alternative_digit_count_two_tables(i)) {
      printf("Error at %llu\n", i);
      return;
    }
    if (digit_count(i + 1) != alternative_digit_count_two_tables(i + 1)) {
      printf("Error at %llu\n", i + 1);
      return;
    }
    if (digit_count(i) != alternative_digit_count_two_tables(i)) {
      printf("Error at %llu\n", i - 1);
      return;
    }
  }
  printf("All good\n");
}

int main(int argc, char **argv) {
  check();
  std::vector<uint64_t> data(100000);
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(
      0, (std::numeric_limits<uint64_t>::max)());
  std::vector<uint32_t> data32(100000);
  std::uniform_int_distribution<uint32_t> dis32(
      0, (std::numeric_limits<uint32_t>::max)());
  std::ranges::generate(data, [&]() { return dis(gen); });
  std::ranges::generate(data32, [&]() { return dis32(gen); });
  size_t volume = data.size();
  volatile uint64_t counter = 0;
  for (size_t i = 0; i < 4; i++) {
    printf("Run %zu\n", i + 1);
    pretty_print(data.size(), volume * sizeof(uint64_t), "digit_count", bench([&data, &counter]() {
                   size_t local_counter = 0;
                   for (auto v : data) {
                     local_counter = local_counter + digit_count(v);
                   }
                   counter = counter + local_counter;
                 }));
    pretty_print(data.size(), volume * sizeof(uint64_t),
                 "alternative_digit_count", bench([&data, &counter]() {
                   size_t local_counter = 0;
                   for (auto v : data) {
                     local_counter = local_counter + alternative_digit_count(v);
                   }
                   counter = counter + local_counter;
                 }));
    pretty_print(data.size(), volume * sizeof(uint64_t),
                 "alternative_digit_count_two_tables",
                 bench([&data, &counter]() {
                   size_t local_counter = 0;
                   for (auto v : data) {
                     local_counter = local_counter + alternative_digit_count_two_tables(v);
                   }
                   counter = counter + local_counter;
                 }));
    pretty_print(data.size(), volume * sizeof(uint32_t), "digit_count 32",
                 bench([&data32, &counter]() {
                   size_t local_counter = 0;
                   for (auto v : data32) {
                     local_counter = local_counter + digit_count(v);
                   }
                   counter = counter + local_counter;
                 }));
    pretty_print(data.size(), volume * sizeof(uint32_t),
                 "alternative_digit_count 32", bench([&data32, &counter]() {
                   size_t local_counter = 0;
                   for (auto v : data32) {
                     local_counter = local_counter + alternative_digit_count(v);
                   }
                   counter = counter + local_counter;
                 }));
  }
}
