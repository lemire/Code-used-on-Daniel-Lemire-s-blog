#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

int main() {
  std::ios_base::sync_with_stdio(false);
  constexpr size_t cache_length = 16384;
  char cachebuffer[cache_length];
  std::vector<char> buffer;
  buffer.reserve(cache_length);
  uint64_t before = nano();
  while(std::cin) {
    std::cin.read(cachebuffer, cache_length);
    buffer.insert(buffer.end(), cachebuffer, cachebuffer + std::cin.gcount());
  }
  uint64_t after = nano();
  double giga = buffer.size() / 1000000000.;
  std::cout << "read " << buffer.size() << " bytes in " << (after - before) << " ns"<< std::endl;
  double seconds = (after - before) / 1000000000.;
  double speed = giga / seconds;
  std::cout <<  speed << " GB/s" << std::endl;
  return EXIT_SUCCESS;
}
