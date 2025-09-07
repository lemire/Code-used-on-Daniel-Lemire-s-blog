#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "benchmarker.h"
#include "counters/event_counter.h"

#include "avx2.h"
#include "avx512vbmi2.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "scalar.h"
#include "ssse3.h"

double pretty_print(const std::string &name, size_t num_values,
                    std::pair<event_aggregate, size_t> result) {
  const auto &agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  fmt::print("{:<50} : ", name);
  fmt::print(" {:5.3f} ns ", agg.fastest_elapsed_ns() / num_values);
  fmt::print(" {:5.2f} GB/s ", num_values / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    fmt::print(" {:5.2f} GHz ", agg.cycles() / agg.elapsed_ns());
    fmt::print(" {:5.2f} c ", agg.fastest_cycles() / num_values);
    fmt::print(" {:5.2f} i ", agg.fastest_instructions() / num_values);
    fmt::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / agg.fastest_cycles());
  }
  fmt::print("\n");
  return num_values / agg.fastest_elapsed_ns();
}

std::string generate_random_ascii_string(size_t length) {
  const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  const size_t max_index = sizeof(charset) - 1;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, max_index - 1);

  std::string result;
  result.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    result += charset[dis(gen)];
  }
  return result;
}

void run(size_t input_size) {
  std::string input = generate_random_ascii_string(input_size);
  std::string output(2 * input_size, '\0');

  for (size_t K : {64, 72}) {
    fmt::println("K = {}\n", K);

    pretty_print("scalar", input.size(), bench([&input, &output, K]() {
                   insert_line_feed(input.data(), input.size(), K,
                                    output.data());
                 }));
    pretty_print("scalar-memcpy", input.size(), bench([&input, &output, K]() {
                   insert_line_feed_memcpy(input.data(), input.size(), K,
                                           output.data());
                 }));
    pretty_print("scalar-memcpy-offline", input.size(), bench([&input, &output, K]() {
                   insert_line_feed_memcpy_offline(input.data(), input.size(), K,
                                           output.data());
                 }));
#ifdef HAVE16
    pretty_print("ssse3", input.size(), bench([&input, &output, K]() {
                   insert_line_feed16(input.data(), input.size(), K,
                                      output.data());
                 }));
#endif

#ifdef HAVE32
    pretty_print("avx2", input.size(), bench([&input, &output, K]() {
                   insert_line_feed32(input.data(), input.size(), K,
                                      output.data());
                 }));
#endif
#ifdef HAVE64
    pretty_print("avx512vbmi2", input.size(), bench([&input, &output, K]() {
                   insert_line_feed64(input.data(), input.size(), K,
                                      output.data());
                 }));
#endif
    fmt::println("");
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  run(1000000);
  return EXIT_SUCCESS;
}
