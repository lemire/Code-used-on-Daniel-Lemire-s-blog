/**
 g++ -O3 -o stlreinsert stlreinsert.cpp -std=c++11
*/

#include "benchmark.h"
#include <algorithm>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>

typedef std::unordered_set<uint32_t> hashset;

int demo(const int N) {
  printf("%d\n", N);
  hashset s1;
  for (int z = 0; z < N; ++z) {
    s1.insert(z);
  }
  const int repeat = 1;
  hashset out;
  out.insert(1000);
  BEST_TIME_NOCHECK(out.insert(s1.begin(), s1.end()), , repeat, N, true);
  return out.size();
}

int main() {
  const int N = 1000000;
  demo(1000);
  demo(1000000);
  demo(10 * 1000000);
  demo(100 * 1000000);

}
