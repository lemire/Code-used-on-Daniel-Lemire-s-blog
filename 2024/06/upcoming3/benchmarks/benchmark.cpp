
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>

#include "vectorclassification.h"
std::string load_file_content(std::string_view filename) {
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
  size_t volume = data.size();
  size_t size = 1;
  volatile uint64_t count = 0;
  pretty_print(size, volume, "NaiveAdvanceString", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   NaiveAdvanceString(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceString", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceString(start, end);
                   if (start < end)
                     start++;
                 }
               }));
  pretty_print(size, volume, "AdvanceStringTable", bench([&data, &count]() {
                 const char *start = data.data();
                 const char *end = start + data.size();
                 while (start < end) {
                   count = *start;
                   AdvanceStringTable(start, end);
                   if (start < end)
                     start++;
                 }
               }));
}
