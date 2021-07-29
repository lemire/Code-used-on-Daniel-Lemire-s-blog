
extern "C" {
#include "nadeau.h"
}
#include <fstream>
#include <iostream>
#include <unistd.h>

int main() {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  constexpr size_t N = 100000000;
  char *buffer = new char[N]; // allocate 100MB
  for (size_t i = 0; i < N; i++) {
    buffer[i] = 1;
    if ((i % (N / 100) == 0) || (i == N - 1)) {
      int64_t sf = getCurrentRSS();
      size_t pages = (i + pagesize) / pagesize;
      std::cout << sf << " " << pages * pagesize << std::endl;
    }
  }
  return EXIT_SUCCESS;
}