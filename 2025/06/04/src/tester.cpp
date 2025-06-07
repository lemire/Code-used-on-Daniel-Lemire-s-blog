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
std::array<int, 4> to_centi_digits_dragon(uint64_t temp) {
      std::array<int, 4> result;

        // 281474978 = ceil(2^48 / 100'0000) + 1
        auto prod = temp * UINT64_C(281474978);
        prod >>= 16;
        result[0] = int(prod >> 32);
        prod = (prod & UINT32_C(0xffffffff)) * 100;
        result[1] = (int(prod >> 32));
        prod = (prod & UINT32_C(0xffffffff)) * 100;
        result[2] = (int(prod >> 32));
        prod = (prod & UINT32_C(0xffffffff)) * 100;
        result[3] = (int(prod >> 32));
        return result;
}

// Templated to_centi_digits function, temp should be in [0, 100**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits_classic(uint64_t temp) {
  static_assert(number_centi_digits > 0 && number_centi_digits <= 5,
                "number_digits must be between 1 and 5 inclusive");
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
  constexpr int number_of_most_significant_bits =   number_centi_digits < 4 ? 32 : 8;
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
template<size_t N>
std::array<int, N+1> prepend_zero(const std::array<int, N>& input) {
    std::array<int, N+1> result{};
    result[0] = 0;
    for(size_t i = 0; i < N; ++i) {
        result[i+1] = input[i];
    }
    return result;
}

template<size_t N>
std::array<int, N+2> prepend_zero2(const std::array<int, N>& input) {
    std::array<int, N+2> result{};
    result[0] = 0;
    result[1] = 0;
    for(size_t i = 0; i < N; ++i) {
        result[i+2] = input[i];
    }
    return result;
}
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits(uint64_t temp) {
  static_assert(number_centi_digits > 0 && number_centi_digits <= 4,
                "number_digits must be between 1 and 4 inclusive");
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
  if constexpr (number_centi_digits == 4) {
    if (temp < int_pow(100, number_centi_digits - 1)) {
        return prepend_zero<3>(to_centi_digits_classic<3>(temp));
    }
  }
  constexpr int number_of_most_significant_bits =  number_centi_digits < 4 ? 32 : 8;
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

constexpr uint64_t div_2pow64_by_n(uint64_t n) {
    if (n == 0) {
        return 0; // Handle division by zero
    }
    // Compute (2^64 - 1) / n and adjust
    uint64_t quotient = UINT64_MAX / n;
    // If n divides 2^64 exactly, add 1 to the quotient
    uint64_t remainder = UINT64_MAX % n;
    if (remainder == n - 1) {
        quotient += 1;
    }
    return quotient;
}

std::pair<uint64_t, uint64_t> mul64x64_to_128(uint64_t a, uint64_t b) {
#ifdef _MSC_VER
    // MSVC: Use _umul128 intrinsic
    uint64_t high;
    uint64_t low = _umul128(a, b, &high);
    return {high, low};
#else
    // GCC/Clang: Use __uint128_t
    __uint128_t result = static_cast<__uint128_t>(a) * static_cast<__uint128_t>(b);
    return {static_cast<uint64_t>(result >> 64), static_cast<uint64_t>(result)};
#endif
}

// Templated to_centi_digits function, temp should be in [0, 100**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits128(uint64_t temp) {
  static_assert(number_centi_digits > 0 && number_centi_digits <= 4,
                "number_digits must be between 1 and 4 inclusive");
  std::array<int, number_centi_digits> result;
   if constexpr (number_centi_digits == 1) {
    result[0] = temp; // Directly return the last two digits
    return result;
    }
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
  constexpr uint64_t constant = div_2pow64_by_n(
      int_pow(100, number_centi_digits-1)) +1;
  auto [high, low] = mul64x64_to_128(temp, constant);
  result[0] = high;

  for (int i : std::ranges::views::iota(1, number_centi_digits)) {
    std::tie(high, low) = mul64x64_to_128(low, 100);
    result[i] = high;
  }

  return result;
}


// Templated to_centi_digits function, temp should be in [0, 100**number_digits)
// (We do not check this, so it is the caller's responsibility)
template <int number_centi_digits>
std::array<int, number_centi_digits> to_centi_digits_non_zero_lead(uint64_t temp) {
  std::array<int, number_centi_digits> result;
   if constexpr (number_centi_digits == 1) {
    result[0] = temp; // Directly return the last two digits
    return result;
    }
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
  constexpr uint64_t constant = div_2pow64_by_n(
      int_pow(100, number_centi_digits-1)) +1;
  auto [high, low] = mul64x64_to_128(temp, constant);
  result[0] = high;

  for (int i : std::ranges::views::iota(1, number_centi_digits)) {
    std::tie(high, low) = mul64x64_to_128(low, 100);
    result[i] = high;
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
std::optional<mistake<4>> find_first_centi_mistake_dragon() {
  uint64_t limit = std::pow(100, 4);

  // Ensure limit doesn't exceed uint64_t max
  if (limit > std::numeric_limits<uint64_t>::max()) {
    return std::nullopt; // Input range too large
  }

  for (uint64_t i = 0; i < limit; ++i) {
    std::array<int, 4> result =
        to_centi_digits_dragon(i);
    std::array<int, 4> expected =
        get_expected_centi_digits<4>(i);

    if (expected[0] != 0 && result != expected) {
      return mistake<4>{i, result, expected};
    }
  }

  return std::nullopt; // No mistakes found
}


// Test function to find the first mistake
template <int number_centi_digits>
std::optional<mistake<number_centi_digits>> find_first_centi_mistake() {
  uint64_t limit = std::pow(100, number_centi_digits);

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


// Test function to find the first mistake
template <int number_centi_digits>
std::optional<mistake<number_centi_digits>> find_first_centi_mistake_non_zero_lead() {
  uint64_t limit = std::pow(100, number_centi_digits);

  // Ensure limit doesn't exceed uint64_t max
  if (limit > std::numeric_limits<uint64_t>::max()) {
    return std::nullopt; // Input range too large
  }
  std::array<int, number_centi_digits> expected{};


  for (uint64_t i = 0; i < limit; ++i) {
    std::array<int, number_centi_digits> result =
        to_centi_digits_non_zero_lead<number_centi_digits>(i);
    //std::array<int, number_centi_digits> expected =
      //  get_expected_centi_digits<number_centi_digits>(i);
      
    if (result[0] != 0 && result != expected) {
      return mistake<number_centi_digits>{i, result, expected};
    }
    for (int j = number_centi_digits - 1; j >= 0; --j) {
      if (++expected[j] == 100) {
        expected[j] = 0;
        // Carry propagates to the next digit, no need to continue if j == 0
      } else {
        break;
      }
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
    printf("Testing dragon...\n");

  if (auto m = find_first_centi_mistake_dragon(); m) {
    std::cout << "dragon:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  printf("Testing 1...\n");

  if (auto m = find_first_centi_mistake<1>(); m) {
    std::cout << "1:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  printf("Testing 2...\n");

  if (auto m = find_first_centi_mistake<2>(); m) {
    std::cout << "2:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  printf("Testing 3...\n");

  if (auto m = find_first_centi_mistake<3>(); m) {
    std::cout << "3:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
printf("Testing 4...\n");

  if (auto m = find_first_centi_mistake<4>(); m) {
    std::cout << "4:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  printf("Testing find_first_centi_mistake_non_zero_lead...\n");

  if (auto m = find_first_centi_mistake_non_zero_lead<8>(); m) {
    std::cout << "8:" << m->to_string() << std::endl;
    return EXIT_FAILURE; // Found a mistake
  }
  printf("Testing find_first_centi_mistake_non_zero_leads...ok\n");
  return EXIT_SUCCESS; // No mistakes found
}

int main() {
  return test_centi_digits(); // No mistakes found
}