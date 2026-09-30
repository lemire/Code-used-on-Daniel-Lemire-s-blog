// Throughput of UTF-8 to UTF-16 with U+FFFD replacement.
// Build: c++ -O3 -std=c++17 -I include bench.cpp libsimdutf.a -o bench
#include "simdutf.h"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

static volatile uint64_t sink;

static uint64_t checksum(const char16_t *p, size_t n) {
  uint64_t h = n;
  if (n != 0) {
    h += p[0];
    h += p[n / 2];
    h += p[n - 1];
  }
  return h;
}

// Scalar WHATWG decoder with a 16-byte ASCII probe, native endianness.
static size_t scalar_whatwg(const char *data, size_t len, char16_t *out) {
  size_t i = 0;
  size_t written = 0;
  uint32_t cp = 0;
  size_t needed = 0;
  size_t seen = 0;
  uint8_t lo = 0x80;
  uint8_t hi = 0xBF;
  auto store = [&](uint32_t c) {
    if (c >= 0x10000) {
      c -= 0x10000;
      out[written++] = char16_t(0xD800 + (c >> 10));
      out[written++] = char16_t(0xDC00 + (c & 0x3FF));
    } else {
      out[written++] = char16_t(c);
    }
  };
  while (i < len) {
    if (needed == 0 && i + 16 <= len) {
      uint64_t v1, v2;
      __builtin_memcpy(&v1, data + i, 8);
      __builtin_memcpy(&v2, data + i + 8, 8);
      if (((v1 | v2) & 0x8080808080808080ULL) == 0) {
        for (size_t k = 0; k < 16; k++) {
          store(uint8_t(data[i + k]));
        }
        i += 16;
        continue;
      }
    }
    const uint8_t b = uint8_t(data[i]);
    if (needed == 0) {
      i++;
      lo = 0x80;
      hi = 0xBF;
      if (b < 0x80) {
        store(b);
      } else if (b >= 0xC2 && b <= 0xDF) {
        needed = 1;
        cp = b & 0x1F;
      } else if (b <= 0xEF) {
        if (b == 0xE0) {
          lo = 0xA0;
        } else if (b == 0xED) {
          hi = 0x9F;
        }
        needed = 2;
        cp = b & 0x0F;
      } else if (b <= 0xF4) {
        if (b == 0xF0) {
          lo = 0x90;
        } else if (b == 0xF4) {
          hi = 0x8F;
        }
        needed = 3;
        cp = b & 0x07;
      } else {
        store(0xFFFD);
      }
      continue;
    }
    if (b < lo || b > hi) {
      needed = 0;
      seen = 0;
      cp = 0;
      lo = 0x80;
      hi = 0xBF;
      store(0xFFFD);
      continue;
    }
    i++;
    lo = 0x80;
    hi = 0xBF;
    cp = (cp << 6) | (b & 0x3F);
    if (++seen == needed) {
      store(cp);
      needed = 0;
      seen = 0;
      cp = 0;
    }
  }
  if (needed != 0) {
    store(0xFFFD);
  }
  return written;
}

template <class F> static double best_gbs(size_t bytes, F fn) {
  fn();
  int iters = 1;
  for (;;) {
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < iters; i++) {
      fn();
    }
    const double sec = std::chrono::duration<double>(
                           std::chrono::steady_clock::now() - t0)
                           .count();
    if (sec >= 0.05 || iters >= 200000) {
      double best = (double(bytes) * iters) / sec;
      for (int r = 0; r < 7; r++) {
        t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < iters; i++) {
          fn();
        }
        const double s = std::chrono::duration<double>(
                             std::chrono::steady_clock::now() - t0)
                             .count();
        const double g = (double(bytes) * iters) / s;
        if (g > best) {
          best = g;
        }
      }
      return best / 1e9;
    }
    iters *= 2;
  }
}

static std::string load(const std::string &path) {
  std::ifstream in(path, std::ios::binary);
  if (!in) {
    return {};
  }
  return std::string((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
}

static void punch(std::string &s, size_t stride) {
  if (stride == 0) {
    return;
  }
  for (size_t i = 0; i < s.size(); i += stride) {
    s[i] = char(0xFF);
  }
}

// `count` isolated 0xFF bytes, each surrounded by ASCII, so one punch is one
// maximal subpart. A broken multibyte character would otherwise be several.
static void punch_isolated(std::string &s, size_t count) {
  if (count == 0 || s.size() < 8) {
    return;
  }
  size_t stride = s.size() / count;
  if (stride < 8) {
    stride = 8;
  }
  for (size_t k = 0; k < count; k++) {
    size_t i = k * stride + 3;
    if (i + 4 >= s.size()) {
      break;
    }
    s[i - 3] = ' ';
    s[i - 2] = ' ';
    s[i - 1] = ' ';
    s[i] = char(0xFF);
    s[i + 1] = ' ';
    s[i + 2] = ' ';
    s[i + 3] = ' ';
  }
}

static void report(const char *file, const char *kind, const char *op,
                   double gbs) {
  std::printf("%s %s %s %.3f\n", file, kind, op, gbs);
  std::fflush(stdout);
}

static void bench_buffer(const char *file, const char *kind, const std::string &input,
                         bool valid) {
  std::vector<char16_t> out(input.size() + 8);
  const char *p = input.data();
  const size_t n = input.size();
  const simdutf::utf8_to_utf16_result plan =
      simdutf::utf16_length_from_utf8_with_replacement(p, n);
  std::printf("plan %s %s errors %zu more %d\n", file, kind, plan.error_count,
              int(plan.more_errors));
  report(file, kind, "scalar", best_gbs(n, [&] {
           const size_t w = scalar_whatwg(p, n, out.data());
           sink = checksum(out.data(), w);
         }));
  // The length result is computed once, outside the timer. Conversion then
  // uses those error locations.
  report(file, kind, "replacement", best_gbs(n, [&] {
           const size_t w = simdutf::convert_utf8_to_utf16_with_replacement(
               p, n, out.data(), plan);
           sink = checksum(out.data(), w);
         }));
  report(file, kind, "pair_replacement", best_gbs(n, [&] {
           const auto len =
               simdutf::utf16_length_from_utf8_with_replacement(p, n);
           const size_t w = simdutf::convert_utf8_to_utf16_with_replacement(
               p, n, out.data(), len);
           sink = checksum(out.data(), w) + len.count;
         }));
  if (!valid) {
    return;
  }
  report(file, kind, "plain", best_gbs(n, [&] {
           const size_t w = simdutf::convert_utf8_to_utf16(p, n, out.data());
           sink = checksum(out.data(), w);
         }));
  report(file, kind, "pair_plain", best_gbs(n, [&] {
           const size_t len = simdutf::utf16_length_from_utf8(p, n);
           const size_t w = simdutf::convert_utf8_to_utf16(p, n, out.data());
           sink = checksum(out.data(), w) + len;
         }));
}

int main(int argc, char **argv) {
  const std::string dir = argc > 1 ? argv[1] : "/home/dlemire/wikimars";
  const char *names[] = {"english", "chinese", "arabic", "hindi",
                         "japanese", "russian", "emoji"};
  const auto impl = simdutf::get_active_implementation()->name();
  std::printf("impl %.*s\n", int(impl.size()), impl.data());
  for (const char *name : names) {
    std::string text = load(dir + "/" + name + ".txt");
    if (text.empty()) {
      std::printf("missing %s\n", name);
      continue;
    }
    if (text.size() < (1u << 20)) {
      const std::string once = text;
      while (text.size() < (1u << 20)) {
        text += once;
      }
    }
    std::printf("file %s bytes %zu\n", name, text.size());
    bench_buffer(name, "valid", text, true);
    std::string few = text;
    punch_isolated(few, 8);
    bench_buffer(name, "eight", few, false);
    std::string sparse = text;
    punch(sparse, 4096);
    bench_buffer(name, "per4k", sparse, false);
    std::string dense = text;
    punch(dense, 64);
    bench_buffer(name, "per64", dense, false);
  }
  return 0;
}
