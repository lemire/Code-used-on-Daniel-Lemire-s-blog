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

class iter_base {
public:
  virtual bool has_next() = 0;
  virtual uint32_t next() = 0;
  virtual void reset() = 0;
  virtual ~iter_base() = default;
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

size_t count_inheritance(iter_base &t) {
  t.reset();
  size_t count = 0;
  while (t.has_next()) {
    t.next();
    count++;
  }
  return count;
}

struct iterable_array final : iter_base  {
  std::vector<uint32_t> array{};
  size_t index = 0;
  void reset() { index = 0; }
  bool has_next() { return index < array.size(); }
  uint32_t next() {
    index++;
    return array[index - 1];
  }
};

size_t f1(iterable_array &a) { return count(a); }



size_t f2(iterable_array &a) { return count_inheritance(a); }



int main() {
  return 0;
}