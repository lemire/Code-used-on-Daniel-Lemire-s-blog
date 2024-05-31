
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>

#include "to_bitmap.h"

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
  size_t size = 5000;
  std::vector<std::vector<uint8_t>> indices(size, {0, 1, 2, 3, 4, 5, 6, 7, 14,15, 16, 17, 18, 19, 20, 21, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55});
  size_t volume = 0;
  for(auto & index : indices) {
    volume += index.size();
  }
  volatile uint64_t map = 0;
  pretty_print(size, volume, "to_bitmap",
               bench([&indices,&map]() {
                for(auto & index : indices) {
                  std::span<uint8_t> span(index);
                  map |= to_bitmap(span);
                }
               }));
  pretty_print(size, volume, "to_bitmap_simd",
               bench([&indices,&map]() {
                for(auto & index : indices) {
                  std::span<uint8_t> span(index);
                  map |= to_bitmap_simd(span);
                }
               }));
}
