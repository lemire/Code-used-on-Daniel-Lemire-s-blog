// Measures the cost of subnormal (denormal) floating-point numbers.
//
// For each kernel we run the same code over four kinds of inputs:
//   normal      : values in [0.5, 1)
//   mixed       : normal values, except that one value in 100 is subnormal
//   subnormal   : values in the subnormal range (exponent field is zero)
//   subnormal+FTZ/DAZ : same subnormal data, but the processor is told
//                 to flush subnormals to zero (x86: FTZ+DAZ, ARM: FZ)
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <format>
#include <print>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"

#if defined(__x86_64__) || defined(_M_X64)
#include <immintrin.h>
// FTZ is bit 15 of MXCSR, DAZ is bit 6.
static void set_flush_subnormals(bool on) {
  unsigned csr = _mm_getcsr();
  if (on) {
    csr |= 0x8040;
  } else {
    csr &= ~0x8040u;
  }
  _mm_setcsr(csr);
}
static bool flush_subnormals_enabled() { return (_mm_getcsr() & 0x8040) != 0; }
#elif defined(__aarch64__)
// FZ is bit 24 of FPCR (FZ16, bit 19, only matters for half precision).
static void set_flush_subnormals(bool on) {
  uint64_t fpcr;
  asm volatile("mrs %0, fpcr" : "=r"(fpcr));
  if (on) {
    fpcr |= (uint64_t(1) << 24);
  } else {
    fpcr &= ~(uint64_t(1) << 24);
  }
  asm volatile("msr fpcr, %0" : : "r"(fpcr));
}
static bool flush_subnormals_enabled() {
  uint64_t fpcr;
  asm volatile("mrs %0, fpcr" : "=r"(fpcr));
  return (fpcr & (uint64_t(1) << 24)) != 0;
}
#else
#error "unsupported architecture"
#endif

template <typename T> struct traits;
template <> struct traits<double> {
  using bits = uint64_t;
  static constexpr int mantissa_bits = 52;
  static constexpr const char *name = "double";
  // multiplying a value in [0.5,1) by this constant gives a subnormal
  static constexpr double tiny = 0x1p-1030;
};
template <> struct traits<float> {
  using bits = uint32_t;
  static constexpr int mantissa_bits = 23;
  static constexpr const char *name = "float";
  static constexpr float tiny = 0x1p-140f;
};

// A random subnormal: zero exponent field, random mantissa. We clear the
// top mantissa bit so that the sum of two such values is still subnormal.
template <typename T> T random_subnormal(std::mt19937_64 &rng) {
  using bits = typename traits<T>::bits;
  bits mask = (bits(1) << (traits<T>::mantissa_bits - 1)) - 1;
  bits b = bits(rng()) & mask;
  if (b == 0) {
    b = 1;
  }
  return std::bit_cast<T>(b);
}

template <typename T> T random_normal(std::mt19937_64 &rng) {
  std::uniform_real_distribution<T> dist(T(0.5), T(1.0));
  return dist(rng);
}

enum class mode { normal, mixed, subnormal, subnormal_ftz };

static const char *mode_name(mode m) {
  switch (m) {
  case mode::normal:
    return "normal";
  case mode::mixed:
    return "1% subnormal";
  case mode::subnormal:
    return "subnormal";
  case mode::subnormal_ftz:
    return "subnormal+FTZ";
  }
  return "?";
}

template <typename T> std::vector<T> make_input(size_t n, mode m, uint64_t seed) {
  std::mt19937_64 rng(seed);
  std::vector<T> v(n);
  for (size_t i = 0; i < n; i++) {
    switch (m) {
    case mode::normal:
      v[i] = random_normal<T>(rng);
      break;
    case mode::mixed:
      v[i] = (i % 100 == 50) ? random_subnormal<T>(rng) : random_normal<T>(rng);
      break;
    case mode::subnormal:
    case mode::subnormal_ftz:
      v[i] = random_subnormal<T>(rng);
      break;
    }
  }
  return v;
}

double pretty_print(const std::string &name, size_t num_values,
                    counters::event_aggregate agg) {
  std::print("{:<32} : ", name);
  std::print(" {:7.3f} ns ", agg.fastest_elapsed_ns() / double(num_values));
  std::print(" {:6.2f} Gv/s ", double(num_values) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:8.2f} c ", agg.fastest_cycles() / double(num_values));
    std::print(" {:7.2f} i ", agg.fastest_instructions() / double(num_values));
    std::print(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::print("\n");
  return double(num_values) / agg.fastest_elapsed_ns();
}

template <typename T> void run(size_t n) {
  std::print("=== {} ({} values per array) ===\n", traits<T>::name, n);
  for (mode m : {mode::normal, mode::mixed, mode::subnormal, mode::subnormal_ftz}) {
    set_flush_subnormals(m == mode::subnormal_ftz);
    std::vector<T> a = make_input<T>(n, m, 1);
    std::vector<T> b = make_input<T>(n, m, 2);
    std::vector<T> normal = make_input<T>(n, mode::normal, 3);
    std::vector<T> out(n);
    const T *pa = a.data();
    const T *pb = b.data();
    const T *pn = normal.data();
    T *po = out.data();
    std::string suffix = std::string(" [") + mode_name(m) + "]";

    // subnormal in, subnormal out
    auto mul = [pa, po, n]() {
      for (size_t i = 0; i < n; i++) {
        po[i] = pa[i] * T(0.75);
      }
    };
    pretty_print("multiply by 0.75" + suffix, n, counters::bench(mul));

    // subnormal in, subnormal out
    auto add = [pa, pb, po, n]() {
      for (size_t i = 0; i < n; i++) {
        po[i] = pa[i] + pb[i];
      }
    };
    pretty_print("add two arrays" + suffix, n, counters::bench(add));

    // subnormal in, subnormal (or, for the tiniest values, zero) out
    auto div = [pa, po, n]() {
      for (size_t i = 0; i < n; i++) {
        po[i] = pa[i] / T(3.0);
      }
    };
    pretty_print("divide by 3" + suffix, n, counters::bench(div));

    // normal in, subnormal out (the inputs are always normal here; only
    // the product lands in the subnormal range when the mode says so)
    T factor = (m == mode::normal) ? T(0.75) : traits<T>::tiny;
    auto mul_out = [pn, po, n, factor]() {
      for (size_t i = 0; i < n; i++) {
        po[i] = pn[i] * factor;
      }
    };
    // in normal mode this is just another multiplication by 0.75 and it
    // serves as the baseline for the subnormal-output rows
    if (m != mode::mixed) {
      pretty_print((m == mode::normal ? "normal in, normal product"
                                      : "normal in, subnormal product") +
                       suffix,
                   n, counters::bench(mul_out));
    }

    // dependent chain: x *= 0.9999, starting either in the normal range
    // or in the subnormal range; the value stays in that range throughout
    if (m != mode::mixed) {
      T start = (m == mode::normal) ? T(1.0) : std::bit_cast<T>(
          typename traits<T>::bits(1) << (traits<T>::mantissa_bits - 2));
      volatile T sink = 0;
      auto decay = [start, &sink, n]() {
        T x = start;
        for (size_t i = 0; i < n; i++) {
          x *= T(0.9999);
        }
        sink = x;
      };
      pretty_print("decay chain x *= 0.9999" + suffix, n,
                   counters::bench(decay));
      // sanity: the value must still be in the expected range
      T last = sink;
      if (m == mode::subnormal && std::fpclassify(last) != FP_SUBNORMAL) {
        std::print("warning: decay chain left the subnormal range ({})\n",
                   double(last));
      }
    }
    // sanity check of the multiply kernel's outputs
    mul();
    if (m == mode::subnormal && std::fpclassify(po[0]) != FP_SUBNORMAL) {
      std::print("warning: multiply output is not subnormal\n");
    }
    if (m == mode::subnormal_ftz && po[0] != T(0)) {
      std::print("warning: FTZ did not flush the product to zero\n");
    }
    set_flush_subnormals(false);
  }
}

int main(int argc, char **argv) {
  size_t n = 16384; // 128 KB of doubles: fits in L2
  if (argc > 1) {
    n = std::stoull(argv[1]);
  }
  std::print("flush-to-zero at startup: {}\n",
             flush_subnormals_enabled() ? "enabled" : "disabled");
  run<double>(n);
  run<float>(n);
  return 0;
}
