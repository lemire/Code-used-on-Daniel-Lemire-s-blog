#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"
#include <arpa/inet.h>
#include "avx512ip.h"
double pretty_print(const std::string &name, size_t num_values,
                    counters::event_aggregate agg) {
  std::print("{:<50} : ", name);
    std::print(" {:9.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
    std::print(" {:9.2f} Mv/s ", double(num_values) * 1000 / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
        std::print(" {:7.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
        std::print(" {:7.2f} c ", agg.fastest_cycles() / double(num_values));
        std::print(" {:7.2f} i ", agg.fastest_instructions() / double(num_values));
        std::print(" {:7.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

std::vector<std::string> generate_random_ipv6_addresses(size_t count) {
    std::vector<std::string> strings;
    strings.reserve(count);

    // High-quality random number generator
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

    char buf[INET6_ADDRSTRLEN];

    for (size_t i = 0; i < count; ++i) {
        // Generate two 64-bit parts → 128-bit IPv6 address
        uint64_t high = dist(gen);
        uint64_t low  = dist(gen);

        struct in6_addr addr{};

        // Fill the 16 bytes in network byte order (big-endian)
        for (int j = 0; j < 8; ++j) {
            addr.s6_addr[j]     = static_cast<uint8_t>(high >> (56 - j * 8));
            addr.s6_addr[j + 8] = static_cast<uint8_t>(low  >> (56 - j * 8));
        }

        // Convert binary to IPv6 string (compressed form)
        if (inet_ntop(AF_INET6, &addr, buf, sizeof(buf)) != nullptr) {
            strings.emplace_back(buf);
        }
    }

    return strings;
}

void collect_benchmark_results(size_t input_size, size_t number_strings) {
  std::vector<std::string> strings = generate_random_ipv6_addresses(number_strings);
  volatile uint64_t counter = 0;

  auto count_classic = [&strings, &counter]() {
    size_t c = 0;
    for (const auto& ip_str : strings) {
        struct in6_addr addr{};
        if (inet_pton(AF_INET6, ip_str.c_str(), &addr) == 1) {
            uint64_t byte_sum = 0;
            // Sum all 16 bytes
            for (int i = 0; i < 16; ++i) {
                byte_sum += addr.s6_addr[i];
            }
            c += byte_sum;
        }
        // Invalid addresses are silently skipped
    }
    counter = c;
  };
  pretty_print("inet_pton", number_strings, counters::bench(count_classic));
  auto count_avx512 = [&strings, &counter]() {
    size_t c = 0;
    for (const auto& ip_str : strings) {
        struct in6_addr addr{};
        if (parse_ipv6_avx512(ip_str.data(), ip_str.size(), addr.s6_addr)) {
            uint64_t byte_sum = 0;
            // Sum all 16 bytes
            for (int i = 0; i < 16; ++i) {
                byte_sum += addr.s6_addr[i];
            }
            c += byte_sum;
        }
        // Invalid addresses are silently skipped
    }
    counter = c;
  };
  pretty_print("AVX-512", number_strings, counters::bench(count_avx512));
}

bool run_tests() {
    // IPv6 examples from the blog post. Each should be accepted by both parsers
    // and produce identical 16-byte results.
    const std::vector<std::string> examples = {
        "2001:0db8:85a3:0000:0000:8a2e:0370:7334",       // full form
        "2001:db8:85a3:0:0:8a2e:370:7334",               // leading zeroes omitted
        "2001:db8:85a3::8a2e:370:7334",                  // double-colon compression
        "::1",                                           // loopback
        "::",                                            // unspecified
        "::ffff:192.168.1.1",                            // IPv4-mapped
        "0000:0000:0000:0000:0000:ffff:255.255.255.255", // longest textual form
    };

    bool all_ok = true;
    for (const auto& s : examples) {
        struct in6_addr ref{};
        struct in6_addr got{};
        int ref_ok = inet_pton(AF_INET6, s.c_str(), &ref);
        int got_ok = parse_ipv6_avx512(s.data(), s.size(), got.s6_addr);

        if (ref_ok != 1) {
            std::print("FAIL  inet_pton rejected {}\n", s);
            all_ok = false;
            continue;
        }
        if (got_ok != 1) {
            std::print("FAIL  AVX-512 rejected   {}\n", s);
            all_ok = false;
            continue;
        }
        if (std::memcmp(ref.s6_addr, got.s6_addr, 16) != 0) {
            std::print("FAIL  result mismatch on {}\n", s);
            all_ok = false;
            continue;
        }
        std::print("ok    {}\n", s);
    }
    return all_ok;
}

int main(int argc, char **argv) {
    if (!run_tests()) {
        std::print("Tests failed; aborting benchmark.\n");
        return 1;
    }
    std::print("\n");
    collect_benchmark_results(1024, 100000);
}
