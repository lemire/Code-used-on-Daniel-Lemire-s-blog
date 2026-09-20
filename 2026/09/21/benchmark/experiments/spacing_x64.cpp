#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "counters/bench.h"
#define STR2(x) #x
#define STR(x) STR2(x)
#define J(S) "jmp 2f\n\t.skip " STR(S) ", 0x90\n\t2:\n\t"
#define J2(S) J(S) J(S)
#define J4(S) J2(S) J2(S)
#define J8(S) J4(S) J4(S)
#define J16(S) J8(S) J8(S)
#define TAIL "subq $1, %[n]\n\tjnz 1b\n\t"
#define DEF(S) __attribute__((noinline)) void jumps_##S(size_t n) { asm volatile(".p2align 6\n\t1:\n\t" J16(S) TAIL : [n] "+r"(n) : : "cc"); }
DEF(1) DEF(2) DEF(6) DEF(14) DEF(22) DEF(30) DEF(38) DEF(46) DEF(62) DEF(126)
#define DEFA(A) __attribute__((noinline)) uint64_t sum4_##A(const uint64_t *p, size_t n) { \
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0; \
  asm volatile(".p2align " #A "\n\t1:\n\t" \
               "addq (%[p]), %[a0]\n\taddq 8(%[p]), %[a1]\n\taddq 16(%[p]), %[a2]\n\taddq 24(%[p]), %[a3]\n\t" \
               "addq $32, %[p]\n\tsubq $4, %[n]\n\tjnz 1b\n\t" \
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3), [p] "+r"(p), [n] "+r"(n) : : "cc", "memory"); \
  return a0 + a1 + a2 + a3; }
DEFA(2) DEFA(3) DEFA(4) DEFA(5) DEFA(6)
// 4x with separate load+add (no memory operand)
__attribute__((noinline)) uint64_t sum4_sep(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, t0, t1, t2, t3;
  asm volatile(".p2align 6\n\t1:\n\t"
               "movq (%[p]), %[t0]\n\tmovq 8(%[p]), %[t1]\n\tmovq 16(%[p]), %[t2]\n\tmovq 24(%[p]), %[t3]\n\t"
               "addq %[t0], %[a0]\n\taddq %[t1], %[a1]\n\taddq %[t2], %[a2]\n\taddq %[t3], %[a3]\n\t"
               "addq $32, %[p]\n\tsubq $4, %[n]\n\tjnz 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3), [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3), [p] "+r"(p), [n] "+r"(n) : : "cc", "memory");
  return a0 + a1 + a2 + a3; }
static counters::bench_parameter params() { counters::bench_parameter p; p.min_repeat = 10; p.min_time_ns = 200'000'000; p.min_time_per_inner_ns = 200'000; return p; }
static void report(const char *name, size_t it, counters::event_aggregate agg) {
  double c = agg.fastest_cycles();
  printf("%-20s cycles/it %8.3f  instr/it %8.3f  br/cyc %6.3f\n", name, c / it, agg.fastest_instructions() / it, agg.fastest_branches() / c);
}
int main() {
  size_t n = 2048;
  uint64_t *p = (uint64_t*)aligned_alloc(64, n * 8); for (size_t i = 0; i < n; i++) p[i] = i * 7;
  volatile uint64_t sink;
#define RUNJ(S) report("jumps skip " #S, n * 17, counters::bench([&] { jumps_##S(n); }, params()));
  RUNJ(1) RUNJ(2) RUNJ(6) RUNJ(14) RUNJ(22) RUNJ(30) RUNJ(38) RUNJ(46) RUNJ(62) RUNJ(126)
#define RUNA(A) report("sum4 p2align " #A, n / 4, counters::bench([&] { sink = sum4_##A(p, n); }, params()));
  RUNA(2) RUNA(3) RUNA(4) RUNA(5) RUNA(6)
  report("sum4 sep load/add", n / 4, counters::bench([&] { sink = sum4_sep(p, n); }, params()));
}
