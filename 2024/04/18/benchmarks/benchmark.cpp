
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>

#include "breakdown.h"

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

bool assert_me(bool condition) {
  if (!condition) {
    printf("assertion failed\n");
    exit(1);
  }
  return true;
}



int main(int argc, char **argv) {
  const size_t N = 131072;
  const size_t line_length = 72;

  std::vector<char> vector(N, 'a');
  std::vector<char> out;

  out.reserve(2*N); // plenty

  size_t volume = vector.size();
  pretty_print(1, volume, "copy",
               bench([&vector, &out]() {
                out.resize(vector.size() + vector.size()/line_length);
                break_lines(out.data(), vector.data(), N, line_length);
               }));
  pretty_print(1, volume, "inplace",
               bench([&vector]() {
                vector.resize(N + N/line_length);
                break_lines_inplace(vector.data(), N, line_length);
               }));
  return out.size();
}
