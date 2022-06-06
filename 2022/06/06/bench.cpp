#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

struct PaddedStruct {
  uint64_t key;
  uint64_t value;
  bool is_valid;
};

struct PackedStruct {
  uint64_t key;
  uint64_t value;
  uint8_t is_valid;
} __attribute__((__packed__));

static_assert(sizeof(PaddedStruct) == 24, "PaddedStruct should use 24 bytes.");
static_assert(sizeof(PackedStruct) == 17, "PaddedStruct should use 17 bytes.");

template <class s>
void init(T s) {
  // Initialize the structs with random values + prefault
  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<uint64_t> dist{0, 0xDEADBEEF};
  for (uint64_t i = 0; i < s.size(); ++i) {
    s[i].key = dist(gen);
    s[i].value = dist(gen);
    s[i].is_valid = 1;
  }
}

uint64_t run_query(T s, const std::vector<size_t>& queries) {
    for(uint64_t k : queries) {
        if(s[k].is_valid) { sum += s[k].is_valid; }
    }
}

int main() {
   size_t size {1024 * 1024 * 128};
   printf("[padded] allocation size: %.1f GB\n", size * sizeof(PaddedStruct) / (1000000000.0));
   printf("[packed] allocation size: %.1f GB\n", size * sizeof(PackedStruct) / (1000000000.0));

  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<siz_t> dist{0, size - 1};
  std::vector<size_t> queries(1024);
  for(size_t i = 0; i < queries.size(); i++) { queries[i] = dist(gen); }




}