#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <arm_neon.h>

uint32x4_t Delta(uint32x4_t curr, uint32x4_t prev) {
   return vsubq_u32(curr, vextq_u32 (prev,curr,3));
}

int main() {
   uint32x4_t curr = {1,10, 15,21};
   uint32x4_t prev = {0,0,0,0};
   uint32x4_t delta = Delta(curr,prev);
   printf("%u %u %u %u \n",delta[0],delta[1],delta[2],delta[3]);
}
