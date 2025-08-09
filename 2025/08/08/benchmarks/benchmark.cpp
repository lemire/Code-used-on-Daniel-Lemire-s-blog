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


void pretty_print(const std::string &name, size_t num_values,
                  event_aggregate agg) {
  fmt::print("{:<50} : ", name);
  fmt::print(" {:5.2f} ns ", agg.fastest_elapsed_ns());
  fmt::print(" {:5.2f} GB/s ", num_values / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    fmt::print(" {:5.2f} GHz ",
               agg.fastest_cycles() / agg.fastest_elapsed_ns());
    fmt::print(" {:5.2f} c ", agg.fastest_cycles() / num_values);
    fmt::print(" {:5.2f} i ", agg.fastest_instructions() / num_values);
    fmt::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / agg.fastest_cycles());
  }
  fmt::print("\n");
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


int main(int argc, char **argv) {
   std::string input = generate_random_ascii_string(10000);
  size_t volume = input.size();
  volatile uint64_t counter = 0;
  for (size_t i = 0; i < 4; i++) {
    printf("Run %zu\n", i + 1);
    pretty_print("std::find",input.size(), 
                 bench([&input, &counter]() {
                  auto it = std::find(input.data(), input.data() + input.size(), '=');
                  counter = counter +  size_t(it - input.data()); 
                 }));

    pretty_print("simdutf::find",input.size(), 
                 bench([&input, &counter]() {
                  auto it = simdutf::find(input.data(), input.data() + input.size(), '=');
                  counter = counter + size_t(it - input.data()); 
                 }));
}
}
