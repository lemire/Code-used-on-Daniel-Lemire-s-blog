#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "counters/bench.h"
#define STR2(x) #x
#define STR(x) STR2(x)
// jump over S nops (4 bytes each)
#define J(S) "b 2f\n\t.rept " STR(S) "\n\tnop\n\t.endr\n\t2:\n\t"
#define J2(S) J(S) J(S)
#define J4(S) J2(S) J2(S)
#define J8(S) J4(S) J4(S)
#define J16(S) J8(S) J8(S)
#define TAIL "subs %[n], %[n], #1\n\tb.ne 1b\n\t"
#define DEF(S) __attribute__((noinline)) void jumps_##S(size_t n) { asm volatile(".p2align 6\n\t1:\n\t" J16(S) TAIL : [n] "+r"(n) : : "cc"); }
DEF(7) DEF(8) DEF(9) DEF(10) DEF(11) DEF(12) DEF(13) DEF(14) DEF(15)
static counters::bench_parameter params() { counters::bench_parameter p; p.min_repeat = 10; p.min_time_ns = 200'000'000; p.min_time_per_inner_ns = 200'000; return p; }
static void report(const char *name, size_t it, counters::event_aggregate agg) {
  if (counters::has_performance_counters()) {
    double c = agg.fastest_cycles();
    printf("%-20s cycles/it %8.3f  instr/it %8.3f  br/cyc %6.3f\n", name, c / it, agg.fastest_instructions() / it, agg.fastest_branches() / c);
  } else {
    printf("%-20s ns/it %8.4f  (%6.3f cycles at 4.51 GHz)\n", name, agg.fastest_elapsed_ns() / it, agg.fastest_elapsed_ns() / it * 4.51);
  }
}
int main() {
  size_t n = 2048;
#define RUNJ(S) report("jumps skip " #S " nops", n * 17, counters::bench([&] { jumps_##S(n); }, params()));
  RUNJ(7) RUNJ(8) RUNJ(9) RUNJ(10) RUNJ(11) RUNJ(12) RUNJ(13) RUNJ(14) RUNJ(15)
}
