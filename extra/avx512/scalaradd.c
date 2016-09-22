#include <x86intrin.h>

int Fibo(int a, int b, size_t number) {
  int t;
  while(number -- != 0) {
    t = a;
    a = b;
    b = b + t;
  }
  return b;
}


int main() {
  int a = 0;
  int b = 1;
  return Fibo(a,b,1000000);
}
