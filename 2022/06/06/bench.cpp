#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

struct padded_struct {
  uint64_t key;
  uint64_t value;
  bool is_valid;
};

struct packed_struct {
  uint64_t key;
  uint64_t value;
  uint8_t is_valid;
} __attribute__((__packed__));

static_assert(sizeof(padded_struct) == 24,
              "padded_struct should use 24 bytes.");
static_assert(sizeof(packed_struct) == 17,
              "padded_struct should use 17 bytes.");

template <class T> void init(T *s, size_t size) {
  // Initialize the structs with random values + prefault
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<uint64_t> dist{0, 0xDEADBEEF};
  for (uint64_t i = 0; i < size; ++i) {
    s[i].key = dist(gen);
    s[i].value = dist(gen);
    s[i].is_valid = 1;
  }
}

template <class T>
void init_queries(T *s, size_t size, T **z, size_t queries_size) {
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<size_t> dist{0, size - 1};
  for (size_t i = 0; i < queries_size; i++) {
    z[i] = &s[dist(gen)];
  }
}

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

template <class type> double benchmark(size_t size, size_t query_size) {
  std::unique_ptr<type[]> data(new type[size]);
  init(data.get(), size);
  std::unique_ptr<type *[]> queries(new type *[query_size]);
  init_queries(data.get(), size, queries.get(), query_size);
  type **p = queries.get();
  auto benchfnc = [p, query_size]() -> uint64_t {
    uint64_t sum{0};
    for (size_t i = 0; i < query_size; i++) {
      type *k = p[i];
      if (k->is_valid) {
        sum += k->value;
      }
    }
    return sum;
  };
  uint64_t start = nano();
  uint64_t finish = start;
  size_t count{0};
  uint64_t val{};
  uint64_t threshold = 500'000'000;
  for (; finish - start < threshold;) {
    count++;
    val += benchfnc();
    finish = nano();
  }
  if (val == 0) {
    std::cout << "bogus" << std::endl;
  }
  return double(finish - start) / count;
}

int main() {
  size_t padded_size{1024 * 1024 * 4};
  size_t volume = padded_size * sizeof(padded_struct);
  size_t packed_size = volume / sizeof(packed_struct);
  size_t query_size = 1024 * 1024;
  for (size_t i = 0; i < 3; i++) {
    benchmark<padded_struct>(padded_size, query_size);
    std::cout << ".";
    std::cout.flush();
    benchmark<packed_struct>(packed_size, query_size);
    std::cout << ".";
    std::cout.flush();
  }
  std::cout << std::endl;
  std::vector<double> padded_timings;
  std::vector<double> packed_timings;
  std::cout << "[" << std::endl;
  std::cout << "['padded', 'packed']," << std::endl;
  for (size_t i = 0; i < 30; i++) {
    std::cout << "[";
    double pdt = benchmark<padded_struct>(padded_size, query_size) / query_size;
    padded_timings.push_back(pdt);
    std::cout << pdt << ",\t";
    double pct = benchmark<packed_struct>(packed_size, query_size) / query_size;
    packed_timings.push_back(pct);
    std::cout << pct;
    std::cout << "]," << std::endl;
  }
  std::cout << "]" << std::endl;
}