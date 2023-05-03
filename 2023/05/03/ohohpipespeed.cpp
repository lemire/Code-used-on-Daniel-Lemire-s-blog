#include "line_iterator.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <memory>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

int main() {
  std::ios_base::sync_with_stdio(false);
  constexpr size_t cache_length = 32768;
  std::unique_ptr<char[]> cachebuffer(new char[cache_length]{});
  uint64_t before = nano();

  size_t lines = 0;
  size_t sum_of_lines = 0;
  if (!isatty(fileno(stdin))) {
       std::string line;
       while (std::getline(std::cin, line)) {
        sum_of_lines += line.size() + 1;
        lines++;
       }
  }

  uint64_t after = nano();
  double giga = sum_of_lines / 1000000000.;
  std::cout << "read " << sum_of_lines << " bytes in " << (after - before)
            << " ns using " << lines << " lines" << std::endl;
  double seconds = (after - before) / 1000000000.;
  double speed = giga / seconds;
  std::cout << speed << " GB/s" << std::endl;
  return EXIT_SUCCESS;
}
