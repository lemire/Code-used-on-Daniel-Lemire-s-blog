#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
int main(int argc, char **argv) {
  std::ios_base::sync_with_stdio(false);
  size_t repeat = 1000;
  if (argc > 1) {
    std::from_chars(argv[1], argv[1] + strlen(argv[1]), repeat);
  }
  constexpr size_t buflength = 36;
  std::vector<char> buffer(buflength, '-');
  for (size_t i = 0; i < repeat; i++) {
    std::cout.write(buffer.data(), buflength - (i % 16));
    std::cout.write("\n", 1);
  }
  std::cout.write(buffer.data(), buflength);
  return EXIT_SUCCESS;
}
