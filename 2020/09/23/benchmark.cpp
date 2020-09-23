
#include <algorithm>
#include <chrono>
#include <cfloat>
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
#include <sstream>
#include <stdio.h>
#include <vector>
#include <charconv>
#include <random>

__attribute__((noinline))
std::pair<double,double> time_it_ns(std::vector<uint64_t> &data, size_t repeat) {
  std::chrono::high_resolution_clock::time_point t1, t2;
  double average = 0;
  double min_value = DBL_MAX;
  for (size_t i = 0; i < repeat; i++) {
    t1 = std::chrono::high_resolution_clock::now();
    uint64_t sum{0};
    for(auto x : data) {
    //  std::cout << x << std::endl;
      sum += x;
    }
    if (sum == 0) { std::cerr << "bad" << std::endl; }
    t2 = std::chrono::high_resolution_clock::now();
    double dif =
        std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    average += dif;
    min_value = min_value < dif ? min_value : dif;
  }
  return std::make_pair(min_value, average);
}

void process(std::vector<uint64_t> &lines) {
  size_t repeat = 40;
  auto pretty_print = [&lines](std::string name,
                                 std::pair<double, double> result) {
    printf("%-40s: %8.2f ns/int (+/- %.1f %%)\n", name.data(),
           result.first/lines.size(),
           100 - (result.second - result.first) * 100.0 / result.second);
  };
  pretty_print("big sum", time_it_ns(lines, repeat));
}

void demo(size_t howmany) {
  std::cout << "# adding integers in a range" << std::endl;
  std::vector<uint64_t> lines;
  std::random_device rd;
  std::mt19937 gen(rd());
  uint64_t max_value{0xFFFFFFFFFFFFFFFF/howmany};
  std::cout << max_value << std::endl;
  std::uniform_int_distribution<uint64_t> dis(0, max_value);
  lines.reserve(howmany); // let us reserve plenty of memory.
  for (size_t i = 0; i < howmany; i++) { 
    uint64_t random_value {dis(gen)};
    lines.push_back(random_value);
  }
  process(lines);
}

int main(int argc, char **argv) {
    demo(100 * 1000);
}