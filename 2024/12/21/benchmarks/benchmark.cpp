
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>
#include <fstream>
#include <iostream>

#include "csvparser.h"

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

std::string load_file(const std::string filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    file.seekg(0, std::ios::end);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(size, '\0');
    file.read(&content[0], size);

    if (!file) {
        throw std::runtime_error("Error reading file: " + filename);
    }
    file.close();

    return content;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  std::string data = load_file(argv[1]);
  size_t volume = data.size();
  uint64_t counter = 0;
  pretty_print(data.size(), volume, "get_column_old",
               bench([&data, &counter]() {
                 for (std::string_view  row: get_column_old(data, 1)) {
                   counter += row.size();
                 }
               }));
  pretty_print(data.size(), volume, "get_column_cxx20",
               bench([&data, &counter]() {
                 for (std::string_view  row: get_column_cxx20(data, 1)) {
                   counter += row.size();
                 }
               }));
}
