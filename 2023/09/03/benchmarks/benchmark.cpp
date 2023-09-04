
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <vector>
#include <filesystem>
#include <iostream>

extern "C" {
#include "identifiers.h"
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
  if (argc < 2) {
    std::cerr << "Please provide a file argument." << std::endl;
    std::cerr << "You may want to try 'input.txt'." << std::endl;
    return EXIT_FAILURE;
  }
  const char *filename = argv[1];
  printf("# loading file %s\n", filename);
  std::filesystem::path path(filename);
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(path);
  std::vector<char> input_data;
  input_data.assign(std::istreambuf_iterator<char>(file),
                    std::istreambuf_iterator<char>());
  volatile uint64_t sum{};
  size_t identifiers = count_identifiers(input_data.data(), input_data.size());
  printf("%zu identifiers\n", identifiers);
  printf("%zu bytes\n", input_data.size());

  pretty_print(identifiers, input_data.size(), "count_identifiers_neon",
               bench([&input_data, &sum, &identifiers]() {
                   size_t r = count_identifiers_neon(input_data.data(), input_data.size());
                   sum += r;
                   if(r != identifiers) {
                       printf("FAIL %zu %zu\n", r, identifiers);
                       exit(1);
                   }
               }));
  pretty_print(identifiers, input_data.size(),"count_identifiers_neon_blob",
               bench([&input_data, &sum, &identifiers]() {
                   size_t r = count_identifiers_neon_blob(input_data.data(), input_data.size());
                   sum += r;
                   if(r != identifiers) {
                       printf("FAIL %zu %zu\n", r, identifiers);
                       exit(1);
                   }
               }));
  pretty_print(identifiers, input_data.size(),"count_identifiers_neon_strager",
               bench([&input_data, &sum, &identifiers]() {
                   size_t r = count_identifiers_neon_strager(input_data.data(), input_data.size());
                   sum += r;
                   if(r != identifiers) {
                       printf("FAIL %zu %zu\n", r, identifiers);
                       exit(1);
                   }
               }));
  pretty_print(identifiers, input_data.size(),"count_identifiers_neon_strager_ranges",
               bench([&input_data, &sum, &identifiers]() {
                   size_t r = count_identifiers_neon_strager_ranges(input_data.data(), input_data.size());
                   sum += r;
                   if(r != identifiers) {
                       printf("FAIL %zu %zu\n", r, identifiers);
                       exit(1);
                   }
               }));
  pretty_print(identifiers, input_data.size(),"count_identifiers",
               bench([&input_data, &sum, &identifiers]() {
                   size_t r = count_identifiers(input_data.data(), input_data.size());
                   sum += r;
                   if(r != identifiers) {
                       printf("FAIL\n");
                       exit(1);
                   }
               }));
}
