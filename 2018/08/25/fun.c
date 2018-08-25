/**
$ gcc -O2 -o fun fun.c -march=native -mno-avx512f && perf stat ./fun a b c 2>&1 |grep GHz && perf stat ./fun a b 2>&1 |grep GHz  && perf stat ./fun a 2>&1 |grep GHz &&  perf stat ./fun  2>&1 |grep GHz
        40,718,941      cycles                    #    3.167 GHz
        40,719,004      cycles                    #    3.163 GHz
        40,735,489      cycles                    #    2.796 GHz
        40,707,631      cycles                    #    2.796 GHz

**/
#include <x86intrin.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  if(argc>1) _mm256_zeroupper();
  float a = 3;
  float b = rand();
  if(argc>2) _mm256_zeroupper();
  for(int k = 0; k < 5000000; k++) {
    b = a + b * b;
    a = b - a * a;
  }
  return (b == 0)? 1 : 0;
}
