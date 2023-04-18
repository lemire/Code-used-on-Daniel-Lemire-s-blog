#include <concepts>
#include <cstdint>
#include <cstddef>
#include <vector>

template<typename T>
concept is_iterable = requires(T v) { 
    { v.has_next() } -> std::convertible_to<bool>; 
    { v.next() } -> std::same_as<uint32_t>; 
    { v.reset() };
};


template <is_iterable T>
constexpr size_t count(T& t) {
    t.reset();
    size_t count = 0;
    while(t.has_next()) {
        t.next();
        count++;
    }
    return count;
}
template <is_iterable T>
constexpr size_t count(T&& t) {
    return count(t);
}

struct iterable_array {
    constexpr iterable_array(size_t s) : array(s) {}
    std::vector<uint32_t> array{};
    size_t index = 0;
    constexpr void reset() { index = 0; }
    constexpr bool has_next() { return index < array.size(); }
    constexpr uint32_t next() { index++; return array[index - 1]; }
};

size_t f(iterable_array & a) {
    return count(a);
}

consteval size_t f2() {
    return count(iterable_array(10));
}

size_t t() {
    return f2();
}

int main() {
  t();
  return 0;
}