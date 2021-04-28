#include <cstdint>

// computes n % 274177
uint64_t div1(uint64_t n) {
    return n % 274177;
}

// computes n % 274177
uint64_t div2(uint64_t n) {
    return (uint64_t( n * 67280421310721 ) * __uint128_t(274177)) >> 64;
}

#include <iostream>
int main() {
  for(uint64_t n = 0; n < 100000; n++) {
    if(div1(n) != div2(n)) {
     std::cerr << "bug" << std::endl;
    }
  }
}
