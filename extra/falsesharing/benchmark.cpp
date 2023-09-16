
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <thread>
#include <vector>

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  /*if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }*/
  printf("\n");
}

template <int align> struct alignas(align) A {
  volatile uint64_t count;
};

constexpr size_t iterations = 100'000'000;

// compilers seemingly struggle to optimize this away
void counter(volatile uint64_t *counterpt) {
  for (size_t i = 0; i < iterations; i++) {
    *counterpt = 3 * *counterpt * *counterpt + 1;
  }
}
template <int align> uint64_t threaded_counter() {
  A<align> a[8];
  static_assert(sizeof(a) == 8 *align);
  a[0].count = 1;
  a[1].count = 2;
  a[2].count = 3;
  a[3].count = 4;
  a[4].count = 5;
  a[5].count = 6;
  a[6].count = 7;
  a[7].count = 8;

  std::thread t1(counter, &a[0].count);
  std::thread t2(counter, &a[1].count);
  std::thread t3(counter, &a[2].count);
  std::thread t4(counter, &a[3].count);
  std::thread t5(counter, &a[4].count);
  std::thread t6(counter, &a[5].count);
  std::thread t7(counter, &a[6].count);
  std::thread t8(counter, &a[7].count);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();
  return a[0].count + a[1].count + a[2].count + a[3].count + a[4].count +
         a[5].count + a[6].count + a[7].count;
}

int main(int argc, char **argv) {
  uint64_t counter = 0;

  pretty_print(8 *iterations, 8 *iterations * sizeof(uint64_t), "8-byte",
               bench([&counter]() { counter += threaded_counter<8>(); }));
  pretty_print(8 *iterations, 8 *iterations * sizeof(uint64_t), "16-byte",
               bench([&counter]() { counter += threaded_counter<16>(); }));
  pretty_print(8 *iterations, 8 *iterations * sizeof(uint64_t), "64-byte",
               bench([&counter]() { counter += threaded_counter<64>(); }));
  pretty_print(8 *iterations, 8 *iterations * sizeof(uint64_t), "128-byte",
               bench([&counter]() { counter += threaded_counter<128>(); }));
  pretty_print(8 *iterations, 8 *iterations * sizeof(uint64_t), "1024-byte",
               bench([&counter]() { counter += threaded_counter<1024>(); }));
}
