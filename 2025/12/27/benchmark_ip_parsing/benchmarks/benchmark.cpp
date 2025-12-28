#include "boost/asio/ip/address.hpp"
#include "counters/bench.h"
#include "fast_float/fast_float.h"
#include <atomic>
#include <charconv>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <random>
#include <string>
#include <expected>

enum class parse_error { invalid_format };

void pretty_print(size_t volume, size_t bytes, std::string name,
                  counters::event_aggregate agg) {
  if (agg.inner_count > 1) {
    printf("# (inner count: %d)\n", agg.inner_count);
  }
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Mip/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/ip ", agg.fastest_elapsed_ns() / volume);
  if (counters::event_collector().has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/ip ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/ip ", agg.fastest_instructions() / volume);
    printf(" %5.2f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

fastfloat_really_inline const char *seek_ip_end(const char *p,
                                                const char *pend) {
  const char *current = p;
  size_t count = 0;
  for (; current != pend; ++current) {
    if (*current == '.') {
      count++;
      if (count == 3) {
        ++current;
        break;
      }
    }
  }
  while (current != pend) {
    if (*current <= '9' && *current >= '0') {
      ++current;
    } else {
      break;
    }
  }
  return current;
}

fastfloat_really_inline std::expected<uint32_t, parse_error> parse_boost_asio(const char *p, const char *pend) {
  boost::system::error_code ec;
  auto addr = boost::asio::ip::make_address(std::string_view(p, pend - p), ec);
  if (!ec) {
    return addr.to_v4().to_uint();
  } else {
    return std::unexpected(parse_error::invalid_format);
  }
}

fastfloat_really_inline  std::expected<uint32_t, parse_error> parse_manual(const char *p, const char *pend) {
uint32_t ip = 0;
    int octets = 0;
    while (p < pend && octets < 4) {
        uint32_t val = 0;
        const char *start = p;
        while (p < pend && *p >= '0' && *p <= '9') {
            val = val * 10 + (*p - '0');
            if (val > 255) return std::unexpected(parse_error::invalid_format);
            p++;
        }
        if (p == start) return std::unexpected(parse_error::invalid_format);
        if (p - start > 1 && *start == '0') return std::unexpected(parse_error::invalid_format);

        ip = (ip << 8) | val;
        octets++;

        if (octets < 4) {
            if (p == pend || *p != '.') return std::unexpected(parse_error::invalid_format);
            p++; // Skip dot
        }
    }
    if (octets == 4) {
        return ip;
    } else {
        return std::unexpected(parse_error::invalid_format);
    }
}

fastfloat_really_inline std::expected<uint32_t, parse_error> parse_manual_unrolled(const char *p, const char *pend) {
    uint32_t ip = 0;
    int octets = 0;
    while (p < pend && octets < 4) {
        uint32_t val = 0;
        if (p < pend && *p >= '0' && *p <= '9') {
            val = (*p++ - '0');
            if (p < pend && *p >= '0' && *p <= '9') {
                if (val == 0) { return std::unexpected(parse_error::invalid_format); }
                val = val * 10 + (*p++ - '0');
                if (p < pend && *p >= '0' && *p <= '9') {
                    val = val * 10 + (*p++ - '0');
                    if (val > 255) { return std::unexpected(parse_error::invalid_format); }
                }
            }
        } else {
            return std::unexpected(parse_error::invalid_format);
        }
        ip = (ip << 8) | val;
        octets++;
        if (octets < 4) {
            if (p == pend || *p != '.') {
              return std::unexpected(parse_error::invalid_format);
            }
            p++; // Skip the dot
        }
    }
    if (octets == 4) {
        return ip;
    } else {
        return std::unexpected(parse_error::invalid_format);
    }
}

enum class parse_method { standard, fast_float };

template <parse_method use_standard>
fastfloat_really_inline std::expected<uint32_t, parse_error>
simple_parse_ip_line(const char *p, const char *pend) {
  const char *current = p;
  uint32_t ip = 0;
  for (int i = 0; i < 4; ++i) {
    uint8_t value;
    if constexpr (use_standard == parse_method::standard) {
      auto r = std::from_chars(current, pend, value);
      if (r.ec != std::errc()) {
        return std::unexpected(parse_error::invalid_format);
      }
      current = r.ptr;
    } else if constexpr (use_standard == parse_method::fast_float) {
      auto r = fast_float::from_chars(current, pend, value);
      if (r.ec != std::errc()) {
        return std::unexpected(parse_error::invalid_format);
      }
      current = r.ptr;
    }
    ip = (ip << 8) | value;
    if (i < 3) {
      if (current == pend || *current++ != '.') {
        return std::unexpected(parse_error::invalid_format);
      }
    }
  }
  return ip;
}

static std::string make_ip_line(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  std::string s;
  s.reserve(16);
  s += std::to_string(a);
  s += '.';
  s += std::to_string(b);
  s += '.';
  s += std::to_string(c);
  s += '.';
  s += std::to_string(d);
  s += '\n';
  return s;
}

int main() {
  constexpr size_t N = 15000;
  std::mt19937 rng(1234);
  std::uniform_int_distribution<int> dist(0, 255);

  std::string buf;
  constexpr size_t ip_size = 16;
  buf.reserve(N * ip_size);

  for (size_t i = 0; i < N; ++i) {
    uint8_t a = (uint8_t)dist(rng);
    uint8_t b = (uint8_t)dist(rng);
    uint8_t c = (uint8_t)dist(rng);
    uint8_t d = (uint8_t)dist(rng);
    std::string ip_line = make_ip_line(a, b, c, d);
    ip_line.resize(ip_size, ' '); // pad to fixed size
    buf.append(ip_line);
  }

  // sentinel to allow 4-byte loads at end
  buf.append(4, '\0');

  const size_t bytes = buf.size() - 4; // exclude sentinel from throughput
  const size_t volume = N;

  volatile uint32_t sink = 0;
  std::string buffer(ip_size * N, ' ');

  pretty_print(volume, bytes, "memcpy baseline", counters::bench([&]() {
                 std::memcpy((char *)buffer.data(), buf.data(), bytes);
               }));

  pretty_print(volume, bytes, "just_seek_ip_end (no parse)",
               counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 int ok = 0;
                 for (size_t i = 0; i < N; ++i) {
                   const char *q = seek_ip_end(p, pend);
                   sum += (uint32_t)(q - p);
                   p += ip_size;
                 }
                 sink += sum;
               }));

  pretty_print(volume, bytes, "parse_ip_std_fromchars", counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 int ok = 0;
                 for (size_t i = 0; i < N; ++i) {
                   auto res = simple_parse_ip_line<parse_method::standard>(p, pend);
                   if (!res) {
                     std::abort();
                   }
                   sum += res.value();
                   p += ip_size;
                 }
                 sink += sum;
               }));

  pretty_print(volume, bytes, "parse_ip_fastfloat", counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 int ok = 0;
                 for (size_t i = 0; i < N; ++i) {
                   auto res = simple_parse_ip_line<parse_method::fast_float>(p, pend);
                   if (!res) {
                     std::abort();
                   }
                   sum += res.value();
                   p += ip_size;
                 }
                 sink += sum;
               }));
// commented out, we don't report Boost.Asio results anyhow
/*  pretty_print(volume, bytes, "parse_boost_asio", counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 for (size_t i = 0; i < N; ++i) {
                   auto res = parse_boost_asio(p, p + 16);
                   if (!res) {
                     std::abort();
                   }
                   sum += res.value();
                   p += ip_size;
                 }
                 sink += sum;
               }));*/

  pretty_print(volume, bytes, "parse_manual", counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 for (size_t i = 0; i < N; ++i) {
                   auto res = parse_manual(p, p + 16);
                   if (!res) {
                     std::abort();
                   }
                   sum += res.value();
                   p += ip_size;
                 }
                 sink += sum;
               }));
  pretty_print(volume, bytes, "parse_manual_unrolled", counters::bench([&]() {
                 const char *p = buf.data();
                 const char *pend = buf.data() + bytes;
                 uint32_t sum = 0;
                 for (size_t i = 0; i < N; ++i) {
                   auto res =  parse_manual_unrolled(p, p + 16);
                   if (!res) {
                     std::abort();
                   }
                   sum += res.value();
                   p += ip_size;
                 }
                 sink += sum;
               }));
  return EXIT_SUCCESS;
}