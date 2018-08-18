// gcc -O3 -o cracktrunc cracktrunc.c 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t xorshift(uint64_t *m_seed) {
  uint64_t result = *m_seed * 0xd989bcacc137dcd5ull;
  *m_seed ^= *m_seed >> 11;
  *m_seed ^= *m_seed << 31;
  *m_seed ^= *m_seed >> 18;
  return (uint32_t)(result >> 32ull);
}

int main() {
  uint32_t output1 = 1;
  uint32_t output2 = 2;
  for (uint64_t low = 0; low <= 0xFFFFFFFF; low++) {
    uint64_t fullmult = low + ((uint64_t)output1 << 32);
    uint64_t reversedstate = 0x95c11c128eba7c7d * fullmult;
    uint64_t pristine = reversedstate;
    if (xorshift(&reversedstate) != output1)
      printf("bug");
    if (xorshift(&reversedstate) == output2) {
      printf("found it! Starting state: %llx \n", pristine); 
      printf("outputs:\n");
      for(int k = 0; k < 5; k++) {
           printf(" %u \n",xorshift(&pristine)); 
      }
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}
