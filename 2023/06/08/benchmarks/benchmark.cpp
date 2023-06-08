
#include "performancecounters/benchmarker.h"
#include <charconv>
#include <random>
#include <vector>

extern "C" {
#include "sse_inet_aton.h"
}
#include <arpa/inet.h>



// This serialises in host order (little endian)
// https://lemire.me/blog/2023/02/01/serializing-ips-quickly-in-c/
std::string ipv4(const uint64_t address) {
  std::string output(16, '\0');
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

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-30s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/a ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/a ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/a ", agg.fastest_instructions() / volume);
    printf(" %5.1f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main() {
  if (!collector.has_events()) {
    printf("Performance counters unavalaible. Consider running in privileged "
           "mode (e.g., sudo).\n");
  }
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0, 0xffffffff);
  std::vector<std::string> test_data;
  size_t N = 20000;
  size_t bytes = 0;
  while (test_data.size() < N) {
    uint64_t ip = distribution(generator);
    test_data.emplace_back(ipv4(ip));
    bytes += test_data.back().size();
  }
  uint64_t sum{};
  pretty_print(N, bytes, "sse_inet_aton", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                  uint32_t ipv4;
                   sse_inet_aton(s.data(), s.size(), &ipv4); // should check error
                   sum += ipv4;
                 }
               }));
  pretty_print(N, bytes, "inet_pton", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                  uint32_t ipv4;
                  inet_pton(AF_INET, s.c_str(), &ipv4); // should check error
                  sum += ipv4;
                 }
               }));
}