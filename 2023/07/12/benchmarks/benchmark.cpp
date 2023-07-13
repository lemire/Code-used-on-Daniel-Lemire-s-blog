
#include "performancecounters/benchmarker.h"
#include <charconv>
#include <random>
#include <vector>

extern "C" {
#include "sse_type.h"
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
  std::vector<std::string> source = {
      "A",          "A6",      "AAAA",     "AFSDB",      "APL",    "CAA",
      "CDS",        "CDNSKEY", "CERT",     "CH",         "CNAME",  "CS",
      "CSYNC",      "DHCID",   "DLV",      "DNAME",      "DNSKEY", "DS",
      "EUI48",      "EUI64",   "GPOS",     "HINFO",      "HIP",    "HS",
      "HTTPS",      "IN",      "IPSECKEY", "ISDN",       "KEY",    "KX",
      "L32",        "L64",     "LOC",      "LP",         "MB",     "MD",
      "MF",         "MG",      "MINFO",    "MR",         "MX",     "NAPTR",
      "NID",        "NS",      "NSAP",     "NSAP-PTR",   "NSEC",   "NSEC3",
      "NSEC3PARAM", "NULL",    "NXT",      "OPENPGPKEY", "PTR",    "PX",
      "RP",         "RRSIG",   "RT",       "SIG",        "SMIMEA", "SOA",
      "SPF",        "SRV",     "SSHFP",    "SVCB",       "TLSA",   "TXT",
      "URI",        "WKS",     "X25",      "ZONEMD"};
  if (!collector.has_events()) {
    printf("Performance counters unavalaible. Consider running in privileged "
           "mode (e.g., sudo).\n");
  }
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0, source.size() - 1);
  std::vector<std::string> test_data;
  size_t N = 20000;
  size_t bytes = 0;
  while (test_data.size() < N) {
    uint64_t t = distribution(generator);
    std::string s = source[t];
    bytes += s.size();
    s.resize(16);
    test_data.push_back(s);
  }
  uint64_t sum{};
  pretty_print(N, bytes, "sse_type", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   uint8_t t;
                   sse_type(s.data(), &t); // should check error
                   sum += t;
                 }
               }));
}