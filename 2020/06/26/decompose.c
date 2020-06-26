#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char ** val) {
  double x = atof(val[1]);
  if(x < 0) x = -x;
  printf("%18.18f\n",x);
  uint64_t number;
  memcpy(&number, &x,8);
  int exponent = (number >> 52) - 1023-52;
  uint64_t mantissa = (number & 0x0fffffffffffff) + 0x10000000000000;
  printf("%llu  * 2 ** %d\n", mantissa, exponent);
}
