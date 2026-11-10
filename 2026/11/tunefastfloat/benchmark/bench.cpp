// Parses four data sets with fast_float::from_chars, without hints and with
// each performance hint, and reports the time per number. The parsing loops
// are in kernel.cpp. On Linux (and on macOS as root), the counters library
// also reports cycles, instructions and branch misses per number.
#include "kernels.h"

#include <cstdio>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"

namespace {

constexpr size_t count = 100000;

// Price changes: two decimals, random signs, e.g., -731.51 or 42.07.
std::vector<std::string> price_changes() {
  std::mt19937_64 gen(1234);
  std::uniform_real_distribution<double> dis(-1000, 1000);
  std::vector<std::string> out;
  char buffer[64];
  for (size_t i = 0; i < count; i++) {
    std::snprintf(buffer, sizeof(buffer), "%.2f", dis(gen));
    out.emplace_back(buffer);
  }
  return out;
}

// Account balances: two decimals, random signs, but longer, e.g., -731271.51.
std::vector<std::string> account_balances() {
  std::mt19937_64 gen(3456);
  std::uniform_real_distribution<double> dis(-1000000, 1000000);
  std::vector<std::string> out;
  char buffer[64];
  for (size_t i = 0; i < count; i++) {
    std::snprintf(buffer, sizeof(buffer), "%.2f", dis(gen));
    out.emplace_back(buffer);
  }
  return out;
}

// Sensor readings: positive, but of varying shapes, e.g., 7, 1042, 42.5,
// 3.25 or 0.375.
std::vector<std::string> sensor_readings() {
  std::mt19937_64 gen(5678);
  std::uniform_int_distribution<int> shape(0, 3);
  std::uniform_real_distribution<double> unit(0, 1);
  std::vector<std::string> out;
  char buffer[64];
  for (size_t i = 0; i < count; i++) {
    double const u = unit(gen);
    switch (shape(gen)) {
    case 0:
      std::snprintf(buffer, sizeof(buffer), "%d", int(u * 10000));
      break;
    case 1:
      std::snprintf(buffer, sizeof(buffer), "%.1f", u * 1000);
      break;
    case 2:
      std::snprintf(buffer, sizeof(buffer), "%.2f", u * 100);
      break;
    default:
      std::snprintf(buffer, sizeof(buffer), "%.3f", u * 10);
      break;
    }
    out.emplace_back(buffer);
  }
  return out;
}

// Regular prices: always the same shape and positive, e.g., 12.34.
std::vector<std::string> regular_prices() {
  std::mt19937_64 gen(9012);
  std::uniform_real_distribution<double> dis(10, 99.99);
  std::vector<std::string> out;
  char buffer[64];
  for (size_t i = 0; i < count; i++) {
    std::snprintf(buffer, sizeof(buffer), "%.2f", dis(gen));
    out.emplace_back(buffer);
  }
  return out;
}

struct config {
  const char *name;
  double (*sum)(const std::vector<std::string> &);
};

const config configs[] = {{"no hint", sum_none},
                          {"unpredictable_sign", sum_sign},
                          {"short_numbers", sum_short},
                          {"assume_round_to_nearest", sum_nearest},
                          {"sign + nearest", sum_sign_nearest},
                          {"short + nearest", sum_short_nearest},
                          {"all three", sum_all}};

volatile double sink;

void run(const char *name, const std::vector<std::string> &numbers) {
  double const expected = sum_none(numbers);
  for (const config &c : configs) {
    if (c.sum(numbers) != expected) {
      std::printf("%s: %s gives a different sum\n", name, c.name);
      std::exit(EXIT_FAILURE);
    }
  }
  for (const config &c : configs) {
    counters::event_aggregate agg =
        counters::bench([&] { sink = c.sum(numbers); });
    double const n = double(numbers.size());
    double const ns = agg.fastest_elapsed_ns() / n;
    std::printf("%-18s %-24s %7.2f ns %8.1f M/s", name, c.name, ns, 1000 / ns);
    if (counters::has_performance_counters()) {
      std::printf(" %7.1f c %7.1f i %5.2f bm", agg.fastest_cycles() / n,
                  agg.fastest_instructions() / n,
                  agg.fastest_branch_misses() / n);
    }
    std::printf("\n");
  }
}

} // namespace

int main() {
  std::printf("# counters: %s\n",
              counters::has_performance_counters() ? "yes" : "no");
  std::printf("# %zu numbers per data set, the sums are checked\n", count);
  std::printf("# data set, hints, ns per number, millions of numbers per "
              "second [, cycles, instructions, branch misses per number]\n");
  run("price_changes", price_changes());
  run("account_balances", account_balances());
  run("sensor_readings", sensor_readings());
  run("regular_prices", regular_prices());
  return EXIT_SUCCESS;
}
