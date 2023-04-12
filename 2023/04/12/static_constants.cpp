#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

__attribute__((noinline)) std::string grab_string(size_t idx) {
  const std::string options[] = {"https", "http", "ftp",   "file",
                                 "ws",    "wss",  "httpr", "filer"};
  return options[idx];
}

__attribute__((noinline)) std::string grab_static_string(size_t idx) {
  const static std::string options[] = {"https", "http", "ftp",   "file",
                                        "ws",    "wss",  "httpr", "filer"};
  return options[idx];
}

__attribute__((noinline)) std::string_view grab_static(size_t idx) {
  const static std::string_view options[] = {"https", "http", "ftp",   "file",
                                             "ws",    "wss",  "httpr", "filer"};
  return options[idx];
}

__attribute__((noinline)) std::string_view grab(size_t idx) {
  constexpr static std::string_view options[] = {
      "https", "http", "ftp", "file", "ws", "wss", "httpr", "filer"};
  return options[idx];
}

volatile size_t counter = 0;

std::tuple<double, double, double, double> simulation(const size_t N) {
  double t1, t2, t3, t4;

  {

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      counter += grab(i % 8).size();
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      counter += grab_static(i % 8).size();
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }
  {

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      counter += grab_static_string(i % 8).size();
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }
  {

    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      counter += grab_string(i % 8).size();
    }
    uint64_t finish = nano();
    t4 = double(finish - start) / N;
  }
  return {t1, t2, t3, t4};
}

void demo() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;
  double avg4 = 0;

  size_t times = 100;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3, t4] = simulation(10000);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
    avg4 += t4;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;

  avg4 /= times;

  std::cout << avg1 << " " << avg2 << " " << avg3 << " " << avg4 << std::endl;
}

int main() {
  puts("We report ns/string_view (first constexpr static, then const "
       "static).\n");
  for (size_t i = 0; i < 10; i++) {
    demo();
  }
  printf("bogus: %zu \n", counter);
}
