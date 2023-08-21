
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <vector>
#include <filesystem>

extern "C" {
#include "latin1_to_utf8.h"
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-30s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.1f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Please provide a file argument." << std::endl;
    std::cerr << "We recommend getting the file https://github.com/lemire/unicode_lipsum/raw/main/wikipedia_mars/french.latin1.txt "<< std::endl;
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
  printf("input = %zu bytes\n", input_data.size());
  std::vector<char> output;
  output.resize(input_data.size()*2);
  size_t characters = latin1_to_utf8(input_data.data(), input_data.size(), output.data());

  printf("characters = %zu\n", characters);

  volatile uint64_t sum{};

  pretty_print(input_data.size(), characters, "latin1_to_utf8_avx512_my_branch1",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8_avx512_my_branch1(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  pretty_print(input_data.size(), characters, "latin1_to_utf8_avx512_my_branch0",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8_avx512_my_branch0(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  pretty_print(input_data.size(), characters, "latin1_to_utf8_avx512_my_nobranch",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8_avx512_my_nobranch(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  pretty_print(input_data.size(), characters, "latin1_to_utf8_avx512_InstLatX64",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8_avx512_InstLatX64(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  pretty_print(input_data.size(), characters, "latin1_to_utf8_avx512",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8_avx512(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  pretty_print(input_data.size(), characters, "latin1_to_utf8",
               bench([&input_data, &output, &sum, &characters]() {
                   size_t r = latin1_to_utf8(input_data.data(), input_data.size(), output.data());
                   sum += r;
               }));
  }
