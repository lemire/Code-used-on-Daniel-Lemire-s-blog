#include <chrono>
#include <cstring>
#include <iostream>

__attribute__((noinline)) char* bench(size_t n) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  char * p = new char[n];
  std::fill(p, p + n, 0);
  asm volatile ("" : : : "memory");
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << "std::fill(p, p + n, 0)      : "
            << (n / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
  return p;
}

__attribute__((noinline)) char* bench1(size_t n) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  char * p = new char[n];
  std::fill(p, p + n, '\0');
  asm volatile ("" : : : "memory");
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << "std::fill(p, p + n, '\\0')   : "
            << (n / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
  return p;
}

int main() {
  for (size_t i = 268435456; i < 1073741824; i *= 2) {
    std::cout << "page count: " << i / 4096
              << ", volume:" << (i / (1024. * 1024)) << " MB" << std::endl;
    for (int z = 0; z < 3; z++) {
      delete[] bench(i);
      delete[] bench1(i);
      std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
  }
}
