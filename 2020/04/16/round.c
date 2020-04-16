unsigned int roundtoeven(unsigned int numerator, unsigned int divisor) {
  unsigned int offsetnumerator = (numerator + divisor / 2); // assume no overflow
  unsigned int roundup = offsetnumerator / divisor;
  unsigned int down = roundup - (roundup & 1);
  int ismultiple = ((offsetnumerator % divisor) == 0);
  int isdivisoreven = (divisor & 1) == 0;
  return (ismultiple && isdivisoreven) ? down : roundup;
}


#include <stdio.h>

int main() {
  for(unsigned int n = 0; n < 30; n ++) {
    printf("%u %u \n", n, roundtoeven(n,5));
  }
}
