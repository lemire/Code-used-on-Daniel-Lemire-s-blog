// gcc -O3 -o binarygcd binarygcd.c  -march=native
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "benchmark.h"
#include <x86intrin.h>

static inline uint64_t gcdwikipedia2fast(uint64_t u, uint64_t v) {
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
    shift = __builtin_ctzll(u | v);
    u >>= __builtin_ctzll(u);
    do {
        v >>= __builtin_ctzll(v);
        if (u > v) {
            uint64_t t = v;
            v = u;
            u = t;
        }
        v = v - u;
    } while (v != 0);
    return u << shift;
}

uint64_t allgcds(size_t N) {
    uint64_t bogus = 0;
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcdwikipedia2fast(x,y);
    return bogus;
}


int main() {
  const size_t N = 2000;
  const int repeat = 5;
  uint64_t expected = allgcds(N);
  BEST_TIME(allgcds(N),expected,, repeat, N * N, true);
}
