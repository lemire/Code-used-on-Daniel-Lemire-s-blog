#include <chrono>
#include <iostream>

__attribute__((noinline)) char *bench(size_t s) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  char *buf1 = new char[s]();
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << (s / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
  return buf1;
}

int main() {
  for (size_t i = 268435456; i < 1073741824; i *= 2) {
    std::cout << "page count: " << i / 4096
              << ", volume:" << (i / (1024. * 1024 )) << " MB"
              << std::endl;

    for (int z = 0; z < 3; z++) {
      char *b = bench(i);
      delete[] b;
    }
  }
}