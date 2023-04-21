#include <concepts>
#include <cstddef>
#include <cstdint>
#include <vector>

template <typename T>
concept is_iterable = requires(T v) {
                        { v.has_next() } -> std::convertible_to<bool>;
                        { v.next() } -> std::same_as<uint32_t>;
                        { v.reset() };
                      };

template <is_iterable T> size_t count(T &t) {
  t.reset();
  size_t count = 0;
  while (t.has_next()) {
    t.next();
    count++;
  }
  return count;
}

struct iterable_array {
  std::vector<uint32_t> array{};
  size_t index = 0;
  // void reset() { index = 0; }
  bool has_next() { return index < array.size(); }
  uint32_t next() {
    index++;
    return array[index - 1];
  }
};

size_t f(iterable_array &a) { return count(a); }

size_t t() {
  iterable_array a;
  a.array.resize(10);
  return f(a);
}

int main() {
  t();
  return 0;
}