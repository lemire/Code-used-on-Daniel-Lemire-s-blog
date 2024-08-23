
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <vector>
#include "batched_shuffle.h"

double pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  (void)bytes;
  (void)name;
  printf(" %5.1f   ", agg.elapsed_ns() / volume);
  fflush(stdout);
  return agg.elapsed_ns() / volume;
}

using cpp_shuffle_function = void (*)(std::vector<uint64_t>::iterator,
                                      std::vector<uint64_t>::iterator,
                                      std::mt19937_64 &);



struct named_cpp_function {
  std::string name;
  cpp_shuffle_function function;
};

named_cpp_function func[] = {
    {"std::shuffle", [](std::vector<uint64_t>::iterator first,
                               std::vector<uint64_t>::iterator last,
                               std::mt19937_64 &g) { std::shuffle(first, last, g); }},
    {"batched_random::shuffle_2",
     [](std::vector<uint64_t>::iterator first,
        std::vector<uint64_t>::iterator last,
        std::mt19937_64 &g) { batched_random::shuffle_2(first, last, g); }}};

void bench_line(std::vector<uint64_t> &input) {
  size_t volume = input.size();
  printf("%zu\t\t", volume);
  std::random_device rd;
  size_t min_repeat = 1;
  size_t min_time_ns = 100000000; // 1 ms
  size_t max_repeat = 100000;
  size_t repeat = 1;
  double tolerance = 1.1;
  while (volume * repeat < 10000000) {
    repeat++;
  }
  std::mt19937_64 mtGenerator{1235};
  double previous = 0;

  for (auto &f : func) {
    bench(
                     [&input, &f, repeat, &mtGenerator]() {
                       for (size_t r = 0; r < repeat; r++) {
                         f.function(input.begin(), input.end(), mtGenerator);
                       }
                     },
                     min_repeat, min_time_ns, max_repeat, tolerance);
    double thisvalue= pretty_print(volume * repeat, repeat * volume * sizeof(uint64_t), f.name,
                 bench(
                     [&input, &f, repeat, &mtGenerator]() {
                       for (size_t r = 0; r < repeat; r++) {
                         f.function(input.begin(), input.end(), mtGenerator);
                       }
                     },
                     min_repeat, min_time_ns, max_repeat, tolerance));
    printf("         ");
    if(previous!=0){
      printf(" %3.f %%  ", previous/thisvalue *100 - 100);
    }
    previous = thisvalue;
  }
}

void bench_table(size_t start, size_t end, size_t lines) {
  double b = pow(double(end) / start, 1.0 / lines);
  printf("# for each scheme, we give the average "
         "time/item in ns \n");
  printf("# Volume ");
  for (auto &f : func) {
    printf("\t%s", f.name.c_str());
  }
  printf("\n");
  for (double i = start; round(i) <= end; i *= b) {
    std::vector<uint64_t> input(round(i));
    bench_line(input);
    std::cout << std::endl;
  }
}

int main(int, char **) {
  bench_table(10000, 1000000, 5);

  return EXIT_SUCCESS;
}
