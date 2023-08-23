
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <fstream>
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

int main(int argc, char **argv) {
  std::vector<std::string> inputs;

  if (argc == 1) {
    printf("You may pass a CSV file from "
           "https://s3-us-west-1.amazonaws.com/umbrella-static/index.html, "
           "e.g., top-1m.csv\n");
    printf(
        "wget "
        "http://s3-us-west-1.amazonaws.com/umbrella-static/top-1m.csv.zip\n");
    printf("unzip top-1m.csv.zip\n");
    printf("./benchmark top-1m.csv  \n");
    printf("\nUsing synth. data instead.\n");
    for (size_t i = 0; i < 10000; i++) {
      std::string ref = database[rand() % database.size()];
      ref.reserve(ref.size() + 32);
      inputs.push_back(ref);
    }
  } else {
    std::ifstream input(argv[1]);
    size_t volume = 0;
    for (std::string line; std::getline(input, line);) {
      size_t pos = line.find(',');
      if (pos != std::string::npos) {
        std::string ref = line.substr(pos + 1);
        ref.erase(
            std::find_if(ref.rbegin(), ref.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            ref.end());
        volume += ref.size();
        ref.reserve(ref.size() + 32);
        inputs.push_back(ref);
      }
    }
    std::cout << "loaded " << inputs.size() << " names" << std::endl;
    std::cout << "average length " << double(volume) / inputs.size()
              << " bytes/name" << std::endl;
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
  printf("name_to_dnswire_idx_avx is simdjson-like\n");
  printf("name_to_dnswire_avx is Prefix-Minimum\n");
  printf("name_to_dnswire is conventional\n");
  printf("\n");
  pretty_print(inputs.size(), bytes, "name_to_dnswire_idx_avx",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_idx_avx(s.data(), output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire_avx",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_avx(s.data(), output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire_simd",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_simd(s.data(), output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire_scalar_labels",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire_scalar_labels(s.data(), output.data());
                 }
               }));
  pretty_print(inputs.size(), bytes, "name_to_dnswire",
               bench([&inputs, &output, &sum]() {
                 for (const std::string &s : inputs) {
                   sum += name_to_dnswire(s.data(), output.data());
                 }
               }));

}
