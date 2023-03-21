
#include "fast_float/fast_float.h"
#include "performancecounters/event_counter.h"
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

event_collector collector;

template <class T>
event_aggregate time_it_ns(std::vector<std::string> &lines, T const &function, size_t repeat) {
  event_aggregate aggregate{};

  // warm up the cache:
  for (size_t i = 0; i < 10; i++) {
    double ts = function(lines);
    if (ts == 0) {
      printf("bug\n");
    }
  }
  for (size_t i = 0; i < repeat; i++) {
    collector.start();
    double ts = function(lines);
    if (ts == 0) {
      printf("bug\n");
    }
    event_count allocate_count = collector.end();
    aggregate << allocate_count;
  }
  return aggregate;
}

void pretty_print(
    double volume, size_t number_of_floats, std::string name,
    event_aggregate aggregate) {
  (void)volume;
  (void)number_of_floats;
  printf(" %32s ", name.c_str());
  printf(" %8.2f ns/float ", aggregate.elapsed_ns() / number_of_floats);

  printf(" %8.2f instructions/float ", aggregate.best.instructions() / number_of_floats);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.2f cycles/float  ", aggregate.best.cycles() / number_of_floats);
  printf("\n");
  printf(" %32s ", "");
  printf(" %8.2f instructions/cycle ",
         aggregate.best.instructions() / aggregate.best.cycles());

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
  parse_random_numbers();
  if(!collector.has_events()) {
    std::cerr << "I cannot access the performance counters. Make sure you run the program in privileged mode (e.g., sudo) under Linux our macOS/ARM." << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}