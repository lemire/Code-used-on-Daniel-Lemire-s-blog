#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <format>
#include <print>
#include <cstdint>

#include "benchmarker.h"
#include "counters/event_counter.h"
#include "search.h"
#include <unordered_map>



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


void collect_benchmark_results(size_t input_size) {
  // Benchmark for at_index and at_index_cache
  std::unordered_map<std::string, size_t> index_map;
  for (size_t i = 0; i < input_size; ++i) {
    index_map["key" + std::to_string(i)] = i;
  }

  volatile size_t counter = 0;

  auto bench_at_index = [&index_map, &counter, &input_size]() {
    size_t sum = 0;
    for (size_t i = 0; i < input_size; ++i) {
      sum += at_index(index_map, i);
    }
    counter += sum;
  };
  pretty_print("at_index", input_size, bench(bench_at_index));

  auto bench_at_index_cache = [&index_map, &counter, &input_size]() {
    size_t sum = 0;
    for (size_t i = 0; i < input_size; ++i) {
      sum += at_index_cache(index_map, i);
    }
    counter += sum;
  };
  pretty_print("at_index_cache", input_size, bench(bench_at_index_cache));

  auto bench_at_index_thread_local_cache = [&index_map, &counter, &input_size]() {
    size_t sum = 0;
    for (size_t i = 0; i < input_size; ++i) {
      sum += at_index_thread_local_cache(index_map, i);
    }
    counter += sum;
  };
  pretty_print("at_index_thread_local_cache", input_size, bench(bench_at_index_thread_local_cache));


  auto bench_ideal = [&index_map, &counter, &input_size]() {
    size_t sum = 0;
    for (auto const& pair : index_map) {
      sum += pair.second;
    }
    counter += sum;
  };
  pretty_print("ideal", input_size, bench(bench_ideal));

}


int main(int argc, char **argv) {
  collect_benchmark_results(1024);
}
