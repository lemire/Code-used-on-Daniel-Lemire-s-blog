/**
 g++ -O3 -o setunionintersection setunionintersection.cpp -std=c++11
*/

#include "benchmark.h"
#include <algorithm>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <vector>
#include <set>


typedef std::unordered_set<uint32_t> hashset;
typedef std::set<uint32_t> treeset;
typedef std::vector<uint32_t> vector;

static void setintersection(hashset &h1, hashset &h2, hashset &answer) {
  if (h1.size() > h2.size()) {
    setintersection(h2, h1, answer);
    return;
  }
  answer.clear();
  for (hashset::iterator i = h1.begin(); i != h1.end(); i++) {
    if (h2.find(*i) != h2.end())
      answer.insert(*i);
  }
}

static void setunion_alt(hashset &h1, hashset &h2, hashset &out) {
  out = h1;
  out.insert(h2.begin(), h2.end());
}

static void setunion(hashset &h1, hashset &h2, hashset &out) {
  out.clear();
  out.insert(h1.begin(), h1.end());
  out.insert(h2.begin(), h2.end());
}

static void arrayunion(vector &v1, vector &v2, vector &v) {
  v.clear();
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 std::back_inserter(v));
}

static void arrayintersection(vector &v1, vector &v2, vector &v) {
  v.clear();
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(v));
}

static void treesetunion(treeset &v1, treeset &v2, treeset &v) {
  v.clear();
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::inserter(v,v.begin()));
}

static void treesetintersection(treeset &v1, treeset &v2, treeset &v) {
  v.clear();
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::inserter(v,v.begin()));
}

int demo(const int N) {
  printf("%d\n", N);
  hashset s1;
  hashset s2;
  vector v1;
  vector v2;
  treeset t1;
  treeset t2;
  for (int z = 0; z < N; ++z) {
    s1.insert(3 * z + 1);
    v1.push_back(3 * z + 1);
    t1.insert(3 * z + 1);
    s2.insert(N - z);
    v2.push_back(N - z);
    t2.insert(N - z);
  }
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  const int repeat = 5;
  hashset out;
  vector v;
  treeset t;
  BEST_TIME_NOCHECK(setintersection(s1, s2, out), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(setunion(s1, s2, out), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(setunion_alt(s1, s2, out), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(arrayintersection(v1, v2, v), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(arrayunion(v1, v2, v), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(treesetintersection(t1, t2, t), , repeat, 2 * N, true);
  BEST_TIME_NOCHECK(treesetunion(t1, t2, t), , repeat, 2 * N, true);

  return out.size() + v.size() + t.size();
}

int main() {
  const int N = 1000000;
  demo(1000);
  demo(1000000);
  demo(10 * 1000000);
}
