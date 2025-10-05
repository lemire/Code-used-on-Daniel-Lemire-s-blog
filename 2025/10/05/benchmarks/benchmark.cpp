#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <format>
#include <cstdint>

#include "benchmarker.h"
#include "counters/event_counter.h"
#include "trim.h"


double pretty_print(const std::string &name, size_t num_values,
                    std::pair<event_aggregate, size_t> result) {
  const auto &agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  std::print("{:<50} : ", name);
  std::print(" {:5.3f} ns ",
             agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} Gv/s ",
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


void collect_benchmark_results(size_t input_size, size_t number_strings) {
  // Generate many strings of varying content, including leading/trailing spaces
  std::vector<std::string> strings;
  strings.reserve(number_strings);
  // Create strings with varying lengths up to input_size
  for (size_t i = 0; i < number_strings; ++i) {
    size_t len = (i % input_size) + 1;
    std::string s;
    s.reserve(len + 10);
    // Add some leading spaces sometimes
    if (i % 3 == 0) s.append(i % 5, ' ');
    // Fill with random printable characters and occasional spaces
    for (size_t k = 0; k < len; ++k) {
      char c;
      if ((k + i) % 17 == 0) c = ' ';
      else c = char('!' + (k + i) % 94);
      s.push_back(c);
    }
    // Add some trailing spaces sometimes
    if (i % 5 == 0) s.append((i + 2) % 7, ' ');
    strings.push_back(std::move(s));
  }

  volatile uint64_t counter = 0;

  auto run_trim_classic = [&strings, &counter]() {
    for (const auto &str : strings) {
      auto v = trim_classic(std::string_view(str));
      counter += v.size();
    }
  };
  auto classic_result = pretty_print("trim_classic", number_strings, bench(run_trim_classic));

  auto run_trim_ranges = [&strings, &counter]() {
    for (const auto &str : strings) {
      auto v = trim_ranges(std::string_view(str));
      counter += v.size();
    }
  };
  auto ranges_result = pretty_print("trim_ranges", number_strings, bench(run_trim_ranges));

  auto run_trim_simple_ranges = [&strings, &counter]() {
    for (const auto &str : strings) {
      auto v = trim_simple_ranges(std::string_view(str));
      counter += v.size();
    }
  };

  auto simple_ranges_result = pretty_print("trim_simple_ranges", number_strings, bench(run_trim_simple_ranges));

}


int main(int argc, char **argv) {
  collect_benchmark_results(1024, 100000);
}
