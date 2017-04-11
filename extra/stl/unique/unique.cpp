//$ g++ -O3 -o unique unique.cpp  -march=native

#include <unordered_set>
#include <algorithm>
#include <x86intrin.h>
#include <stddef.h>
#include <stdlib.h>
#include "benchmark.h"


static size_t array_unique(uint32_t *out, size_t len) {
    std::sort(out, out + len);
    return std::unique(out, out + len) - out;
}

static size_t hash_unique(uint32_t *out, size_t len) {
    std::unordered_set<uint32_t> s;
    for(uint32_t * i = out ; i != out + len; i++) s.insert(*i);
    uint32_t *outp = out;
    for(auto j = s.begin() ; j != s.end() ; ++j) {*outp = *j; outp++;}
    return outp - out;
}


void init(uint32_t * data, size_t N) {
  for(int i = 0; i < N; i++) data[i] = rand() % N;
}

int main() {
  const int N = 1024 * 1024 * 10;
  uint32_t * data = (uint32_t *)malloc(N * sizeof(uint32_t));
  const int repeat = 5;
  const bool verbose = true;
  BEST_TIME_NOCHECK(array_unique(data,N), init(data,N), repeat, N, verbose);
  BEST_TIME_NOCHECK(hash_unique(data,N), init(data,N), repeat, N, verbose);

  free(data);
}
