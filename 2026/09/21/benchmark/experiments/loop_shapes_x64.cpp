#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "counters/bench.h"
#define DEF(name, body, ...) __attribute__((noinline)) uint64_t name(const uint64_t *p, size_t n) { \
  uint64_t a0 = 0, t0 = 0; asm volatile(".p2align 6\n\t1:\n\t" body : [a0] "+r"(a0), [t0] "+r"(t0), [p] "+r"(p), [n] "+r"(n) : : "cc", "memory"); return a0; }
DEF(fused,   "addq (%[p]), %[a0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(sep,     "movq (%[p]), %[t0]\n\taddq %[t0], %[a0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(noload,  "addq %[p], %[a0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(loadonly,"movq (%[p]), %[t0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(idx,     "addq (%[p],%[n],8), %[a0]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(three,   "addq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(four_alu,"addq $1, %[a0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(five_alu,"addq $1, %[a0]\n\taddq $1, %[t0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(fused_nop, "addq (%[p]), %[a0]\n\taddq $8, %[p]\n\tnop\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(fused_2nop, "addq (%[p]), %[a0]\n\taddq $8, %[p]\n\tnop\n\tnop\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(six_alu, "addq $1, %[a0]\n\taddq $1, %[t0]\n\taddq $2, %[t0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(seven_alu, "addq $1, %[a0]\n\taddq $1, %[t0]\n\taddq $2, %[t0]\n\taddq $3, %[a0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(eight_alu, "addq $1, %[a0]\n\taddq $1, %[t0]\n\taddq $2, %[t0]\n\taddq $3, %[a0]\n\taddq $4, %[t0]\n\taddq $8, %[p]\n\tsubq $1, %[n]\n\tjnz 1b\n\t")
DEF(dec_jnz, "addq (%[p]), %[a0]\n\taddq $8, %[p]\n\tdecq %[n]\n\tjnz 1b\n\t")
static counters::bench_parameter params() { counters::bench_parameter p; p.min_repeat = 10; p.min_time_ns = 200'000'000; p.min_time_per_inner_ns = 200'000; return p; }
static void report(const char *name, size_t it, counters::event_aggregate agg) {
  double c = agg.fastest_cycles();
  printf("%-12s cycles/it %6.3f  instr/it %6.3f\n", name, c / it, agg.fastest_instructions() / it);
}
int main() {
  size_t n = 2048; uint64_t *p = (uint64_t*)aligned_alloc(64, n * 8); for (size_t i = 0; i < n; i++) p[i] = i;
  volatile uint64_t sink;
#define RUN(f) report(#f, n, counters::bench([&] { sink = f(p, n); }, params()));
  RUN(fused) RUN(sep) RUN(noload) RUN(loadonly) RUN(idx) RUN(three) RUN(four_alu) RUN(five_alu) RUN(dec_jnz) RUN(fused_nop) RUN(fused_2nop) RUN(six_alu) RUN(seven_alu) RUN(eight_alu)
}
