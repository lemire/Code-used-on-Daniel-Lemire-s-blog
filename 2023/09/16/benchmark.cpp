
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <vector>
#include <filesystem>
#include <iostream>


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

#include <thread>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <iostream>
 
template <int align>
struct alignas(align) A
{
    uint64_t count = rand();
};

constexpr size_t iterations = 10'000'000;

// compilers seemingly struggle to optimize this away
void counter(uint64_t* counterpt) {
    for(int i = 0; i < iterations; i++) {
        *counterpt = 3 * *counterpt * *counterpt  + 1;
    }
}
template <int align>
uint64_t threaded_counter() {
    A<align> a[4];
    static_assert(sizeof(a) == 4*align);
    std::thread t1(counter, &a[0].count);
    std::thread t2(counter, &a[1].count);
    std::thread t3(counter, &a[2].count);
    std::thread t4(counter, &a[3].count);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    //printf("%lu %lu %lu %lu\n", a[0].count, a[1].count, a[2].count, a[3].count);

    return a[0].count + a[1].count + a[2].count + a[3].count;
}

int main(int argc, char **argv) {
  uint64_t counter = 0;

  pretty_print(4*iterations, 4*iterations*sizeof(uint64_t), "8-byte",
               bench([&counter]() {
                counter += threaded_counter<8>();
               }));
  pretty_print(4*iterations, 4*iterations*sizeof(uint64_t), "16-byte",
               bench([&counter]() {
                counter += threaded_counter<16>();
               }));
  pretty_print(4*iterations, 4*iterations*sizeof(uint64_t), "64-byte",
               bench([&counter]() {
                counter += threaded_counter<64>();
               }));
  pretty_print(4*iterations, 4*iterations*sizeof(uint64_t), "128-byte",
               bench([&counter]() {
                counter += threaded_counter<128>();
               }));

}
