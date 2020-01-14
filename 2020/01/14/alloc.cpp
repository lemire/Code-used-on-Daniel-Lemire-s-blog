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

__attribute__((noinline))void benchmemset(size_t s,  char * buf) {
  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  memset(buf,0,s);
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << "memset: " << (s / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
}

__attribute__((noinline))char * benchmemcpy(size_t s,  char * buf) {
  char * newbuf = new char[s]();

  std::chrono::time_point<std::chrono::steady_clock> start_clock =
      std::chrono::steady_clock::now();
  memcpy(newbuf, buf,s);
  std::chrono::time_point<std::chrono::steady_clock> end_clock =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end_clock - start_clock;
  std::cout << "memcpy: " << (s / (1024. * 1024 * 1024.)) / elapsed.count() << " GB/s"
            << std::endl;
  return newbuf;
}

int main() {
  for (size_t i = 268435456; i < 1073741824; i *= 2) {
    std::cout << "page count: " << i / 4096
              << ", volume:" << (i / (1024. * 1024 )) << " MB"
              << std::endl;

    for (int z = 0; z < 3; z++) {
      char *b = bench(i);
      benchmemset(i, b);
      char *b2 = benchmemcpy(i,b);
      delete[] b;
      delete[] b2;
    }
  }
}