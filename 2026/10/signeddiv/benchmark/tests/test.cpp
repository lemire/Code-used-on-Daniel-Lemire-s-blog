// Checks div32<d>(x) == x / d for every 32-bit x, for many divisors.
#include <cstdint>
#include <cstdio>
#include <utility>

#include "div32.h"

int failures = 0;

template <int32_t d> void test() {
  if constexpr (d > 1 && (d & (d - 1)) != 0) {
    for (int64_t x = INT32_MIN; x <= INT32_MAX; x++) {
      if (div32<d>(int32_t(x)) != int32_t(x) / d) {
        if (failures++ < 10) std::printf("div32<%d>(%lld) failed\n", d, (long long)x);
      }
    }
  }
}

template <int32_t... is> void test_range(std::integer_sequence<int32_t, is...>) {
  (test<3 + is>(), ...);
}

int main() {
  test_range(std::make_integer_sequence<int32_t, 98>{}); // 3 to 100
  test<365>(); test<641>(); test<1000>(); test<3600>(); test<86400>();
  test<1000000007>(); test<1073741825>(); test<2147483647>();
  std::printf(failures ? "FAILED\n" : "all tests passed\n");
  return failures != 0;
}
