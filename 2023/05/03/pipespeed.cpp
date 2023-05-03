#include "line_iterator.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <cstring>
uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

int main() {
  std::ios_base::sync_with_stdio(false);
  constexpr size_t cache_length = 32768;
  std::unique_ptr<char[]> cachebuffer(new char[cache_length]);
  uint64_t before = nano();
  size_t howmany = 0;
  size_t offset = 0;
  size_t blocks = 0;
  size_t sum_of_lines = 0;
  size_t lines = 0;

  while (std::cin) {
    std::cin.read(cachebuffer.get() + offset, cache_length - offset);
    size_t tr = std::cin.gcount();
    howmany += tr;
    blocks++;
    size_t capacity = tr + offset;
    line_iterator li(cachebuffer.get(), capacity);
    if (li.find_another_complete_line()) {
      std::string_view line = li.grab_line();
      sum_of_lines += line.size() + 1;
      lines++;
    }
    while (li.find_another_complete_line()) {
      std::string_view line = li.grab_line();
      sum_of_lines += line.size() + 1;
      lines++;
    }
    if ((offset = li.tail()) > 0) {
      memmove(cachebuffer.get(), cachebuffer.get() + capacity - offset, offset);
    }
  }
  if (offset > 0) {
    // have a line of length offset at cachebuffer.get()
    lines++;
    sum_of_lines += offset;
  }
  uint64_t after = nano();
  double giga = howmany / 1000000000.;
  std::cout << "read " << howmany << " bytes in " << (after - before)
            << " ns using " << lines << " lines, sum_of_lines is "
            << sum_of_lines << " used " << blocks << " loads" << std::endl;
  double seconds = (after - before) / 1000000000.;
  double speed = giga / seconds;
  std::cout << speed << " GB/s" << std::endl;
  return EXIT_SUCCESS;
}
