#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




uint64_t matching_bytes_in_word(uint64_t x, uint64_t y) {
  uint64_t xor_xy = x ^ y;
  // credit: mula
  // 7th bit set if lower 7 bits are zero
  const uint64_t t0 = (~xor_xy & 0x7f7f7f7f7f7f7f7fllu) + 0x0101010101010101llu;
  // 7th bit set if 7th bit is zero
  const uint64_t t1 = (~xor_xy & 0x8080808080808080llu);
  uint64_t zeros = t0 & t1;
  return ((zeros >> 7) * 0x0101010101010101ULL) >> 56;
}

uint64_t matching_bytes(const char * c1, const char * c2, size_t n) {
    size_t count = 0;
    size_t i = 0;
    uint64_t x, y;
    for(; i + sizeof(uint64_t) <= n; i+= sizeof(uint64_t)) {
      memcpy(&x, c1 + i, sizeof(uint64_t) );
      memcpy(&y, c2 + i, sizeof(uint64_t) );
      count += matching_bytes_in_word(x,y);
    }
    for(; i < n; i++) {
        if(c1[i]  == c2[i]) { count++; }
    }
    return count;
}
uint64_t standard_matching_bytes(const char * c1, const char * c2, size_t n) {
    size_t count = 0;
    size_t i = 0;
    for(; i < n; i++) {
        if(c1[i]  == c2[i]) { count++; }
    }
    return count;
}

void test() {
    size_t N = 100;
    char c1[N];
    char c2[N];
    for(size_t i = 0; i < 100000; i++) {
        for(size_t j = 0; j < N; j++) {
            c1[j] = (3*j + 7*j*i + 2 * i) %3;
            c2[j] = (5*j + 2 * i) %4;
        }
        if(matching_bytes(c1, c2, N) != standard_matching_bytes(c1, c2, N)) { abort(); }
    }
}

int main() {
    test();
}
