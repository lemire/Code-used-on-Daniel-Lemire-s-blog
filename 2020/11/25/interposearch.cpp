#include <iostream>
#include <random>
#include <vector>

#include <algorithm>
#include <cmath>

std::vector<uint32_t> generate(size_t N) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
  std::vector<uint32_t> answer;
  // This is not ideal C++,  but it will do:
  for (size_t i = 0; i < N; i++) {
    answer.push_back(dis(gen));
  }
  sort(answer.begin(), answer.end());
  return answer;
}

// Do not rely on this function for production code, this is a prototype that
// is almost surely buggy in corner cases.
int64_t search(const std::vector<uint32_t> &array, uint32_t key, size_t &hits) {

  if (array.size() == 0) {
    return -1;
  }
  // We assume that we have a random/uniform distribution.
  // Next line could be optimized highly.
  double max_value = 4294967296.;
  double min_value = 0;
  int64_t lower = 0;
  int64_t upper = int64_t(array.size() - 1);
  int64_t index =
      int64_t(floor(double(key - min_value) / double(max_value - min_value) *
                    (double(upper) - double(lower) + 1))) +
      lower;

  while (true) {
    hits++;
    uint32_t value = array[index];
    if (value > key) {
      upper = index - 1;
      max_value = value;
    } else if (value < key) {
      lower = index + 1;
      min_value = value;
    } else if (value == key) {
      return index;
    }
    if (upper < lower) {
      return -lower - 1;
    }

    index =
        int64_t(floor(double(key - min_value) / double(max_value - min_value) *
                      (double(upper) - double(lower) + 1))) +
        lower;
  }
}

double expected_hits(size_t N) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, N - 1);
  std::vector<uint32_t> t = generate(N);

  size_t expected = 0;
  size_t tests = 1000;
  for (size_t i = 0; i < tests; i++) {
    size_t index = dis(gen);
    uint32_t key = t[index];
    size_t hits = 0;
    int64_t found_index = search(t, key, hits);
    assert(t[found_index] == key);
    expected += hits;
  }
  return double(expected) / tests;
}
int main() {
  for (size_t N = 100; N <= 1000000000; N *= 10) {
    std::cout << N << " " << expected_hits(N) << std::endl;
  }
}
