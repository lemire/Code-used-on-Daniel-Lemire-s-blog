#include <time.h>
#include <x86intrin.h>
#include <stdio.h>
#include <cstdint>
#include <cstring>

// From "20141103 012910", we want to get
// 0x20141103012910
uint64_t extract_nibbles(const char* c) {
  uint64_t part1, part2;
  memcpy(&part1, c, sizeof(uint64_t));
  memcpy(&part2 , c + 7, sizeof(uint64_t));
  part1 = _bswap64(part1);
  part2 = _bswap64(part2);
  part1 = _pext_u64(part1, 0x0f0f0f0f0f0f0f0f);
  part2 = _pext_u64(part2, 0x0f000f0f0f0f0f0f);
  return (part1<<24) | (part2);
}

int main() {
  const char* c = "20141103 012910";
  printf("%zx\n", extract_nibbles(c));
}