#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <numeric>
#include <print>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "counters/bench.h"
#include "mylib.h"

double pretty_print(const std::string &name, size_t num_values,
                    counters::event_aggregate agg) {
  std::print("{:<50} : ", name);
  std::print(" {:5.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} Gint/s ", double(num_values) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:5.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:5.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

__attribute__((noinline)) int sum_with_add_func(const std::vector<int>& numbers) {
  int sum = 0;
  for (int x : numbers) {
    sum = add(sum, x);
  }
  return sum;
}

__attribute__((noinline)) int sum_with_inline_add_func(const std::vector<int>& numbers) {
  int sum = 0;
  for (int x : numbers) {
    sum = inline_add(sum, x);
  }
  return sum;
}

__attribute__((noinline)) int sum_with_inline_add_no_vec_func(const std::vector<int>& numbers) {
  int sum = 0;
#pragma GCC novector
#pragma clang loop vectorize(disable)
  for (int x : numbers) {
    sum = inline_add(sum, x);
  }
  return sum;
}

__attribute__((noinline)) size_t count_spaces_func(const std::vector<std::string>& strings) {
  size_t total = 0;
  for (const auto& s : strings) {
    total += count_spaces(std::string_view(s));
  }
  return total;
}

__attribute__((noinline)) size_t count_spaces_inline_func(const std::vector<std::string>& strings) {
  size_t total = 0;
  for (const auto& s : strings) {
    total += count_spaces_inline(std::string_view(s));
  }
  return total;
}

void collect_benchmark_results(size_t num_elements) {
  // Create a large vector of ints
  std::vector<int> numbers(num_elements);
  std::iota(numbers.begin(), numbers.end(), 1); // Fill with 1, 2, 3, ..., num_elements

  volatile uint64_t counter = 0;

  auto sum_with_add = [&numbers, &counter]() {
    counter += sum_with_add_func(numbers);
  };
  pretty_print("sum_with_add", num_elements, counters::bench(sum_with_add));

  auto sum_with_inline_add = [&numbers, &counter]() {
    counter += sum_with_inline_add_func(numbers);
  };
  pretty_print("sum_with_inline_add", num_elements, counters::bench(sum_with_inline_add));

  auto sum_with_inline_add_no_vec = [&numbers, &counter]() {
    counter += sum_with_inline_add_no_vec_func(numbers);
  };
  pretty_print("sum_with_inline_add_no_vec", num_elements, counters::bench(sum_with_inline_add_no_vec));

  // Benchmark count_spaces on small strings
  std::vector<std::string> small_strings;
  small_strings.reserve(100000);
  for (size_t i = 0; i < 100000; ++i) {
    std::string s;
    size_t len = i % 6; // 0 to 5
    for (size_t j = 0; j < len; ++j) {
      s += (j % 2 == 0 ? ' ' : 'a');
    }
    small_strings.push_back(std::move(s));
  }

  auto count_spaces_small = [&small_strings, &counter]() {
    counter += count_spaces_func(small_strings);
  };
  pretty_print("count_spaces_small", small_strings.size(), counters::bench(count_spaces_small));

  auto count_spaces_inline_small = [&small_strings, &counter]() {
    counter += count_spaces_inline_func(small_strings);
  };
  pretty_print("count_spaces_inline_small", small_strings.size(), counters::bench(count_spaces_inline_small));

  // Benchmark count_spaces on large strings
  std::vector<std::string> large_strings;
  large_strings.reserve(1000);
  for (size_t i = 0; i < 1000; ++i) {
    std::string s(1024, 'a');
    for (size_t j = 0; j < 1024; j += 10) {
      s[j] = ' ';
    }
    large_strings.push_back(std::move(s));
  }

  auto count_spaces_large = [&large_strings, &counter]() {
    counter += count_spaces_func(large_strings);
  };
  pretty_print("count_spaces_large", large_strings.size(), counters::bench(count_spaces_large));

  auto count_spaces_inline_large = [&large_strings, &counter]() {
    counter += count_spaces_inline_func(large_strings);
  };
  pretty_print("count_spaces_inline_large", large_strings.size(), counters::bench(count_spaces_inline_large));
}

int main(int argc, char **argv) { collect_benchmark_results(1000000); }
