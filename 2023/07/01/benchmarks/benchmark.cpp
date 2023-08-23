
#include "performancecounters/benchmarker.h"
#include <charconv>
#include <random>
#include <vector>

extern "C" {
#include "sse_date.h"
}

std::string to_date_string(time_t rawtime) {
  std::string output(16, '\0');
  struct tm *timeinfo = gmtime(&rawtime);
  size_t len = strftime(output.data(), output.size(), "%Y%m%d%H%M%S", timeinfo);
  if (len != 14) {
    printf("len = %zu rawtime = %ld \n", len, rawtime);
    abort();
  }
  output.resize(len);

  return output;
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
    uint64_t t = distribution(generator);
    test_data.emplace_back(to_date_string(t));
    bytes += test_data.back().size();
  }
  uint64_t sum{};
  pretty_print(N, bytes, "sse_parse_time_alt", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   uint32_t t;
                   sse_parse_time_alt(s.data(), &t); // should check error
                   sum += t;
                 }
               }));
  pretty_print(N, bytes, "sse_parse_time", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   uint32_t t;
                   sse_parse_time(s.data(), &t); // should check error
                   sum += t;
                 }
               }));
  pretty_print(N, bytes, "parse_time", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   uint32_t t;
                   parse_time(s.data(), &t); // should check error
                   sum += t;
                 }
               }));

}