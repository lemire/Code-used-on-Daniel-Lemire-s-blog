
#include "fast_float/fast_float.h"
#include "m1cycles.h"
#include <algorithm>
#include <charconv>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale.h>
#include <random>
#include <sstream>
#include <stdio.h>
#include <vector>

#include "random_generators.h"

double findmax_strtod(std::vector<std::string> &s) {
  double answer = 0;
  double x = 0;
  for (std::string &st : s) {
    char *pr = (char *)st.data();
    static locale_t c_locale = newlocale(LC_ALL_MASK, "C", NULL);
    x = strtod_l(st.data(), &pr, c_locale);
    if (pr == st.data()) {
      throw std::runtime_error("bug in findmax_strtod");
    }
    answer = answer > x ? answer : x;
  }
  return answer;
}

double findmax_fastfloat(std::vector<std::string> &s) {
  double answer = 0;
  double x = 0;
  for (std::string &st : s) {
    auto [p, ec] = fast_float::from_chars(st.data(), st.data() + st.size(), x);
    if (p == st.data()) {
      throw std::runtime_error("bug in findmax_fastfloat");
    }
    answer = answer > x ? answer : x;
  }
  return answer;
}

template <class T>
std::pair<performance_counters, performance_counters>
time_it_ns(std::vector<std::string> &lines, T const &function, size_t repeat) {
  performance_counters agg_min{1e300};
  performance_counters agg_avg{0.0};
  // warm up the cache:
  for (size_t i = 0; i < 10; i++) {
    double ts = function(lines);
    if (ts == 0) {
      printf("bug\n");
    }
  }
  for (size_t i = 0; i < repeat; i++) {
    performance_counters start = get_counters();
    double ts = function(lines);
    if (ts == 0) {
      printf("bug\n");
    }
    performance_counters end = get_counters();
    performance_counters diff = end - start;
    agg_min = agg_min.min(diff);
    agg_avg += diff;
  }
  agg_avg /= lines.size() * repeat;
  agg_min /= lines.size();
  return std::make_pair(agg_min, agg_avg);
}

void pretty_print(
    double volume, size_t number_of_floats, std::string name,
    std::pair<performance_counters, performance_counters> result) {
  (void)volume;
  (void)number_of_floats;
  printf(" %32s ", name.c_str());
  printf(" %8.2f instructions/float (+/- %3.1f %%) ", result.first.instructions,
         (result.second.instructions - result.first.instructions) * 100.0 /
             result.first.instructions);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.2f cycles/float (+/- %3.1f %%) ", result.first.cycles,
         (result.second.cycles - result.first.cycles) * 100.0 /
             result.first.cycles);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.2f instructions/cycle ",
         result.first.instructions / result.first.cycles);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.2f branches/float (+/- %3.1f %%) ", result.first.branches,
         (result.second.branches - result.first.branches) * 100.0 /
             result.first.branches);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.4f mis. branches/float ", result.second.missed_branches);
  printf("\n");
}

void process(std::vector<std::string> &lines, size_t volume) {
  size_t repeat = 100;
  double volumeMB = volume / (1024. * 1024.);
  std::cout << "volume = " << volumeMB << " MB " << std::endl;
  pretty_print(volume, lines.size(), "strtod",
               time_it_ns(lines, findmax_strtod, repeat));
  printf("\n");
  pretty_print(volume, lines.size(), "fastfloat",
               time_it_ns(lines, findmax_fastfloat, repeat));
  printf("\n");
}

void parse_random_numbers() {
  std::cout << "# parsing random numbers" << std::endl;
  std::vector<std::string> lines;
  size_t howmany{10000};
  auto g = std::unique_ptr<string_number_generator>(
      get_generator_by_name("uniform"));
  std::cout << "model: " << g->describe() << std::endl;
  std::cout << "volume: " << howmany << " floats" << std::endl;
  lines.reserve(howmany); // let us reserve plenty of memory.
  size_t volume = 0;
  for (size_t i = 0; i < howmany; i++) {
    std::string line = g->new_string();
    volume += line.size();
    lines.push_back(line);
  }
  process(lines, volume);
}

int main() {
  setup_performance_counters();
  parse_random_numbers();
  return EXIT_SUCCESS;
}