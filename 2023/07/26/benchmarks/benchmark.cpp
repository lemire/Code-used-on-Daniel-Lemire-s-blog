
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <random>
#include <vector>
#include <stdlib.h>

extern "C" {
#include "base16.h"
}

int b32_pton(const char *src, uint8_t *target, size_t tsize) {
  char ch;
  size_t p = 0;

  memset(target, '\0', tsize);
  while ((ch = *src++)) {
    uint8_t d;
    size_t b;
    size_t n;

    if (p + 5 >= tsize * 8)
      return -1;

    if (isspace((unsigned char)ch))
      continue;

    if (ch >= '0' && ch <= '9')
      d = ch - '0';
    else if (ch >= 'A' && ch <= 'V')
      d = ch - 'A' + 10;
    else if (ch >= 'a' && ch <= 'v')
      d = ch - 'a' + 10;
    else
      return -1;

    b = 7 - p % 8;
    n = p / 8;

    if (b >= 4)
      target[n] |= d << (b - 4);
    else {
      target[n] |= d >> (4 - b);
      target[n + 1] |= d << (b + 4);
    }
    p += 5;
  }
  return (p + 7) / 8;
}
void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-30s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.1f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main() {

  std::vector<std::string> inputs;
  for(size_t i = 0; i < 10000; i++) {
    std::string new_str;
    new_str.resize((rand()%24)&~1);
    for(size_t z = 0; z < new_str.size(); z++) {
      int digit = rand()%16;
      new_str[z] = (digit < 10 ? '0' + digit : 'A' + (digit - 10) );
    }
    inputs.push_back(new_str);
  }
  if (!collector.has_events()) {
    printf("Performance counters unavailable. Consider running in privileged "
           "mode (e.g., sudo).\n");
  }
  size_t bytes = 0;
  for (const std::string &s : inputs) {
    bytes += s.size();
  }
  std::string output;
  output.resize(20 + 16);
  volatile uint64_t sum{};

  pretty_print(inputs.size(), bytes, "base16hex_simd",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += base16hex_simd((uint8_t *)output.data(),
                                         (const uint8_t *)s.c_str());
                 }
               }));
  pretty_print(inputs.size(), bytes, "base16hex_simd_geoff",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += base16hex_simd_geoff((uint8_t *)output.data(),
                                         (const uint8_t *)s.c_str());
                 }
               }));
  pretty_print(inputs.size(), bytes, "base16hex_simple",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += base16hex_simple((uint8_t *)output.data(),
                                           (const uint8_t *)s.c_str());
                 }
               }));
}
