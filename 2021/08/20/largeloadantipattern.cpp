#include <unistd.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <limits>
#include <cassert>
#include <sstream>


uint64_t nano() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

__attribute__((noinline))
void init(int * content, size_t volume) {
  for(size_t i = 0; i < volume/sizeof(int); i++) {
    content[i] = i;
  }
}
__attribute__((noinline))
uint64_t load_read_and_process(size_t volume) {
  int * content = new int[volume/sizeof(int)];
  init(content, volume);
  uint64_t sum = 0;
  for(size_t i = 0; i < volume/sizeof(int); i++) {
    sum += content[i];
  }
  delete[] content;
  return sum;
}

__attribute__((noinline))
void load_read(size_t volume) {
  int * content = new int[volume/sizeof(int)];
  init(content, volume);
  delete[] content;
}


__attribute__((noinline))
uint64_t read_and_process(int * content, size_t volume) {
  uint64_t sum = 0;
  for(size_t i = 0; i < volume/sizeof(int); i++) {
    sum += content[i];
  }
  return sum;
}

void bench(const size_t size_in_bytes) {
  uint64_t before, after;
  double s0, s1, s2;
  uint64_t ref_count = load_read_and_process(size_in_bytes);

  for(size_t i = 0; i < 3; i++) {
    before = nano();
    load_read(size_in_bytes);
    after = nano();
    s0 = size_in_bytes / double(after - before);
    std::cout << s0 << " GB/s ";
  }
  std::cout << " | ";

  for(size_t i = 0; i < 3; i++) {
    before = nano();
    uint64_t count = load_read_and_process(size_in_bytes);
    after = nano();
    s1 = size_in_bytes / double(after - before);
    std::cout << s1 << " GB/s ";
    if(count != ref_count) { throw std::runtime_error("bug"); }
  }
  std::cout << " | ";

  int * content = new int[size_in_bytes/sizeof(int)];
  init(content, size_in_bytes);
  ref_count = read_and_process(content, size_in_bytes);
  for(size_t i = 0; i < 3; i++) {
    before = nano();
    uint64_t count = read_and_process(content, size_in_bytes);
    after = nano();
    s2 = size_in_bytes / double(after - before);
    std::cout << s2 << " GB/s ";
    if(count != ref_count) { throw std::runtime_error("bug"); }
  }
  delete[] content;
  std::cout << std::endl;

}

int main() {
  std::cout << "Small:" << std::endl;
  for(size_t length = 1000000; length <= 4000000; length *= 2) {
    std::cout << length / 1000000.0 << " MB : ";
    bench(length);
  }
  std::cout << "Large:" << std::endl;
  for(size_t length = 250000000; length <= 4000000000; length *= 2) {
    std::cout << length / 1000000000.0 << " GB : ";
    bench(length);
  }

  return EXIT_SUCCESS;
}
