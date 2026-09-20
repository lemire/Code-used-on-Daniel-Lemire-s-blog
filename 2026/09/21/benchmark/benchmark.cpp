// How many taken branches per cycle can a processor retire? We write tight
// loops in assembly (ARM64 and x64), with one taken branch per iteration,
// and vary how much work each iteration does. If the taken-branch rate is
// the bottleneck, the loop takes the same number of cycles per iteration no
// matter how much work we put in it, until some other limit is hit.
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "counters/bench.h"

// ---------------------------------------------------------------------------
// Sum of an array of 64-bit integers, in assembly, unrolled U times. Each
// iteration ends with a single conditional branch, taken except for the last
// one. The unrolled versions use U independent accumulators (at most 8) so
// that the addition latency is not a bottleneck. The length n must be a
// positive multiple of 16.
// ---------------------------------------------------------------------------
#if defined(__aarch64__)

__attribute__((noinline)) uint64_t sum_asm_1(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, t0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "ldr %[t0], [%[p]], #8\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "subs %[n], %[n], #1\n\t"
               "b.ne 1b\n\t"
               : [a0] "+r"(a0), [t0] "=&r"(t0), [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return a0;
}

__attribute__((noinline)) uint64_t sum_asm_2(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, t0, t1;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "ldp %[t0], %[t1], [%[p]], #16\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "add %[a1], %[a1], %[t1]\n\t"
               "subs %[n], %[n], #2\n\t"
               "b.ne 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [t0] "=&r"(t0), [t1] "=&r"(t1),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return a0 + a1;
}

__attribute__((noinline)) uint64_t sum_asm_4(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, t0, t1, t2, t3;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "ldp %[t0], %[t1], [%[p]]\n\t"
               "ldp %[t2], %[t3], [%[p], #16]\n\t"
               "add %[p], %[p], #32\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "add %[a1], %[a1], %[t1]\n\t"
               "add %[a2], %[a2], %[t2]\n\t"
               "add %[a3], %[a3], %[t3]\n\t"
               "subs %[n], %[n], #4\n\t"
               "b.ne 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return (a0 + a1) + (a2 + a3);
}

__attribute__((noinline)) uint64_t sum_asm_8(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
  uint64_t t0, t1, t2, t3, t4, t5, t6, t7;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "ldp %[t0], %[t1], [%[p]]\n\t"
               "ldp %[t2], %[t3], [%[p], #16]\n\t"
               "ldp %[t4], %[t5], [%[p], #32]\n\t"
               "ldp %[t6], %[t7], [%[p], #48]\n\t"
               "add %[p], %[p], #64\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "add %[a1], %[a1], %[t1]\n\t"
               "add %[a2], %[a2], %[t2]\n\t"
               "add %[a3], %[a3], %[t3]\n\t"
               "add %[a4], %[a4], %[t4]\n\t"
               "add %[a5], %[a5], %[t5]\n\t"
               "add %[a6], %[a6], %[t6]\n\t"
               "add %[a7], %[a7], %[t7]\n\t"
               "subs %[n], %[n], #8\n\t"
               "b.ne 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [a4] "+r"(a4), [a5] "+r"(a5), [a6] "+r"(a6), [a7] "+r"(a7),
                 [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3),
                 [t4] "=&r"(t4), [t5] "=&r"(t5), [t6] "=&r"(t6), [t7] "=&r"(t7),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return ((a0 + a1) + (a2 + a3)) + ((a4 + a5) + (a6 + a7));
}

__attribute__((noinline)) uint64_t sum_asm_16(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
  uint64_t t0, t1, t2, t3, t4, t5, t6, t7;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "ldp %[t0], %[t1], [%[p]]\n\t"
               "ldp %[t2], %[t3], [%[p], #16]\n\t"
               "ldp %[t4], %[t5], [%[p], #32]\n\t"
               "ldp %[t6], %[t7], [%[p], #48]\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "add %[a1], %[a1], %[t1]\n\t"
               "add %[a2], %[a2], %[t2]\n\t"
               "add %[a3], %[a3], %[t3]\n\t"
               "add %[a4], %[a4], %[t4]\n\t"
               "add %[a5], %[a5], %[t5]\n\t"
               "add %[a6], %[a6], %[t6]\n\t"
               "add %[a7], %[a7], %[t7]\n\t"
               "ldp %[t0], %[t1], [%[p], #64]\n\t"
               "ldp %[t2], %[t3], [%[p], #80]\n\t"
               "ldp %[t4], %[t5], [%[p], #96]\n\t"
               "ldp %[t6], %[t7], [%[p], #112]\n\t"
               "add %[p], %[p], #128\n\t"
               "add %[a0], %[a0], %[t0]\n\t"
               "add %[a1], %[a1], %[t1]\n\t"
               "add %[a2], %[a2], %[t2]\n\t"
               "add %[a3], %[a3], %[t3]\n\t"
               "add %[a4], %[a4], %[t4]\n\t"
               "add %[a5], %[a5], %[t5]\n\t"
               "add %[a6], %[a6], %[t6]\n\t"
               "add %[a7], %[a7], %[t7]\n\t"
               "subs %[n], %[n], #16\n\t"
               "b.ne 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [a4] "+r"(a4), [a5] "+r"(a5), [a6] "+r"(a6), [a7] "+r"(a7),
                 [t0] "=&r"(t0), [t1] "=&r"(t1), [t2] "=&r"(t2), [t3] "=&r"(t3),
                 [t4] "=&r"(t4), [t5] "=&r"(t5), [t6] "=&r"(t6), [t7] "=&r"(t7),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return ((a0 + a1) + (a2 + a3)) + ((a4 + a5) + (a6 + a7));
}

// A loop that does nothing but count down: two instructions, one of them a
// taken branch. It measures the bare cost of a taken branch.
__attribute__((noinline)) void empty_loop(size_t n) {
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "subs %[n], %[n], #1\n\t"
               "b.ne 1b\n\t"
               : [n] "+r"(n)
               :
               : "cc");
}

// The same loop with K unconditional taken branches added to its body. With
// the loop branch itself, the body has K+1 taken branches. The "dense" jumps
// skip a single nop, so there is a taken branch every 8 bytes; the "sparse"
// jumps skip 15 nops, so there is one taken branch per 64-byte cache line.
#define JUMP_DENSE "b 2f\n\tnop\n\t2:\n\t"
#define JUMP_SPARSE "b 2f\n\t.rept 15\n\tnop\n\t.endr\n\t2:\n\t"
#define LOOP_TAIL "subs %[n], %[n], #1\n\tb.ne 1b\n\t"

#elif defined(__x86_64__)

__attribute__((noinline)) uint64_t sum_asm_1(const uint64_t *p, size_t n) {
  uint64_t a0 = 0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "addq (%[p]), %[a0]\n\t"
               "addq $8, %[p]\n\t"
               "subq $1, %[n]\n\t"
               "jnz 1b\n\t"
               : [a0] "+r"(a0), [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return a0;
}

__attribute__((noinline)) uint64_t sum_asm_2(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "addq (%[p]), %[a0]\n\t"
               "addq 8(%[p]), %[a1]\n\t"
               "addq $16, %[p]\n\t"
               "subq $2, %[n]\n\t"
               "jnz 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return a0 + a1;
}

__attribute__((noinline)) uint64_t sum_asm_4(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "addq (%[p]), %[a0]\n\t"
               "addq 8(%[p]), %[a1]\n\t"
               "addq 16(%[p]), %[a2]\n\t"
               "addq 24(%[p]), %[a3]\n\t"
               "addq $32, %[p]\n\t"
               "subq $4, %[n]\n\t"
               "jnz 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return (a0 + a1) + (a2 + a3);
}

__attribute__((noinline)) uint64_t sum_asm_8(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "addq (%[p]), %[a0]\n\t"
               "addq 8(%[p]), %[a1]\n\t"
               "addq 16(%[p]), %[a2]\n\t"
               "addq 24(%[p]), %[a3]\n\t"
               "addq 32(%[p]), %[a4]\n\t"
               "addq 40(%[p]), %[a5]\n\t"
               "addq 48(%[p]), %[a6]\n\t"
               "addq 56(%[p]), %[a7]\n\t"
               "addq $64, %[p]\n\t"
               "subq $8, %[n]\n\t"
               "jnz 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [a4] "+r"(a4), [a5] "+r"(a5), [a6] "+r"(a6), [a7] "+r"(a7),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return ((a0 + a1) + (a2 + a3)) + ((a4 + a5) + (a6 + a7));
}

__attribute__((noinline)) uint64_t sum_asm_16(const uint64_t *p, size_t n) {
  uint64_t a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0;
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "addq (%[p]), %[a0]\n\t"
               "addq 8(%[p]), %[a1]\n\t"
               "addq 16(%[p]), %[a2]\n\t"
               "addq 24(%[p]), %[a3]\n\t"
               "addq 32(%[p]), %[a4]\n\t"
               "addq 40(%[p]), %[a5]\n\t"
               "addq 48(%[p]), %[a6]\n\t"
               "addq 56(%[p]), %[a7]\n\t"
               "addq 64(%[p]), %[a0]\n\t"
               "addq 72(%[p]), %[a1]\n\t"
               "addq 80(%[p]), %[a2]\n\t"
               "addq 88(%[p]), %[a3]\n\t"
               "addq 96(%[p]), %[a4]\n\t"
               "addq 104(%[p]), %[a5]\n\t"
               "addq 112(%[p]), %[a6]\n\t"
               "addq 120(%[p]), %[a7]\n\t"
               "addq $128, %[p]\n\t"
               "subq $16, %[n]\n\t"
               "jnz 1b\n\t"
               : [a0] "+r"(a0), [a1] "+r"(a1), [a2] "+r"(a2), [a3] "+r"(a3),
                 [a4] "+r"(a4), [a5] "+r"(a5), [a6] "+r"(a6), [a7] "+r"(a7),
                 [p] "+r"(p), [n] "+r"(n)
               :
               : "cc", "memory");
  return ((a0 + a1) + (a2 + a3)) + ((a4 + a5) + (a6 + a7));
}

__attribute__((noinline)) void empty_loop(size_t n) {
  asm volatile(".p2align 6\n\t"
               "1:\n\t"
               "subq $1, %[n]\n\t"
               "jnz 1b\n\t"
               : [n] "+r"(n)
               :
               : "cc");
}

// A short jmp is 2 bytes: the dense jumps give a taken branch every 3 bytes,
// the sparse ones a taken branch every 64 bytes.
#define JUMP_DENSE "jmp 2f\n\tnop\n\t2:\n\t"
#define JUMP_SPARSE "jmp 2f\n\t.skip 62, 0x90\n\t2:\n\t"
#define LOOP_TAIL "subq $1, %[n]\n\tjnz 1b\n\t"

#else
#error "unsupported architecture"
#endif

#define JUMPS_1(J) J
#define JUMPS_2(J) J J
#define JUMPS_4(J) JUMPS_2(J) JUMPS_2(J)
#define JUMPS_8(J) JUMPS_4(J) JUMPS_4(J)
#define JUMPS_16(J) JUMPS_8(J) JUMPS_8(J)

#define DEFINE_JUMP_LOOP(name, K, J)                                          \
  __attribute__((noinline)) void name##_##K(size_t n) {                       \
    asm volatile(".p2align 6\n\t1:\n\t" JUMPS_##K(J) LOOP_TAIL                \
                 : [n] "+r"(n)                                                 \
                 :                                                             \
                 : "cc");                                                      \
  }

DEFINE_JUMP_LOOP(dense_jumps, 1, JUMP_DENSE)
DEFINE_JUMP_LOOP(dense_jumps, 2, JUMP_DENSE)
DEFINE_JUMP_LOOP(dense_jumps, 4, JUMP_DENSE)
DEFINE_JUMP_LOOP(dense_jumps, 8, JUMP_DENSE)
DEFINE_JUMP_LOOP(dense_jumps, 16, JUMP_DENSE)
DEFINE_JUMP_LOOP(sparse_jumps, 1, JUMP_SPARSE)
DEFINE_JUMP_LOOP(sparse_jumps, 2, JUMP_SPARSE)
DEFINE_JUMP_LOOP(sparse_jumps, 4, JUMP_SPARSE)
DEFINE_JUMP_LOOP(sparse_jumps, 8, JUMP_SPARSE)
DEFINE_JUMP_LOOP(sparse_jumps, 16, JUMP_SPARSE)

// What the compiler does with a plain C++ loop (typically: vectorize and
// unroll), for reference.
__attribute__((noinline)) uint64_t sum_compiler(const uint64_t *p, size_t n) {
  uint64_t s = 0;
  for (size_t i = 0; i < n; i++) {
    s += p[i];
  }
  return s;
}

// ---------------------------------------------------------------------------
// Harness
// ---------------------------------------------------------------------------
static counters::bench_parameter params() {
  counters::bench_parameter p;
  p.min_repeat = 10;
  p.min_time_ns = 300'000'000;
  // std::steady_clock only has microsecond resolution on macOS: make each
  // timed block long enough that this does not matter.
  p.min_time_per_inner_ns = 200'000;
  return p;
}

static void print_header() {
  printf("%-28s %10s", "", "iterations");
  if (counters::has_performance_counters()) {
    printf(" %8s %10s %10s %10s %10s %8s", "GHz", "cycles/it", "instr/it",
           "branch/it", "branch/cyc", "IPC");
  } else {
    printf(" %10s", "ns/it");
  }
  printf("\n");
}

// `iterations` is the number of times the loop body ran per call, i.e. the
// number of taken branches per call for the sum loops. For the jump loops,
// we pass the number of taken branches per call instead, so that "cycles/it"
// is always cycles per taken branch.
static void report(const std::string &name, size_t iterations,
                   counters::event_aggregate agg) {
  printf("%-28s %10zu", name.c_str(), iterations);
  if (counters::has_performance_counters()) {
    double c = agg.fastest_cycles();
    printf(" %8.2f %10.3f %10.3f %10.3f %10.3f %8.2f",
           agg.cycles() / double(agg.elapsed_ns()), c / double(iterations),
           agg.fastest_instructions() / double(iterations),
           agg.fastest_branches() / double(iterations),
           agg.fastest_branches() / c, agg.fastest_instructions() / c);
  } else {
    printf(" %10.4f", agg.fastest_elapsed_ns() / double(iterations));
  }
  printf("\n");
}

int main(int argc, char **argv) {
  // 2048 x 8 bytes = 16 kB: fits in every L1 data cache.
  size_t n = 2048;
  if (argc > 1) {
    n = std::strtoull(argv[1], nullptr, 10);
    if (n == 0 || n % 16 != 0) {
      fprintf(stderr, "n must be a positive multiple of 16\n");
      return EXIT_FAILURE;
    }
  }
  // 64-byte aligned so that the compiler's vector loads never straddle a
  // cache line.
  uint64_t *data = static_cast<uint64_t *>(std::aligned_alloc(64, n * 8));
  uint64_t x = 0x9E3779B97F4A7C15ull;
  uint64_t expected = 0;
  for (size_t i = 0; i < n; i++) {
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    data[i] = x;
    expected += x;
  }
  const uint64_t *p = data;
  volatile uint64_t sink = 0;

  auto check = [&](const char *name, uint64_t got) {
    if (got != expected) {
      fprintf(stderr, "%s: wrong sum\n", name);
      std::exit(EXIT_FAILURE);
    }
  };
  check("sum_asm_1", sum_asm_1(p, n));
  check("sum_asm_2", sum_asm_2(p, n));
  check("sum_asm_4", sum_asm_4(p, n));
  check("sum_asm_8", sum_asm_8(p, n));
  check("sum_asm_16", sum_asm_16(p, n));
  check("sum_compiler", sum_compiler(p, n));

  printf("summing %zu 64-bit integers (%zu bytes)\n", n, n * sizeof(uint64_t));
  if (!counters::has_performance_counters()) {
    printf("(no hardware counters: run as root to get cycles)\n");
  }
  printf("\n");
  print_header();
  report("sum, no unrolling", n / 1,
         counters::bench([&] { sink = sum_asm_1(p, n); }, params()));
  report("sum, unrolled 2x", n / 2,
         counters::bench([&] { sink = sum_asm_2(p, n); }, params()));
  report("sum, unrolled 4x", n / 4,
         counters::bench([&] { sink = sum_asm_4(p, n); }, params()));
  report("sum, unrolled 8x", n / 8,
         counters::bench([&] { sink = sum_asm_8(p, n); }, params()));
  report("sum, unrolled 16x", n / 16,
         counters::bench([&] { sink = sum_asm_16(p, n); }, params()));
  report("sum, compiler (-O3)", n,
         counters::bench([&] { sink = sum_compiler(p, n); }, params()));
  printf("\n");
  printf("(the sum rows count one iteration per loop trip, except the "
         "compiler row which counts one per element)\n\n");

  print_header();
  report("empty loop", n,
         counters::bench([&] { empty_loop(n); }, params()));
  report("loop + 1 dense jump", n * 2,
         counters::bench([&] { dense_jumps_1(n); }, params()));
  report("loop + 2 dense jumps", n * 3,
         counters::bench([&] { dense_jumps_2(n); }, params()));
  report("loop + 4 dense jumps", n * 5,
         counters::bench([&] { dense_jumps_4(n); }, params()));
  report("loop + 8 dense jumps", n * 9,
         counters::bench([&] { dense_jumps_8(n); }, params()));
  report("loop + 16 dense jumps", n * 17,
         counters::bench([&] { dense_jumps_16(n); }, params()));
  report("loop + 1 sparse jump", n * 2,
         counters::bench([&] { sparse_jumps_1(n); }, params()));
  report("loop + 2 sparse jumps", n * 3,
         counters::bench([&] { sparse_jumps_2(n); }, params()));
  report("loop + 4 sparse jumps", n * 5,
         counters::bench([&] { sparse_jumps_4(n); }, params()));
  report("loop + 8 sparse jumps", n * 9,
         counters::bench([&] { sparse_jumps_8(n); }, params()));
  report("loop + 16 sparse jumps", n * 17,
         counters::bench([&] { sparse_jumps_16(n); }, params()));
  printf("\n(for these rows, one iteration = one taken branch)\n");
  std::free(data);
  return EXIT_SUCCESS;
}
