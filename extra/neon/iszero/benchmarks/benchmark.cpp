
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <arm_neon.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <vector>

int veq_non_zero_max(uint8x16_t v) {
  return vmaxvq_u32(vreinterpretq_u32_u8(v)) != 0;
}

int veq_non_zero_mov(uint8x16_t v) {
  return vget_lane_u64(vqmovn_u64(vreinterpretq_u64_u8(v)), 0) != 0;
}

int veq_non_zero_narrow(uint8x16_t v) {
  return vget_lane_u64(vshrn_n_u16(vreinterpretq_u16_u8(v), 4), 0) != 0;
}

int veq_non_zero_float(uint8x16_t v) {
  uint8x8_t narrowed = vshrn_n_u16(vreinterpretq_u16_u8(v), 4);
  return (vdupd_lane_f64(vreinterpret_f64_u16(narrowed), 0) != 0.0);
}

template <typename F> int scan(uint8_t *input, size_t length, F f) {
  int result = 0;
  for (size_t i = 0; i + 16 <= length; i += 16) {
    uint8x16_t v = vld1q_u8(input + i);
    result++;
    i += f(v);
  }
  return result;
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main(int argc, char **argv) {
  std::vector<uint8_t> data(1000000);
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> dis(
      0, (std::numeric_limits<uint8_t>::max)());
  std::ranges::generate(data, [&]() { return dis(gen); });
  size_t count = scan(data.data(), data.size(), veq_non_zero_max);
  size_t volume = count * 16;
  volatile size_t counter = 0;
  printf("# check: %d %d %d %d\n",
         scan(data.data(), data.size(), veq_non_zero_max),
         scan(data.data(), data.size(), veq_non_zero_mov),
         scan(data.data(), data.size(), veq_non_zero_narrow),
         scan(data.data(), data.size(), veq_non_zero_float));
  for (size_t trial = 0; trial < 10; trial++) {
    printf("Trial %zu\n", trial + 1);

    pretty_print(count, volume, "veq_non_zero_max", bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_max);
                 }));
    pretty_print(count, volume, "veq_non_zero_mov", bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_mov);
                 }));
    pretty_print(
        count, volume, "veq_non_zero_narrow", bench([&data, &counter]() {
          counter = counter + scan(data.data(), data.size(), veq_non_zero_mov);
        }));
    pretty_print(count, volume, "veq_non_zero_float",
                 bench([&data, &counter]() {
                   counter = counter +
                             scan(data.data(), data.size(), veq_non_zero_float);
                 }));
  }
}
