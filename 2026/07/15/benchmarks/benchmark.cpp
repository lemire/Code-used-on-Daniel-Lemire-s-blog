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
#include "serialize_ipv4_variants.h"
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

// ---------------------------------------------------------------------------
// Full 256-entry LUT, branchless single 4-byte store.
// Layout per entry (little-endian): [d0, d1, d2, len]. A 4-byte memcpy places
// the 3 left-aligned digit chars, then we advance by len. Writes one scratch
// byte past the digits, so the destination needs 1 byte of slack (fine here:
// the next '.'/NUL overwrites it, buf is 16 bytes).
// ---------------------------------------------------------------------------
namespace lut256 {
constexpr std::array<uint32_t, 256> make_u8_table() noexcept {
  std::array<uint32_t, 256> t{};
  for (uint32_t v = 0; v < 256; ++v) {
    uint32_t len = v >= 100 ? 3 : v >= 10 ? 2 : 1;
    char d[3] = {0, 0, 0};
    uint32_t x = v;
    for (uint32_t i = len; i-- > 0;) { d[i] = static_cast<char>('0' + x % 10); x /= 10; }
    uint32_t w = static_cast<uint8_t>(d[0])
               | (static_cast<uint32_t>(static_cast<uint8_t>(d[1])) << 8)
               | (static_cast<uint32_t>(static_cast<uint8_t>(d[2])) << 16)
               | (len << 24);
    t[v] = w;
  }
  return t;
}
constexpr auto u8_table = make_u8_table();

inline char* write_u8(char* point, uint8_t value) {
  uint32_t w = u8_table[value];
  std::memcpy(point, &w, 4);
  return point + (w >> 24);
}
size_t ipv4(uint64_t address, char* out) {
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
} // namespace lut256

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

  auto count_lut256 = [&values, &counter]() {
    size_t c = 0;
    char buf[16];
    for (uint32_t x : values) {
        size_t n = lut256::ipv4(ntohl(x), buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("lut256 ipv4 (256-LUT, branchless)", number_values,
               counters::bench(count_lut256));

  auto count_maskht = [&values, &counter]() {
    size_t c = 0;
    char buf[16];
    for (uint32_t x : values) {
        size_t n = ipv4_to_string_maskht(x, buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4_to_string_maskht (mask from ht)", number_values,
               counters::bench(count_maskht));

  auto count_x4 = [&values, &counter]() {
    size_t c = 0;
    char buf[64];
    for (size_t i = 0; i + 4 <= values.size(); i += 4) {
        size_t n = ipv4x4_to_string(&values[i], buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4x4_to_string (batched 4, compress)", number_values,
               counters::bench(count_x4));

  auto count_x4fixed = [&values, &counter]() {
    size_t c = 0;
    char buf[64];
    for (size_t i = 0; i + 4 <= values.size(); i += 4) {
        size_t n = ipv4x4_to_string_fixed(&values[i], buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4x4_to_string_fixed (no compress)", number_values,
               counters::bench(count_x4fixed));

  auto count_gather = [&values, &counter]() {
    size_t c = 0;
    char buf[16];
    for (uint32_t x : values) {
        size_t n = ipv4_to_string_gather(x, buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4_to_string_gather (gather+compress)", number_values,
               counters::bench(count_gather));

  auto count_x4gather = [&values, &counter]() {
    size_t c = 0;
    char buf[64];
    for (size_t i = 0; i + 4 <= values.size(); i += 4) {
        size_t n = ipv4x4_to_string_gather(&values[i], buf);
        c += n + (uint8_t)buf[0];
    }
    counter = c;
  };
  pretty_print("ipv4x4_to_string_gather (batched gather)", number_values,
               counters::bench(count_x4gather));
}

// Verify each variant against the reference ipv4_to_string over a wide sample.
static void verify_variants() {
  std::mt19937_64 gen(999);
  std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
  char ref[16];
  for (size_t iter = 0; iter < 2'000'000; ++iter) {
    uint32_t x = (iter < 256 * 4)
        ? (uint32_t(iter % 256) << (8 * (iter / 256 % 4)))
        : dist(gen);
    size_t rn = ipv4_to_string(x, ref);

    // Variant A: same output as reference.
    char a[16];
    size_t an = ipv4_to_string_maskht(x, a);
    if (an != rn || std::memcmp(a, ref, rn) != 0) {
      std::print("maskht MISMATCH x={:#x}: ref='{:.{}}' got='{:.{}}'\n",
                 x, ref, (int)rn, a, (int)an);
      std::exit(1);
    }

    // Variant G1: gather, same output as reference.
    char g[16];
    size_t gn1 = ipv4_to_string_gather(x, g);
    if (gn1 != rn || std::memcmp(g, ref, rn) != 0) {
      std::print("gather MISMATCH x={:#x}: ref='{:.{}}' got='{:.{}}'\n",
                 x, ref, (int)rn, g, (int)gn1);
      std::exit(1);
    }
  }
  // Variant B: batched; each lane == reference + '\n'.
  for (size_t iter = 0; iter < 500'000; ++iter) {
    uint32_t ip[4];
    for (auto& z : ip) z = dist(gen);
    char got[64];
    size_t gn = ipv4x4_to_string(ip, got);
    // Rebuild the expected concatenation.
    char exp[64]; size_t off = 0;
    for (int k = 0; k < 4; ++k) {
      size_t rn = ipv4_to_string(ip[k], exp + off);
      off += rn;
      exp[off++] = '\n';
    }
    if (gn != off || std::memcmp(got, exp, off) != 0) {
      std::print("x4 MISMATCH at iter {}\n", iter);
      std::exit(1);
    }

    // Variant G4: batched gather must match ipv4x4_to_string byte for byte.
    char gg[64];
    size_t ggn = ipv4x4_to_string_gather(ip, gg);
    if (ggn != off || std::memcmp(gg, exp, off) != 0) {
      std::print("x4-gather MISMATCH at iter {}\n", iter);
      std::exit(1);
    }
  }
  std::print("variants verified against ipv4_to_string: OK\n");
}

int main(int argc, char **argv) {
    verify_variants();
    collect_ipv4_serialize_benchmark(100000);
}
