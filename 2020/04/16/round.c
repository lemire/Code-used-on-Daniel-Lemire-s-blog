unsigned int roundtoeven(unsigned int n, unsigned int d) {
  unsigned int off = (n + d / 2); // assume no overflow
  unsigned int roundup = off / d;
  int ismultiple = ((off % d) == 0);
  int iseven = (d & 1) == 0;
  return (ismultiple && iseven) ? roundup - (roundup & 1) : roundup;
}

unsigned int roundtoeven2(unsigned int numerator, unsigned int divisor) {
  unsigned int offsetnumerator = (numerator + divisor / 2); // assume no overflow
  unsigned int roundup = offsetnumerator / divisor;
  unsigned int down = roundup - (roundup & 1);
  int ismultiple = ((offsetnumerator % divisor) == 0);
  return (divisor | (roundup ^ ismultiple)) & roundup;
}
#include <stdio.h>

int main() {
  for(unsigned int n = 0; n < 30; n ++) {
    printf("%u %u %u \n", n, roundtoeven(n,5), roundtoeven2(n,5));
  }
}
