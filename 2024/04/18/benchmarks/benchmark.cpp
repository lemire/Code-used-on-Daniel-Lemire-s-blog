
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

size_t process(const size_t N) {
  const size_t line_length = 72;

  std::vector<char> vector(N, 'a');
  std::vector<char> out;

  out.reserve(2*N); // plenty

  size_t volume = vector.size();
  pretty_print(1, volume, "copy",
               bench([&vector, &out, N]() {
                out.resize(N + N/line_length);
                break_lines(out.data(), vector.data(), N, line_length);
               }));
  pretty_print(1, volume, "copy template",
               bench([&vector, &out, N]() {
                out.resize(N + N/line_length);
                break_lines_template<72>(out.data(), vector.data(), N);
               }));
  pretty_print(1, volume, "copy (no inline) ",
               bench([&vector, &out, N]() {
                out.resize(N + N/line_length);
                break_lines_no_inline(out.data(), vector.data(), N, line_length);
               }));
  pretty_print(1, volume, "memcpy",
               bench([&vector, &out, N]() {
                memcpy(out.data(), vector.data(), N);
               }));
  pretty_print(1, volume, "inplace",
               bench([&vector, N]() {
                vector.resize(N + N/line_length);
                break_lines_inplace(vector.data(), N, line_length);
               }));
  pretty_print(1, volume, "inplace (no inline)",
               bench([&vector, N]() {
                vector.resize(N + N/line_length);
                break_lines_inplace_no_inline(vector.data(), N, line_length);
               }));
  return out.size();
}

int main(int, char **) {
  for(size_t size = 8192; size <= 262144; size *= 2) {
    printf("size %zu\n", size);
    process(size);
  }

}
