#include <x86intrin.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  _mm256_zeroupper();
  float a = 3;
  float b = atoi(argv[1]);
  if(argc>2) _mm256_zeroupper();
  for(int k = 0; k < 5000000; k++) {
    b = a + b * b;
    a = b - a * a;
  }
  return (b == 0)? 1 : 0;
}
