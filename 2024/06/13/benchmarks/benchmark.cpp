
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>
#include <string>

#include "matrix_multiply.h"

void pretty_print(size_t, size_t doubles, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f Kvalue/s ", doubles * 1000 * 1000 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/value ", agg.fastest_elapsed_ns() / doubles);

  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/value ", agg.fastest_cycles() / doubles);
    printf(" %5.2f i/value ", agg.fastest_instructions() / doubles);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  size_t size = 100;
  size_t volume = 1;
  Matrix<double> a(size, size);
  Matrix<double> b(size, size);
  Matrix<double> c = allocate_multiply(a, b);
  pretty_print(size, volume, "multiply_ikj",
               bench([&]() { multiply_ikj(a, b, c); }));
  pretty_print(size, volume, "multiply_ijk",
               bench([&]() { multiply_ijk(a, b, c); }));
  pretty_print(size, volume, "multiply_kij",
               bench([&]() { multiply_kij(a, b, c); }));
  pretty_print(size, volume, "multiply_kji",
               bench([&]() { multiply_kji(a, b, c); }));
  pretty_print(size, volume, "multiply_jki",
               bench([&]() { multiply_jki(a, b, c); }));
  pretty_print(size, volume, "multiply_jik",
               bench([&]() { multiply_jik(a, b, c); }));

  pretty_print(size, volume, "multiply_ikj_safe",
               bench([&]() { multiply_ikj_safe(a, b, c); }));
  pretty_print(size, volume, "multiply_ijk_safe",
               bench([&]() { multiply_ijk_safe(a, b, c); }));
  pretty_print(size, volume, "multiply_kij_safe",
               bench([&]() { multiply_kij_safe(a, b, c); }));
  pretty_print(size, volume, "multiply_kji_safe",
               bench([&]() { multiply_kji_safe(a, b, c); }));
  pretty_print(size, volume, "multiply_jki_safe",
               bench([&]() { multiply_jki_safe(a, b, c); }));
  pretty_print(size, volume, "multiply_jik_safe",
               bench([&]() { multiply_jik_safe(a, b, c); }));
  return EXIT_SUCCESS;
}
