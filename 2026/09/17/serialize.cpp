// A std::flat_map is two arrays: sorted keys and matching values. Serializing
// it means writing out those two arrays; loading it back means handing two
// arrays to the constructor.
//
//   g++-15 -std=c++23 -O2 serialize.cpp -o serialize && ./serialize
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <flat_map>
#include <vector>

// A fixed-size, trivially copyable value: it can be written to disk as is.
struct point {
  double x;
  double y;
  bool operator==(const point &) const = default;
};

int main() {
  std::flat_map<uint64_t, point> m;
  m[42] = {1.0, 2.0};
  m[7] = {3.0, 4.0};
  m[1000] = {5.0, 6.0};

  // Serialize: the keys and the values are two arrays, and the keys are
  // sorted. Copy them out.
  std::vector<uint64_t> keys(m.keys().begin(), m.keys().end());
  std::vector<point> values(m.values().begin(), m.values().end());
  // keys   = {7, 42, 1000}
  // values = {{3, 4}, {1, 2}, {5, 6}}

  // Both arrays are plain bytes: keys.size() * 8 bytes and
  // values.size() * 16 bytes. Write them to a file, send them over the
  // network... Here we just copy them through a byte buffer.
  std::vector<char> buffer(keys.size() * sizeof(uint64_t) +
                           values.size() * sizeof(point));
  std::memcpy(buffer.data(), keys.data(), keys.size() * sizeof(uint64_t));
  std::memcpy(buffer.data() + keys.size() * sizeof(uint64_t), values.data(),
              values.size() * sizeof(point));

  // Deserialize: read the two arrays back...
  size_t n = 3;
  std::vector<uint64_t> loaded_keys(n);
  std::vector<point> loaded_values(n);
  std::memcpy(loaded_keys.data(), buffer.data(), n * sizeof(uint64_t));
  std::memcpy(loaded_values.data(), buffer.data() + n * sizeof(uint64_t),
              n * sizeof(point));

  // ... and give them to the constructor. Because we promise, with
  // std::sorted_unique, that the keys are sorted and distinct, nothing is
  // sorted, checked or copied: the map adopts the two arrays.
  std::flat_map<uint64_t, point> back(std::sorted_unique, std::move(loaded_keys),
                                      std::move(loaded_values));

  for (const auto &[k, v] : back) {
    std::printf("%llu -> (%g, %g)\n", (unsigned long long)k, v.x, v.y);
  }
  std::printf("back == m: %s\n", back == m ? "true" : "false");
  return back == m ? 0 : 1;
}
