#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "counters/bench.h"
// K independent adds on 8 registers, then sub/jnz
#define A1 "addq $1, %[a0]\n\t"
#define A2 A1 "addq $1, %[a1]\n\t"
#define A3 A2 "addq $1, %[a2]\n\t"
#define A4 A3 "addq $1, %[a3]\n\t"
#define A5 A4 "addq $1, %[a4]\n\t"
#define A6 A5 "addq $1, %[a5]\n\t"
#define A7 A6 "addq $1, %[a6]\n\t"
#define A8 A7 "addq $1, %[a7]\n\t"
#define DEF(K) __attribute__((noinline)) uint64_t alu_##K(size_t n) { \
  uint64_t a0=0,a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0; \
  asm volatile(".p2align 6\n\t1:\n\t" A##K "subq $1, %[n]\n\tjnz 1b\n\t" \
  : [a0]"+r"(a0),[a1]"+r"(a1),[a2]"+r"(a2),[a3]"+r"(a3),[a4]"+r"(a4),[a5]"+r"(a5),[a6]"+r"(a6),[a7]"+r"(a7),[n]"+r"(n) : : "cc"); \
  return a0+a1+a2+a3+a4+a5+a6+a7; }
DEF(1) DEF(2) DEF(3) DEF(4) DEF(5) DEF(6) DEF(7) DEF(8)
// same but sub and jnz separated by a nop so they cannot macro-fuse
#define DEFN(K) __attribute__((noinline)) uint64_t alun_##K(size_t n) { \
  uint64_t a0=0,a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0; \
  asm volatile(".p2align 6\n\t1:\n\t" A##K "subq $1, %[n]\n\tnop\n\tjnz 1b\n\t" \
  : [a0]"+r"(a0),[a1]"+r"(a1),[a2]"+r"(a2),[a3]"+r"(a3),[a4]"+r"(a4),[a5]"+r"(a5),[a6]"+r"(a6),[a7]"+r"(a7),[n]"+r"(n) : : "cc"); \
  return a0+a1+a2+a3+a4+a5+a6+a7; }
DEFN(1) DEFN(2) DEFN(3) DEFN(4) DEFN(5) DEFN(6) DEFN(7) DEFN(8)
static counters::bench_parameter params() { counters::bench_parameter p; p.min_repeat = 10; p.min_time_ns = 200'000'000; p.min_time_per_inner_ns = 200'000; return p; }
static void report(const char *name, size_t it, counters::event_aggregate agg) {
  double c = agg.fastest_cycles();
  printf("%-12s cycles/it %6.3f  instr/it %6.3f\n", name, c / it, agg.fastest_instructions() / it);
}
int main() {
  size_t n = 2048; volatile uint64_t sink;
#define RUN(f) report(#f, n, counters::bench([&] { sink = f(n); }, params()));
  RUN(alu_1) RUN(alu_2) RUN(alu_3) RUN(alu_4) RUN(alu_5) RUN(alu_6) RUN(alu_7) RUN(alu_8)
  RUN(alun_1) RUN(alun_2) RUN(alun_3) RUN(alun_4) RUN(alun_5) RUN(alun_6) RUN(alun_7) RUN(alun_8)
}
