
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#if __has_include(<generator.h>)
#include <generator>
#define GENERATOR 1
#endif
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <string_view>

std::string load_file_content(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return "";
  }

  std::string content;
  std::string line;
  while (std::getline(file, line)) {
    content += line + "\n";
  }
  file.close();
  return content;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  std::string data = load_file_content("data.html");
  data.resize(4000);
  size_t volume = data.size();
  size_t size = 1;
  volatile uint64_t count = 0;
  const char *targets = "<&\r\0";
  pretty_print(size, volume, "std", bench([&data, &count, targets]() {
                 count = 0;
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = count + *start;
                   start = std::find_first_of(start, end, targets,
                                              targets + strlen(targets));
                   if (start < end) {
                     start++;
                   }
                 }
               }));
#if GENERATOR
  auto target_finder = [](std::string_view data,
                          const char *targets) -> std::generator<const char *> {
    const char *start = data.data();
    const char *end = start + data.size();
    while (start < end) {
      start = std::find_first_of(start, end, targets,
                                 targets + std::strlen(targets));
      if (start == end) {
        co_return;
      }
      co_yield start;
      start++;
    }
  };
  pretty_print(size, volume, "generator",
               bench([&data, &count, targets, target_finder]() {
                 for (const char *match : target_finder(data, targets)) {
                   count = count + *match;
                 };
               }));
#endif
  pretty_print(size, volume, "filter", bench([&data, &count, targets]() {
                 auto matched_characters =
                     data | std::views::filter([](char c) {
                       return c == '<' | c == '&' | c == '\r' | c == '\0';
                     });
                 for (const char &c : matched_characters) {
                   count = count + c;
                 };
               }));
  return EXIT_SUCCESS;
}
