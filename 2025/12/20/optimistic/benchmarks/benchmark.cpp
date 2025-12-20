#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <format>
#include <print>
#include <cstdint>

#include "simdutf.h"
#include "benchmarker.h"
#include "counters/event_counter.h"



double pretty_print(const std::string &name, size_t num_values,
                    std::pair<event_aggregate, size_t> result) {
  const auto &agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  std::print("{:<50} : ", name);
  std::print(" {:5.3f} ns/byte ",
             agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} GB/s ",
             double(num_values) / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
  std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
  std::print(" {:5.2f} c ", agg.fastest_cycles() / double(num_values));
  std::print(" {:5.2f} i ", agg.fastest_instructions() / double(num_values));
  std::print(" {:5.2f} i/c ", agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}


// count the number of strings containing at least one non-ASCII character
size_t optimistic(const std::vector<std::string> &strings) {
  size_t counter = 0;
  for (const auto &s : strings) {
    unsigned char result = 0;
    for (char c : s) {
        result |= static_cast<unsigned char>(c);
    }
    if(result > 0x7F) {
      counter += 1;
    }
  }
  return counter;
};

size_t simdutf_optimistic(const std::vector<std::string> &strings) {
  size_t counter = 0;
  for (const auto &s : strings) {
    counter += simdutf::validate_ascii(s.data(), s.size());
  }
  return counter;
};

// count the number of strings containing at least one non-ASCII character
size_t pessimistic(const std::vector<std::string> &strings) {
  size_t counter = 0;
  for (const auto &s : strings) {
    unsigned char result = 0;
    for (char c : s) {
      if(static_cast<unsigned char>(c) > 0x7F) {
        counter += 1;
        break;
      }
    }
  }
  return counter;
};


// count the number of strings containing at least one non-ASCII character
size_t simdutf_pessimistic(const std::vector<std::string> &strings) {
  size_t counter = 0;
  for (const auto &s : strings) {
    if(simdutf::validate_ascii_with_errors(s.data(), s.size()).error != simdutf::error_code::SUCCESS) {
      counter += 1;
    }
  }
  return counter;
};

void collect_benchmark_results(size_t input_size, size_t number_strings) {
  // Generate many strings of varying content, including leading/trailing spaces
  std::vector<std::string> strings;
  strings.reserve(number_strings);
  size_t volume = 0;
  // Create strings with varying lengths up to input_size
  for (size_t i = 0; i < number_strings; ++i) {
    size_t len = (i % input_size) + 1;
    std::string s;
    s.reserve(len + 10);
    // Fill with random printable characters and occasional spaces
    for (size_t k = 0; k < len; ++k) {
      char c = char('!' + (k + i) % 94);
      s.push_back(c);
    }
    volume += s.size();
    strings.push_back(std::move(s));
  }

  volatile uint64_t counter = 0;

  pretty_print("optimistic", volume, bench([&strings, &counter]() {  counter = optimistic(strings); }));
  pretty_print("simdutf_optimistic", volume, bench([&strings, &counter]() {  counter = simdutf_optimistic(strings); }));

  pretty_print("pessimistic", volume, bench([&strings, &counter]() {  counter = pessimistic(strings); }));
  pretty_print("simdutf_pessimistic", volume, bench([&strings, &counter]() {  counter = simdutf_pessimistic(strings); }));
}


int main(int argc, char **argv) {
  collect_benchmark_results(1024, 100000);
}
