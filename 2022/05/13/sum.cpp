#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <x86intrin.h>

const size_t size = 10000;

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
template <typename PROCEDURE>
double bench(PROCEDURE f, uint64_t threshold = 500'000'000) {
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  int val{};
  for (; finish - start < threshold;) {
    count++;
    val += f();
    finish = nano();
  }
  if(val == 0) { std::cout << "bogus" << std::endl; }
  return double(finish - start) / count / size;
}

int get_positive_value(int x) {
    if(x < 0) { throw std::runtime_error("it is not positive!"); }
    return x;
}
__attribute__ ((noinline))
int sum_abs_value_exception(std::vector<int> &a) {
    int sum = 0;
    for (int x : a) {
        try {
            sum += get_positive_value(x);
        } catch (...) {
            sum += -x;
        }
    }
    return sum;
}


__attribute__ ((noinline)) 
int sum_abs_value(std::vector<int> &a) noexcept {
    int sum = 0;
    for (int x : a) {
        if(x < 0) {
            sum += -x;
        } else {
            sum += x;
        }
    }
    return sum;
}

int main() {

  std::vector<int> data;
  for (size_t i = 0; i < size; i++) {
    data.push_back((rand() % 1000) - 500);
  }
  assert(sum_abs_value_exception(data) == sum_abs_value(data));
  auto exceptionless = [&data]() -> int {
    return sum_abs_value(data);
  };
  auto exceptionfull = [&data]() -> int {
    return sum_abs_value_exception(data);
  };

  for (size_t i = 0; i < 5; i++) {

    std::cout << "exceptionless ";
    std::cout << bench(exceptionless) << " ns/value" << std::endl;
    std::cout << "exceptionfull ";
    std::cout << bench(exceptionfull) << " ns/value" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < size; i++) {
      data[i] = (rand() % 1000) - 500;
    }
    assert(sum_abs_value_exception(data) == sum_abs_value(data));
  }
}
