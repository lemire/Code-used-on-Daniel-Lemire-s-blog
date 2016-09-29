#include <stdint.h>

static void fibo(int * x, int * y) {
  int c = * y;
  *y = *x + *y;
  *x = c;
}

uint64_t howmany() {
  int x = 1;
  int y = 1;
  uint64_t counter = 0;
  while( y != 0) {
    int c = y;
    y = x + y;
    x = c;
    counter++;
  }
  return counter;
}


