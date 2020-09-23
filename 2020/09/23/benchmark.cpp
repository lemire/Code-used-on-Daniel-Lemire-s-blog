
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

template <typename T>
__attribute__((noinline))
std::pair<double,double> time_it_ns(std::vector<T> &data, size_t repeat) {
  std::chrono::high_resolution_clock::time_point t1, t2;
  double average = 0;
  double min_value = DBL_MAX;
  for (size_t i = 0; i < repeat; i++) {
    t1 = std::chrono::high_resolution_clock::now();
    T sum{0};
    for(auto x : data) {
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

template <typename T>
void process(std::vector<T> &lines) {
  size_t repeat = 40;
  auto pretty_print = [&lines](std::string name,
                                 std::pair<double, double> result) {
    printf("%-40s: %8.2f ns/int (+/- %.1f %%)\n", name.data(),
           result.first/lines.size(),
           100 - (result.second - result.first) * 100.0 / result.second);
  };
  pretty_print("big sum ", time_it_ns(lines, repeat));
}

template <typename T>
void demo(size_t howmany) {
  std::cout << "# adding integers in a range" << std::endl;
  std::vector<T> lines;
  std::random_device rd;
  std::mt19937 gen(rd());
  const T max_value = std::numeric_limits<T>::max()/howmany;
  std::cout << max_value << std::endl;
  std::uniform_int_distribution<T> dis(0, max_value);
  lines.reserve(howmany); // let us reserve plenty of memory.
  for (size_t i = 0; i < howmany; i++) { 
    T random_value {dis(gen)};
    lines.push_back(random_value);
  }
  process(lines);
}

int main(int argc, char **argv) {
    std::cout << "unsigned" << std::endl;
    demo<uint64_t>(1000);
    std::cout << "signed (trap)" << std::endl;
    demo<int64_t>(1000);
}
