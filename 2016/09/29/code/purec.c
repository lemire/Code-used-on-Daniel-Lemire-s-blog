#include <stdint.h>
#include <stdio.h>

void fibo(int * x, int * y) {
  int c = * y;
  *y = *x + *y;
  *x = c;
}

uint64_t howmany() {
  int x = 1;
  int y = 1;
  uint64_t counter = 0;
  while( y != 0) {
    fibo(&x, &y);
    counter++;
  }
  return counter;
}

int main() {
  printf("%u \n", (unsigned int) howmany());
}
