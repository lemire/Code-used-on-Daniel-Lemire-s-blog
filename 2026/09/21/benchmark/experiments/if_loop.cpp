// A loop with an if inside it.
//
// That is two branches per iteration: the compare, and the loop back-edge.
// If both are taken, a core that can retire two taken branches per cycle
// should run at about one cycle per iteration. A core that can only take
// one should take about two cycles.
//
// A tiny `if (x) hits++` does not do this: clang turns it into `cinc` /
// `cmov` and you are left with the loop branch alone. The store below is
// enough to keep a real branch. `[[likely]]` lays the "hit" path out as
// fall-through, so a miss is a taken skip. Feed it an array that never
// hits, and both branches are taken every iteration.
//
//   clang++ -O3 -std=c++20 -march=native \
//     -I build/_deps/counters-src/include \
//     experiments/if_loop.cpp -o /tmp/if_loop
//   sudo /tmp/if_loop

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "counters/bench.h"

__attribute__((noinline)) uint8_t last_hit(const uint8_t *p, size_t n,
                                           uint8_t thresh) {
  volatile uint8_t last = 0;
#if defined(__clang__)
#pragma clang loop unroll(disable) vectorize(disable)
#elif defined(__GNUC__)
#pragma GCC unroll 1
#endif
  do {
    if (*p > thresh) [[likely]]
      last = *p;
    p++;
  } while (--n);
  return last;
}

static counters::bench_parameter params() {
  counters::bench_parameter p;
  p.min_repeat = 10;
  p.min_time_ns = 200'000'000;
  p.min_time_per_inner_ns = 200'000;
  return p;
}

static void report(const char *name, size_t iters,
                   counters::event_aggregate agg) {
  if (counters::has_performance_counters()) {
    double c = agg.fastest_cycles();
    printf("%-12s cycles/iter %6.3f  taken/cyc %6.3f\n", name, c / iters,
           agg.fastest_branches() / c);
  } else {
    printf("%-12s ns/iter %8.4f  (%6.3f cycles at 4.4 GHz)\n", name,
           agg.fastest_elapsed_ns() / iters,
           agg.fastest_elapsed_ns() / iters * 4.4);
  }
}

int main() {
  const size_t n = 4096;
  uint8_t *low = (uint8_t *)aligned_alloc(64, n);
  uint8_t *high = (uint8_t *)aligned_alloc(64, n);
  for (size_t i = 0; i < n; i++) {
    low[i] = 1;    // never > 128: inner branch taken (skip), loop taken
    high[i] = 255; // always > 128: inner not-taken, only the loop is taken
  }

  volatile uint8_t sink;
  auto p = params();
  report("always miss", n,
         counters::bench([&] { sink = last_hit(low, n, 128); }, p));
  report("always hit", n,
         counters::bench([&] { sink = last_hit(high, n, 128); }, p));
  return sink ? 0 : 1;
}
