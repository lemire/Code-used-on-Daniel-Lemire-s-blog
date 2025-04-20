
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
#include "sumvalues.h"
#include <vector>

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-45s : ", name.c_str());
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

std::map<std::string, uint64_t> generate_large_map(size_t count = 1'000'000) {
  std::map<std::string, uint64_t> result;

  // Random number generator for values
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dist;

  // Generate 1,000,000 unique keys
  for (size_t i = 0; i < count; ++i) {
    // Create unique key: "key_0000000" to "key_9999999"
    std::string key = "key"s + std::to_string(i);
    // Insert key-value pair with random uint64_t value
    result.emplace(key, dist(gen));
  }
  return result;
}

int main(int argc, char **argv) {
  auto data = generate_large_map();
  std::unordered_map<std::string, uint64_t> udata(data.begin(), data.end());
  if (data.size() != udata.size()) {
    throw std::runtime_error("unexpected mismatch.");
  }
  size_t volume = data.size();
  volatile uint64_t counter = 0;
  for (size_t i = 0; i < 4; i++) {
    printf("Run %zu\n", i + 1);
    pretty_print(data.size(), volume, "sum_values", bench([&data, &counter]() {
                   counter = counter + sum_values(data);
                 }));
    pretty_print(data.size(), volume, "sum_values_daniel",
                 bench([&data, &counter]() {
                   counter = counter + sum_values_daniel(data);
                 }));
    pretty_print(data.size(), volume, "sum_values_cpp11",
                 bench([&data, &counter]() {
                   counter = counter + sum_values_cpp11(data);
                 }));
    pretty_print(
        data.size(), volume, "sum_values (unordered)",
        bench([&udata, &counter]() { counter = counter + sum_values(udata); }));
    pretty_print(data.size(), volume, "sum_values_daniel (unordered)",
                 bench([&udata, &counter]() {
                   counter = counter + sum_values_daniel(udata);
                 }));
    pretty_print(data.size(), volume, "sum_values_cpp11 (unordered)",
                 bench([&udata, &counter]() {
                   counter = counter + sum_values_cpp11(udata);
                 }));

    pretty_print(data.size(), volume, "count_keys_with_prefix",
                 bench([&data, &counter]() {
                   counter = counter + count_keys_with_prefix(data, "key_");
                 }));
    pretty_print(data.size(), volume, "count_keys_with_prefix_daniel",
                 bench([&data, &counter]() {
                   counter =
                       counter + count_keys_with_prefix_daniel(data, "key_");
                 }));
    pretty_print(data.size(), volume, "count_keys_with_prefix_cpp11",
                 bench([&data, &counter]() {
                   counter =
                       counter + count_keys_with_prefix_cpp11(data, "key_");
                 }));
    pretty_print(data.size(), volume, "count_keys_with_prefix (unordered)",
                 bench([&udata, &counter]() {
                   counter = counter + count_keys_with_prefix(udata, "key_");
                 }));
    pretty_print(
        data.size(), volume, "count_keys_with_prefix_daniel (unordered)",
        bench([&udata, &counter]() {
          counter = counter + count_keys_with_prefix_daniel(udata, "key_");
        }));
    pretty_print(data.size(), volume, "count_keys_with_prefix (unordered)",
                 bench([&udata, &counter]() {
                   counter = counter + count_keys_with_prefix(udata, "key_");
                 }));
  }
}
