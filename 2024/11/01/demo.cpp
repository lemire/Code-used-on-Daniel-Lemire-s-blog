#include <algorithm>
#include <bit>
#include <chrono>
#include <concepts>
#include <expected>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <ranges>
#include <source_location>
#include <string_view>
#include <vector>

using namespace std::literals::string_view_literals;

void print(auto ...args) {
  (std::println("{:->50}", args), ...);
}
void prints() {
  std::string_view name = "Daniel"sv;
  std::println("Hello, {0} today is {1:%Y-%m-%d %X}, good day {0}!", name,
               std::chrono::system_clock::now());
   // Hello, Daniel today is 2024-11-02 00:02:17, good day Daniel!
  for (int k = 0; k < 5; k++) {
    std::println("{:-^50}", 333 * k);
  }
  print("a", 1, "b", 2, "c", 3);
}


template <std::integral number>
std::expected<number, std::string> divide(number a, number b) {
  if (b == 0) {
    return std::unexpected("Division by zero");
  }
  return a / b;
}

void test(auto x, auto y) {
  if (auto result = divide(x, y); result) {
    std::println("Result: {}", result.value());
  } else {
    std::println(stderr, "Error: {}", result.error());
  }
}

int main() {
  prints();
  test(1, 2);
  test(1, 0);
  std::println("{:b} {}", 4u, std::countr_zero(4u));
  std::println("{:x}", std::rotl(0xf0f0f0f0u, 4));
  std::println("{:b}", std::bit_cast<uint64_t>(1.0));
  std::println("{} bytes", std::filesystem::file_size("demo.cpp"));

  auto log = [](std::string_view message, std::source_location loc) {
    std::println("{} {}:{} function name: {}", message, loc.file_name(),
                 loc.line(), loc.function_name());
  };

  log("problem", std::source_location::current());
  log("problem", std::source_location::current());

  std::vector<int> v = {1, 2, 3, 4, 5};
  std::println("{}", v);
  std::println("{:_^40}", v);

  std::println("{}", std::views::all(v));
  std::println("{}", std::ranges::max(v));
  std::println("{}", v | std::views::reverse);
}