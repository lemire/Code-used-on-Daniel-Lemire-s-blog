
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <vector>
#include <tuple>
#include <string_view>
#include <charconv>

uint64_t nano() {
  return std::chrono::duration_cast< ::std::chrono::nanoseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count();
}

#include <string>

std::string ipv40(const uint64_t address) noexcept {
  std::string output = std::to_string(address % 256);
  auto n = address >> 8;
  for (size_t i = 1; i < 4; i++) {
    output.insert(0, std::to_string(n % 256) + ".");
    n >>= 8;
  }
  return output;
}

std::string ipv41(const uint64_t address) noexcept {
  std::string output = std::to_string(address >> 24);
  for (int i = 2; i >= 0; i--) {
    output.append(std::to_string((address >> (i * 8)) % 256) + ".");
  }
  return output;
}


std::string ipv42(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, address >> 24).ptr;
  for (int i = 2; i >= 0; i--) {
    *point++ = '.';
    point = std::to_chars(point, point_end, (address >> (i * 8)) % 256).ptr;
  }
  output.resize(point - output.data());
  return output;
}

std::string ipv43(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, uint8_t(address >> 24)).ptr;
  for (int i = 2; i >= 0; i--) {
    *point++ = '.';
    point = std::to_chars(point, point_end, uint8_t(address >> (i * 8))).ptr;
  }
  output.resize(point - output.data());
  return output;
}
std::vector<std::string> data;

std::tuple<double, double, double, double> simulation(const size_t N) {
  double t1, t2, t3, t4;
  data.reserve(N);

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv40(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }
  {

    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv41(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }

  {

    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv42(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }
  {

    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv43(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t4 = double(finish - start) / N;
  }
  return { t1, t2, t3, t4 };
}

void demo() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;
  double avg4 = 0;

  size_t times = 10;

  for (size_t i = 0; i < times; i++) {
    auto[ t1, t2, t3, t4 ] = simulation(131072);
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

int main() { demo(); }
