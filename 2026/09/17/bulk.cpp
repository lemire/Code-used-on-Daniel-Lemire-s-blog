// Building a std::flat_map in bulk, and adding a batch to an existing one.
//
//   g++-15 -std=c++23 -O2 bulk.cpp -o bulk && ./bulk
#include <cstdint>
#include <cstdio>
#include <flat_map>
#include <utility>
#include <vector>

struct point {
  double x;
  double y;
};

int main() {
  // Build from a range of pairs, in any order: the constructor appends them
  // all, sorts once, and removes duplicate keys.
  std::vector<std::pair<uint64_t, point>> pairs = {
      {42, {1.0, 2.0}}, {7, {3.0, 4.0}}, {1000, {5.0, 6.0}}};
  std::flat_map<uint64_t, point> m(pairs.begin(), pairs.end());
  // keys = {7, 42, 1000}

  // Add a batch to an existing map: the batch is sorted and merged in one
  // pass, instead of shifting the arrays once per element.
  std::vector<std::pair<uint64_t, point>> more = {{500, {7.0, 8.0}},
                                                  {3, {9.0, 10.0}}};
  m.insert_range(more);
  // keys = {3, 7, 42, 500, 1000}

  const uint64_t expected[] = {3, 7, 42, 500, 1000};
  size_t i = 0;
  bool ok = m.size() == 5;
  for (const auto &[k, v] : m) {
    std::printf("%llu -> (%g, %g)\n", (unsigned long long)k, v.x, v.y);
    ok = ok && i < 5 && k == expected[i++];
  }
  std::printf("keys in order: %s\n", ok ? "true" : "false");
  return ok ? 0 : 1;
}
