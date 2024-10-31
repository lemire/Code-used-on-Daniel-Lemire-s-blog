#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <iostream>
int int_log2(uint64_t x) { return 63 - __builtin_clzll(x | 1); }

int digit_count(uint64_t x) {
  static uint64_t table[] = {9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 9999999999, 99999999999, 999999999999, 9999999999999, 99999999999999, 999999999999999ULL, 9999999999999999ULL, 99999999999999999ULL, 999999999999999999ULL, 9999999999999999999ULL};
  int y = (19*int_log2(x)>>6);//(9 * int_log2(x)) >> 5;
  y += x > table[y];
  return y + 1;
}


int main() {
    size_t counter = 0;
    for(uint64_t i = 1; i <= 10000000000000000000ULL; i*=10) {
        ++counter;
        std::cout << i << " " << digit_count(i) << " == " << counter << std::endl;
        printf("%" PRIu64 " %d == %zu\n", i, digit_count(i), counter);

    }
    return 0;
}