
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>

#include "escape.h"

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
  std::vector<std::string> input;

  std::ifstream fs("isaacs_files.txt");
  if (!fs) {
    std::cout << "Could not find isaacs_files.txt." << std::endl;
    return EXIT_FAILURE;
  }
  size_t volume = 0;
  for (std::string line; std::getline(fs, line);) {
    input.push_back(line);
    volume += line.size();
  }

  std::cout << "loaded " << input.size() << " URLs" << std::endl;
  std::cout << "volume " << volume << " bytes" << std::endl;

  size_t sum = 0;
  pretty_print(input.size(), volume, "find_string_escape", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   sum += find_string_escape(s).size();
                 }
               }));
  pretty_print(input.size(), volume, "string_escape", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   sum += string_escape(s).size();
                 }
               }));
  pretty_print(input.size(), volume, "stream_escape", bench([&input, &sum]() {
                 for (const std::string &s : input) {
                   sum += stream_escape(s).size();
                 }
               }));
}
