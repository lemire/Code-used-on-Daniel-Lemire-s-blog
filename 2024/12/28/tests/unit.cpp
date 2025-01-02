#include "utf16fixer.h"
#include <algorithm>
#include <cstdlib>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <ranges>
#include <vector>

bool compare() {
  std::vector<char16_t> data(10000000);
  std::mt19937 gen(42); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(0, 0xFFFF); // Range for char16_t

  for (size_t i = 0; i < data.size(); i++) {
    data[i] = static_cast<char16_t>(dis(gen));
  }
  std::vector<char16_t> copy1(data);
  std::vector<char16_t> copy2(data);

  replace_invalid_utf16_neon(copy1.data(), copy1.size());
  replace_invalid_utf16(copy2.data(), copy2.size());

  size_t c1 =
      std::ranges::count_if(std::views::zip(copy1, data), [](const auto &pair) {
        return std::get<0>(pair) != std::get<1>(pair);
      });
  size_t c2 =
      std::ranges::count_if(std::views::zip(copy2, data), [](const auto &pair) {
        return std::get<0>(pair) != std::get<1>(pair);
      });
  std::print("c1: {}, c2: {}\n", c1, c2);
  return copy1 == copy2;
}



bool compare2() {
  std::vector<char16_t> data(10000000);
  std::mt19937 gen(42); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(0, 0xFFFF); // Range for char16_t

  for (size_t i = 0; i < data.size(); i++) {
    data[i] = static_cast<char16_t>(dis(gen));
  }
  std::vector<char16_t> copy1(data);
  std::vector<char16_t> copy2(data);

  replace_invalid_utf16_neon_v2(copy1.data(), copy1.size());
  replace_invalid_utf16(copy2.data(), copy2.size());

  size_t c1 =
      std::ranges::count_if(std::views::zip(copy1, data), [](const auto &pair) {
        return std::get<0>(pair) != std::get<1>(pair);
      });
  size_t c2 =
      std::ranges::count_if(std::views::zip(copy2, data), [](const auto &pair) {
        return std::get<0>(pair) != std::get<1>(pair);
      });
  std::print("c1: {}, c2: {}\n", c1, c2);
  return copy1 == copy2;
}

int main() {
  if (compare() && compare2()) {
    std::cerr << "Test succeeded!" << std::endl;
    return EXIT_SUCCESS;
  } else {
    std::cerr << "Test failed!" << std::endl;
    return EXIT_FAILURE;
  }
}