#include <stdio.h>
#include <x86intrin.h>

__m512i vectorFibo(__m512i a, __m512i b, size_t number) {
  __m512i t;
  while(number -- != 0) {
    t = a;
    a = b;
    b = _mm512_add_epi32(b,t);
  }
  return b;
}


int main() {
  __m512i a =_mm512_set1_epi32(0);
  __m512i b =_mm512_set1_epi32(1);
  __m512i c = vectorFibo(a,b,1000000);
  int returnval =  _mm256_extract_epi32( _mm512_extracti64x4_epi64(c,1),7);
  printf("%d \n", returnval);
  return returnval;
}
