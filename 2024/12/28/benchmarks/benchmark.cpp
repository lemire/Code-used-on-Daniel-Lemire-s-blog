
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>
#include <fstream>
#include <iostream>

#include "utf16fixer.h"

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
    printf(" %5.2f ns ", agg.fastest_elapsed_ns());

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
  std::u16string data(10000000, u' ');
  size_t volume = data.size();
  uint64_t counter = 0;
  volatile uint64_t sink = 0;
  pretty_print(data.size(), volume, "replace_invalid_utf16_neon",
               bench([&data,&sink]() {
                 replace_invalid_utf16_neon(data.data(), data.size());
                 sink = sink + data[0];
               }));
  pretty_print(data.size(), volume, "replace_invalid_utf16",
               bench([&data,&sink]() {
                replace_invalid_utf16(data.data(), data.size());
                 sink = sink + data[0];

               }));

}
