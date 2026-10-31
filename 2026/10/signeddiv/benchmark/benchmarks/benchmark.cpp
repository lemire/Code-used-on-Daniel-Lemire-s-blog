// Signed 32-bit division on a 64-bit system.
//
// 1. Constant divisor: the compiler (x / d) versus div32<d>(x).
// 2. Divisor known only at runtime: the division instruction (x / d) versus
//    fastmod::fastdiv_s32, before (0.1.0) and after (0.2.0) the change.
//
// throughput: out[i] = f(in[i]), independent operations
// latency:    acc = f(in[i] ^ (acc & 1)), each division waits for the previous one
#include <cstdint>
#include <cstdlib>
#include <print>
#include <random>
#include <string>
#include <vector>

#include "counters/bench.h"
#include "div32.h"
#include "fastmod.h"

// fastdiv_s32 as it was in fastmod 0.1.0, for comparison.
namespace fastmod_010 {
inline uint64_t mul128_u32(uint64_t lowbits, uint32_t d) {
  return ((__uint128_t)lowbits * d) >> 64;
}
inline uint64_t mul128_from_u64(uint64_t lowbits, uint64_t d) {
  return ((__uint128_t)lowbits * d) >> 64;
}
inline uint64_t mul128_s32(uint64_t lowbits, int32_t d) {
  if (d < 0) {
    return mul128_from_u64(lowbits, (int64_t)d) - lowbits;
  }
  return mul128_u32(lowbits, d);
}
inline int32_t fastdiv_s32(int32_t a, uint64_t M, int32_t d) {
  uint64_t highbits = mul128_s32(M, a);
  highbits += (a < 0 ? 1 : 0);
  if (d < 0)
    return -(int32_t)(highbits);
  return (int32_t)(highbits);
}
} // namespace fastmod_010

void pretty_print(const std::string &name, size_t n, counters::event_aggregate agg) {
  std::print("{:<48} : {:6.3f} ns ", name, agg.fastest_elapsed_ns() / double(n));
  if (counters::has_performance_counters()) {
    std::print(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::print(" {:6.2f} c ", agg.fastest_cycles() / double(n));
    std::print(" {:6.2f} i ", agg.fastest_instructions() / double(n));
  }
  std::print("\n");
}

template <class F>
void run(const std::string &name, const std::vector<int32_t> &in, std::vector<int32_t> &out,
         F f) {
  const int32_t *pin = in.data();
  int32_t *pout = out.data();
  size_t n = in.size();
  pretty_print(name + " (throughput)", n, counters::bench([=]() {
    for (size_t i = 0; i < n; i++) {
      pout[i] = f(pin[i]);
    }
  }));
  pretty_print(name + " (latency)", n, counters::bench([=]() {
    int32_t acc = 0;
    for (size_t i = 0; i < n; i++) {
      acc = f(int32_t(pin[i] ^ (acc & 1)));
    }
    pout[0] = acc;
  }));
}

template <int32_t d> void constant(const std::vector<int32_t> &in, std::vector<int32_t> &out) {
  std::string sfx = " d=" + std::to_string(d);
  run("compiler x / d" + sfx, in, out, [](int32_t x) { return x / d; });
  run("div32" + sfx, in, out, [](int32_t x) { return div32<d>(x); });
}

void runtime(int32_t d, const std::vector<int32_t> &in, std::vector<int32_t> &out) {
  std::string sfx = " d=" + std::to_string(d);
  uint64_t M = fastmod::computeM_s32(d);
  run("instruction x / d" + sfx, in, out, [d](int32_t x) { return x / d; });
  run("fastmod 0.1.0" + sfx, in, out,
      [M, d](int32_t x) { return fastmod_010::fastdiv_s32(x, M, d); });
  run("fastmod 0.2.0" + sfx, in, out,
      [M, d](int32_t x) { return fastmod::fastdiv_s32(x, M, d); });
}

int main(int argc, char **argv) {
  const size_t n = 16384;
  std::mt19937_64 rng(1234);
  std::vector<int32_t> in(n), out(n);
  for (auto &x : in) {
    x = int32_t(rng()); // random signs
  }
  std::print("# constant divisor\n");
  constant<7>(in, out);    // the compiler needs a 33-bit multiplier
  constant<60>(in, out);   // the compiler needs a 33-bit multiplier
  constant<365>(in, out);  // the compiler needs a 33-bit multiplier
  constant<1000>(in, out); // the compiler uses a 32-bit multiplier
  std::print("# divisor known at runtime\n");
  // from the command line so that the compiler cannot see it
  int32_t d = argc > 1 ? std::atoi(argv[1]) : 7;
  runtime(d, in, out);
}
