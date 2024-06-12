#include <iostream>

// https://rockset.com
// credit: Nathan Bronson, essentially branchless (https://godbolt.org/z/bG5cTnz3x) 
// malloc_usable_size
// folly: https://github.com/search?q=repo%3Afacebook%2Ffolly%20goodMallocSize&type=code
size_t fastGoodMallocSize(size_t minSize) {
  // This computes the smallest x where x >= minSize and x is 0, 8, 16, 32,
  // 48, or x >= 64 and x's binary representation starts with 100, 101, 110,
  // or 111.  This matches the default jemalloc allocation sizes for
  // minSize <= 0x7000000000000000 (almost 2^63).
  auto tailMask = (~size_t(0) >> 4) >> (std::countl_zero(minSize) - 1);
  tailMask |= (minSize & 0x30) != 0 ? 0xf : 0x7;
  return ((minSize - 1) | tailMask) + 1;
}

int main() {
  size_t cost = 0;
  size_t current_alloc = 0;
  size_t N = 10000000000;
  std::cout << std::hex;
  for (size_t i = 1; i < N; i++) {
    size_t this_alloc = fastGoodMallocSize(i);
    if (this_alloc > current_alloc) {
      std::cout << i << "  " << fastGoodMallocSize(i) << std::endl;
      current_alloc = this_alloc;
      cost += this_alloc;
      std::cout << double(cost) / double(i) << std::endl;
    }
  }
  std::cout << N << " " << cost << std::endl;
}
