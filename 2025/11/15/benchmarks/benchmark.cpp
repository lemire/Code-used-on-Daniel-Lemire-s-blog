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
#include "simdutf.h"

std::u16string generate_random_utf16_string(size_t length) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(0, 0xFFFF);
  std::u16string result;
  result.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    uint32_t code_point;
    do {
      code_point = dist(gen);
    } while (code_point >= 0xD800 && code_point <= 0xDFFF); // Exclude surrogates
    result.push_back(static_cast<char16_t>(code_point));
  }
  return result;
}

double pretty_print(const std::string &name, size_t num_values,
                    size_t volume,
                    std::pair<event_aggregate, size_t> result) {
  const auto &agg = result.first;
  size_t N = result.second;
  num_values *= N; // Adjust num_values to account for repetitions
  std::print("{:<50} : ", name);
  std::print(" {:5.3f} ns ",
             agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:5.2f} GB/s ",
             double(volume * N) / agg.fastest_elapsed_ns());
  if (collector.has_events()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:5.2f} c/B ", agg.fastest_cycles() / double(volume * N));
    std::print(" {:5.2f} ins/B ", agg.fastest_instructions() / double(volume * N));
    std::print(" {:5.2f} i/c ", agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}


void collect_benchmark_results(size_t input_size, size_t number_strings) {
  // Generate many strings of varying content, including leading/trailing spaces
  std::vector<std::vector<char16_t>> strings;
  strings.reserve(number_strings);
  size_t volume = 0;
  // Create strings with varying lengths up to input_size
  for (size_t i = 0; i < number_strings; ++i) {
    auto str = generate_random_utf16_string(input_size);
    strings.push_back(std::vector<char16_t>(str.begin(), str.end()));
    volume += str.size() * sizeof(char16_t);
  }

  std::vector<char>  buffer(input_size * 4); // Sufficiently large buffer


  auto simdutf_conversion = [&strings, &buffer]() {
    for (const auto &str : strings) {
      size_t utf8_length = simdutf::utf8_length_from_utf16(str.data(), str.size());
      if(buffer.size() < utf8_length) {
        buffer.resize(utf8_length);
      }
      size_t converted_length = simdutf::convert_utf16_to_utf8(str.data(), str.size(), buffer.data());
      if(converted_length != utf8_length) {
        throw std::runtime_error("The lengths do not match which indicates that the string conversion failed. Invalid UTF-16 input?");
      }
    }
  };
  pretty_print("UTF-16 to UTF-8", number_strings, volume, bench(simdutf_conversion));

}


int main(int argc, char **argv) {
  collect_benchmark_results(1024*10, 100);
}
