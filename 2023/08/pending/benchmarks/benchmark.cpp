
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <vector>

extern "C" {
#include "name_to_dnswire.h"
}

std::vector<std::string> database = {
    "google.com",  "live.com",    "netflix.com", "office.com",  "bing.com",
    "apple.com",   "office.net",  "skype.com",   "netflix.net", "msn.com",
    "lencr.org",   "icloud.com",  "youtube.com", "windows.net", "aaplimg.com",
    "c.lencr.org", "outlook.com", "windows.com", "msedge.net",  "yahoo.com",
    "ytimg.com",   "amazon.com",  "pki.goog",    "fbcdn.net",   "azure.com",
    "adnxs.com",   "demdex.net",  "gvt2.com",    "gvt1.com",    "sentry.io",
    "c.bing.com",  "ggpht.com",   "sfx.ms",      "adsrvr.org",  "adobe.com",
    "g.live.com",  "ntp.org",     "criteo.com",  "e2ro.com",    "licdn.com",
};

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
  for (size_t i = 0; i < 10000; i++) {
    std::string ref = database[rand() % database.size()];
    ref.reserve(32);
    inputs.push_back(ref);
  }
  if (!collector.has_events()) {
    printf("Performance counters unavailable. Consider running in privileged "
           "mode (e.g., sudo).\n");
  }
  size_t bytes = 0;
  for (const std::string &s : inputs) {
    bytes += s.size();
  }

  std::vector<uint8_t> output;
  output.resize(1024);
  volatile uint64_t sum{};

  pretty_print(inputs.size(), bytes, "name_to_dnswire_simd_fast",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_simd_fast(s.data(),
                                               output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire_simd",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_simd(s.data(),
                                               output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire(s.data(),
                                               output.data());
                 }
               }));
}
