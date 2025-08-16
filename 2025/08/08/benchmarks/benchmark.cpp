#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>
#include <random>

#include "counters/event_counter.h"
#include "benchmarker.h"

#include "simdutf.h"
#include "fmt/core.h"
#include "fmt/format.h"


double pretty_print(const std::string &name, size_t num_values,
                  std::pair<event_aggregate,size_t> result) {
  const auto& agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  fmt::print("{:<50} : ", name);
  fmt::print(" {:5.2f} ns ", agg.fastest_elapsed_ns()/num_values);
  fmt::print(" {:5.2f} GB/s ", num_values / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    fmt::print(" {:5.2f} GHz ",
               agg.cycles() / agg.elapsed_ns());
    fmt::print(" {:5.2f} c ", agg.fastest_cycles() / num_values);
    fmt::print(" {:5.2f} i ", agg.fastest_instructions() / num_values);
    fmt::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / agg.fastest_cycles());
  }
  fmt::print("\n");
  return num_values / agg.fastest_elapsed_ns();
}



std::string generate_random_ascii_string(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t max_index = sizeof(charset) - 1;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max_index - 1);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += charset[dis(gen)];
    }
    result += '='; // Add '=' at the end
    return result;
}


// Naive implementation of find
const char* naive_find(const char* start, const char* end, char character) {
    while (start != end) {
        if (*start == character) {
            return start;
        }
        ++start;
    }
    return end;
}


std::vector<std::tuple<size_t, double, double, double, double>> benchmark_results;

void collect_benchmark_results(size_t input_size) {
    std::string input = generate_random_ascii_string(input_size);
    volatile uint64_t counter = 0;

    // Benchmark strchr
    auto memchr_result = pretty_print("memchr", input.size(), 
                                   bench([&input, &counter]() {
                                       char* ptr = (char*)memchr(input.data(),'=',input.size());
                                       counter = counter + size_t(ptr - input.data());
                                   }));

    // Benchmark std::find
    auto std_result = pretty_print("std::find", input.size(), 
                                   bench([&input, &counter]() {
                                       auto it = std::find(input.data(), input.data() + input.size(), '=');
                                       counter = counter + size_t(it - input.data());
                                   }));

    // Benchmark simdutf::find
    auto simdutf_result = pretty_print("simdutf::find", input.size(), 
                                       bench([&input, &counter]() {
                                           auto it = simdutf::find(input.data(), input.data() + input.size(), '=');
                                           counter = counter + size_t(it - input.data());
                                       }));

    // Benchmark naive_find
    auto naive_result = pretty_print("naive_find", input.size(), 
                                      bench([&input, &counter]() {
                                          auto it = naive_find(input.data(), input.data() + input.size(), '=');
                                          counter = counter + size_t(it - input.data());
                                      }));

    benchmark_results.emplace_back(input_size, memchr_result, std_result, simdutf_result, naive_result);
}

void print_markdown_table() {
    fmt::print("| Input Size (bytes) | memchr (GB/s)    | std::find (GB/s) |  simdutf::find (GB/s) | naive_find (GB/s) |\n");
    fmt::print("|--------------------|------------------|------------------|-----------------------|-------------------|\n");

    for (const auto& [size, memchr_gbps, std_gbps, simdutf_gbps, naive_gbps] : benchmark_results) {
        fmt::print("| {:<18} | {:<16.2f} | {:<16.2f} | {:<21.2f} | {:<17.2f} |\n", size, memchr_gbps, std_gbps, simdutf_gbps, naive_gbps);
    }
}

int main(int argc, char **argv) {
    std::vector<size_t> input_sizes = {1024, 8192, 65536, 524288, 2097152}; // Sizes from cache-friendly to RAM-intensive

    for (size_t size : input_sizes) {
        collect_benchmark_results(size);
    }

    // Print markdown table
    print_markdown_table();
}
