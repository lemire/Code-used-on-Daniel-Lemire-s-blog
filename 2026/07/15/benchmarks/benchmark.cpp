#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "counters/bench.h"
#include <arpa/inet.h>
#include "serialize_ipv4.h"
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


namespace ada_url {

// Scalar reference serializer from ada-url, included here for comparison.
// Digit pair LUT for fast decimal write: index 0..99 -> two chars.
constexpr std::array<char, 200> make_digit_pairs() noexcept {
  std::array<char, 200> t{};
  for (size_t i = 0; i < 100; ++i) {
    t[i * 2] = static_cast<char>('0' + i / 10);
    t[i * 2 + 1] = static_cast<char>('0' + i % 10);
  }
  return t;
}

constexpr auto digit_pairs = make_digit_pairs();

// Writes the decimal representation of an octet (0..255); returns the new
// write position. Reconstructed to match ada-url's digit_pairs LUT usage.
inline char* write_u8(char* point, uint8_t value) {
  if (value < 10) {
    *point = static_cast<char>('0' + value);
    return point + 1;
  } else if (value < 100) {
    std::memcpy(point, &digit_pairs[value * 2], 2);
    return point + 2;
  }
  *point = static_cast<char>('0' + value / 100);
  std::memcpy(point + 1, &digit_pairs[(value % 100) * 2], 2);
  return point + 3;
}

// out needs at least 16 bytes; NUL-terminated on return. Returns the length.
size_t ipv4(const uint64_t address, char* out) {
  char* point = out;
  point = write_u8(point, static_cast<uint8_t>(address));
  *point++ = '.';
  point = write_u8(point, static_cast<uint8_t>(address >> 8));
  *point++ = '.';
  point = write_u8(point, static_cast<uint8_t>(address >> 16));
  *point++ = '.';
  point = write_u8(point, static_cast<uint8_t>(address >> 24));
  *point = '\0';
  return static_cast<size_t>(point - out);
}

} // namespace ada_url

void collect_ipv4_serialize_benchmark(size_t number_values) {
  // Random 32-bit values to serialize (stored as four network-order octets).
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
  std::vector<uint32_t> values(number_values);
  for (auto& x : values) { x = htonl(dist(gen)); }
  volatile uint64_t counter = 0;

  // Consume the length and first byte so the work cannot be optimized away.
  auto count_ntop = [&values, &counter]() {
    size_t c = 0;
    char buf[INET_ADDRSTRLEN];
    for (uint32_t x : values) {
        struct in_addr addr{};
        addr.s_addr = x;
        if (inet_ntop(AF_INET, &addr, buf, sizeof(buf)) != nullptr) {
            c += std::strlen(buf) + (uint8_t)buf[0];
        }
    }
    counter = c;
  };
  pretty_print("inet_ntop", number_values, counters::bench(count_ntop));
  auto count_avx512 = [&values, &counter]() {
    size_t c = 0;
    char buf[16];
    for (uint32_t x : values) {
        size_t n = ipv4_to_string(x, buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4_to_string (AVX-512 compress)", number_values, counters::bench(count_avx512));
  auto count_ada = [&values, &counter]() {
    size_t c = 0;
    char buf[16];
    for (uint32_t x : values) {
        // ada's ipv4() takes a host-order integer (first octet in the high
        // byte); values are network order, so byte-swap to feed the same address.
        size_t n = ada_url::ipv4(ntohl(x), buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ada ipv4 (LUT, char*)", number_values, counters::bench(count_ada));
}

int main(int argc, char **argv) {
    collect_ipv4_serialize_benchmark(100000);
}
