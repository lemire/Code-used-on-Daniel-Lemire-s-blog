#include <charconv>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <sstream>

#include <string_view>
#include <string>

#include "agg1.h"
#include "agg2.h"

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

std::vector<std::string_view> base = {"Microsoft", "goto", ":", "Apple", "http://google.com", "elephant"};

std::tuple<double, double, double>
simulation4(const size_t N) {
  double t1, t2, t3;
  std::vector<std::string_view> database;
  for(size_t i = 0; i < N * 10; i++) { database.emplace_back(base[(1271132211 * i)%base.size()]); }
  std::vector<std::string> data;

  data.reserve(N);

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat0(database[4*i+0], database[4*i+1], database[4*i+2], database[4*i+3]));
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat1(database[4*i+0], database[4*i+1], database[4*i+2], database[4*i+3]));
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat2(database[4*i+0], database[4*i+1], database[4*i+2], database[4*i+3]));
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }


  return {t1, t2, t3};
}

std::tuple<double, double, double>
simulation3(const size_t N) {
  double t1, t2, t3;
  std::vector<std::string_view> database;
  for(size_t i = 0; i < N * 10; i++) { database.emplace_back(base[(1271132211 * i)%base.size()]); }
  std::vector<std::string> data;

  data.reserve(N);

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat0(database[3*i+0], database[3*i+1], database[3*i+2]));
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat1(database[3*i+0], database[3*i+1], database[3*i+2]));
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat2(database[3*i+0], database[3*i+1], database[3*i+2]));
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }


  return {t1, t2, t3};
}



std::tuple<double, double, double>
simulation2(const size_t N) {
  double t1, t2, t3;
  std::vector<std::string_view> database;
  for(size_t i = 0; i < N * 10; i++) { database.emplace_back(base[(1271132211 * i)%base.size()]); }
  std::vector<std::string> data;

  data.reserve(N);

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat0(database[3*i+0], database[3*i+1]));
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat1(database[3*i+0], database[3*i+1]));
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(concat2(database[3*i+0], database[3*i+1]));
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }


  return {t1, t2, t3};
}

void demo4() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;

  size_t times = 10;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3] = simulation4(131072);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;

  std::cout << "Time per string in ns.\n";
  std::cout << "append  " << avg1 << std::endl;
  std::cout << "reserve+append  " << avg2 << std::endl;
  std::cout << "resize+copy " << avg3 << std::endl;
}


void demo3() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;

  size_t times = 10;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3] = simulation3(131072);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;

  std::cout << "Time per string in ns.\n";
  std::cout << "append  " << avg1 << std::endl;
  std::cout << "reserve+append  " << avg2 << std::endl;
  std::cout << "resize+copy " << avg3 << std::endl;
}


void demo2() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;

  size_t times = 10;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3] = simulation2(131072);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;

  std::cout << "Time per string in ns.\n";
  std::cout << "append  " << avg1 << std::endl;
  std::cout << "reserve+append  " << avg2 << std::endl;
  std::cout << "resize+copy " << avg3 << std::endl;
}
int main() {
  for(size_t i = 0; i < 10; i++) {  
    printf("two string_view\n");
    demo2();
    printf("\nthree string_view\n");
    demo3();
    printf("\nfour string_view\n");
    demo4();
    printf("\n");
  }


}
