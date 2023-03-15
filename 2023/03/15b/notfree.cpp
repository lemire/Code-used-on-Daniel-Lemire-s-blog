#include <iostream>
#include <random>
#include <vector>
#include <cassert>
#include <chrono>
uint64_t nano() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
__attribute__((noinline)) void copy(uint64_t *x1, uint64_t *x2, size_t N) {
  for (size_t i = 0; i < N; i++) {
    x1[i] = x2[i];
  }
}
__attribute__((noinline)) void copy2(uint64_t *x1, uint64_t *x2, size_t N) {
  for (size_t i = 0; i < N; i++) {
    assert(x2[i] <= RAND_MAX);
    x1[i] = x2[i];
  }
}

void demo() {
  size_t N = 1000;
  uint64_t *input = new uint64_t[N];
  uint64_t *output = new uint64_t[N];
  for (size_t i = 0; i < N; i++) {
    input[i] = rand();
  }
  // cache warmup
  copy(input, output, N);
  copy2(input, output, N);

  uint64_t start, finish;
  std::cout << "no assert" << std::endl;
  for (size_t i = 0; i < 5; i++) {
    start = nano();
    copy(input, output, N);
    finish = nano();
    double t1 = double(finish - start) / N;
    std::cout << t1 << std::endl;
  }
  std::cout << "assert" << std::endl;
  for (size_t i = 0; i < 5; i++) {
    start = nano();
    copy2(input, output, N);
    finish = nano();
    double t2 = double(finish - start) / N;
    std::cout << t2 << std::endl;
  }
}

int main() {
  printf("===\n");
  for (size_t i = 0; i < 5; i++) {
    demo();
  }
  return EXIT_SUCCESS;
}
