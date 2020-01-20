#include <chrono>
#include <cstring>
#include <iostream>

__attribute__((noinline)) char* bench(char *p, size_t n) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
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

__attribute__((noinline)) char* bench1(char *p, size_t n) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
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

__attribute__((noinline)) char* bench2(char *p, size_t n) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  memset(p,0,n);
  asm volatile ("" : : : "memory");
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << "memset(p,0,n)               : "
            << (n / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
  return p;
}
int main() {
  for (size_t i = 131072; i < 4294967296; i *= 2) {
    std::cout << "page count: " << i / 4096
              << ", volume:" << (i / (1024. * 1024)) << " MB" << std::endl;
    char * p = new char[i];
    memset(p, 1, i);
    for (int z = 0; z < 3; z++) {
      bench(p, i);
      bench1(p, i);
      bench2(p, i);
      std::cout << std::endl;
    }
    delete[] p;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
  }
}
