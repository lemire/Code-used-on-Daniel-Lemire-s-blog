#include <array>
#include <format>
#include <iostream>
#include <optional>
#include <ranges>
#include <tuple>
#include <vector>

template <typename T, std::size_t N>
void print_array(const std::array<T, N> &arr) {
  std::cout << "["; // Start with an opening bracket
  for (std::size_t i = 0; i < N; ++i) {
    // Use std::format for printing individual elements
    std::cout << std::format("{}", arr[i]);
    if (i < N - 1) {
      std::cout << ", "; // Add comma and space between elements
    }
  }
  std::cout << "]\n"; // End with a closing bracket and newline
}

// Templated to_digits function, temp should be in [0, 10**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_digits>
std::array<int, number_digits> to_digits(uint64_t temp) {
  static_assert(number_digits > 0 && number_digits <= 9,
                "number_digits must be between 1 and 9 inclusive");
  std::array<int, number_digits> result;
  constexpr auto int_pow = [](uint64_t base, uint64_t exponent) -> uint64_t {
    uint64_t result = 1;
    while (exponent > 0) {
      if (exponent & 1)
        result *= base;
      base *= base;
      exponent >>= 1;
    }
    return result;
  };
  if constexpr (number_digits < 6) {
    constexpr uint64_t constant =
        ((1ULL << 32) / int_pow(10, number_digits - 1)) + 1;
    uint64_t full = temp * constant; // Scale to get the first digit
    result[0] = full >> 32;

    for (int i : std::ranges::views::iota(1, number_digits)) {
      full = (full & 0xffffffff) * 10;
      result[i] = full >> 32;
    }
  } else {
    constexpr int number_of_most_significant_bits = 8;
    constexpr int number_of_least_significant_bits =
        64 - number_of_most_significant_bits;
    constexpr uint64_t mask =
        UINT64_C(0xFFFFFFFFFFFFFFFF) >> number_of_most_significant_bits;
    constexpr uint64_t constant = ((1ULL << number_of_least_significant_bits) /
                                   int_pow(10, number_digits - 1)) +
                                  1;
    uint64_t full = temp * constant; // Scale to get the first digit
    result[0] = full >> number_of_least_significant_bits;

    for (int i : std::ranges::views::iota(1, number_digits)) {
      full = (full & mask) * 10;
      result[i] = full >> number_of_least_significant_bits;
    }
  }

  return result;
}

// Templated to_centi_digits function, temp should be in [0, 100**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits(uint64_t temp) {
  static_assert(number_centi_digits > 0 && number_centi_digits <= 6,
                "number_digits must be between 1 and 6 inclusive");
  std::array<int, number_centi_digits> result;
  constexpr auto int_pow = [](uint64_t base, uint64_t exponent) -> uint64_t {
    uint64_t result = 1;
    while (exponent > 0) {
      if (exponent & 1)
        result *= base;
      base *= base;
      exponent >>= 1;
    }
    return result;
  };
  constexpr int number_of_most_significant_bits =
      number_centi_digits < 5 ? 32 : 8;
  constexpr int number_of_least_significant_bits =
      64 - number_of_most_significant_bits;
  constexpr uint64_t mask =
      UINT64_C(0xFFFFFFFFFFFFFFFF) >> number_of_most_significant_bits;
  constexpr uint64_t constant = ((1ULL << number_of_least_significant_bits) /
                                 int_pow(100, number_centi_digits - 1)) +
                                1;
  uint64_t full = temp * constant; // Scale to get the first digit
  result[0] = full >> number_of_least_significant_bits;

  for (int i : std::ranges::views::iota(1, number_centi_digits)) {
    full = (full & mask) * 100;
    result[i] = full >> number_of_least_significant_bits;
  }

  return result;
}

// Structure to hold the result of a mistake
template <int number_digits> struct mistake {
  uint64_t input;
  std::array<int, number_digits> output;
  std::array<int, number_digits> expected;

  // Convert mistake to string representation using std::format
  std::string to_string() const {
    std::string result = std::format("Input: {}, Output: {{", input);
    for (size_t i = 0; i < number_digits; ++i) {
      result += std::format("{}", output[i]);
      if (i < number_digits - 1) {
        result += ", ";
      }
    }
    result += "}, Expected: {";
    for (size_t i = 0; i < number_digits; ++i) {
      result += std::format("{}", expected[i]);
      if (i < number_digits - 1) {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }
};

// Helper function to compute expected digits
template <int number_digits>
std::array<int, number_digits> get_expected_digits(uint64_t num) {
  std::array<int, number_digits> result = {};

  // Fill digits from right to left (least significant to most)
  for (size_t i = 0; i < number_digits; ++i) {
    result[number_digits - 1 - i] = (num % 10); // Get the last digit
    num /= 10;
  }
  return result;
}

// Helper function to compute expected digits
template <int number_digits>
std::array<int, number_digits> get_expected_centi_digits(uint64_t num) {
  std::array<int, number_digits> result = {};

  // Fill digits from right to left (least significant to most)
  for (size_t i = 0; i < number_digits; ++i) {
    result[number_digits - 1 - i] = (num % 100); // Get the last digit
    num /= 100;
  }
  return result;
}
// Test function to find the first mistake
template <int number_digits>
std::optional<mistake<number_digits>> find_first_mistake() {
  uint64_t limit = std::pow(10, number_digits);

  // Ensure limit doesn't exceed uint64_t max
  if (limit > std::numeric_limits<uint64_t>::max()) {
    return std::nullopt; // Input range too large
  }

  for (uint64_t i = 0; i < limit; ++i) {
    std::array<int, number_digits> result = to_digits<number_digits>(i);
    std::array<int, number_digits> expected =
        get_expected_digits<number_digits>(i);

    if (result != expected) {
      return mistake<number_digits>{i, result, expected};
    }
  }

  return std::nullopt; // No mistakes found
}

// Test function to find the first mistake
template <int number_centi_digits>
std::optional<mistake<number_centi_digits>> find_first_centi_mistake() {
  uint64_t limit = std::pow(10, number_centi_digits);

  // Ensure limit doesn't exceed uint64_t max
  if (limit > std::numeric_limits<uint64_t>::max()) {
    return std::nullopt; // Input range too large
  }

  for (uint64_t i = 0; i < limit; ++i) {
    std::array<int, number_centi_digits> result =
        to_centi_digits<number_centi_digits>(i);
    std::array<int, number_centi_digits> expected =
        get_expected_centi_digits<number_centi_digits>(i);

    if (result != expected) {
      return mistake<number_centi_digits>{i, result, expected};
    }
  }

  return std::nullopt; // No mistakes found
}

int test_digits() {

  if (auto m = find_first_mistake<1>(); m) {
    std::cout << "1:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<2>(); m) {
    std::cout << "2:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<3>(); m) {
    std::cout << "3:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<4>(); m) {
    std::cout << "4:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<5>(); m) {
    std::cout << "5:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<6>(); m) {
    std::cout << "6:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<7>(); m) {
    std::cout << "7:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<8>(); m) {
    std::cout << "8:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_mistake<9>(); m) {
    std::cout << "9:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  return EXIT_SUCCESS; // No mistakes found
}

int test_centi_digits() {
  if (auto m = find_first_centi_mistake<1>(); m) {
    std::cout << "1:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_centi_mistake<2>(); m) {
    std::cout << "2:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_centi_mistake<3>(); m) {
    std::cout << "3:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_centi_mistake<4>(); m) {
    std::cout << "4:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_centi_mistake<5>(); m) {
    std::cout << "5:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  if (auto m = find_first_centi_mistake<6>(); m) {
    std::cout << "6:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  return EXIT_SUCCESS; // No mistakes found
}

int main() {
  return test_centi_digits(); // No mistakes found
}