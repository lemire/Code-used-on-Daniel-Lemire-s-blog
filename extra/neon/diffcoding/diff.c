#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <arm_neon.h>

uint32x4_t Delta(uint32x4_t curr, uint32x4_t prev) {
   return vsubq_u32(curr, vextq_u32 (prev,curr,3));
}

uint32x4_t PrefixSum(uint32x4_t curr, uint32x4_t prev) {
   uint32x4_t zero = {0, 0, 0, 0};
   uint32x4_t add = vextq_u32 (zero, curr, 3);
   uint8x16_t BroadcastLast = {12,13,14,15,12,13,14,15,12,13,14,15,12,13,14,15};
   prev = vreinterpretq_u32_u8(vqtbl1q_u8(vreinterpretq_u8_u32(prev),BroadcastLast));
   curr = vaddq_u32(curr,add); 
   add = vextq_u32 (zero, curr, 2);
   curr = vaddq_u32(curr,prev);
   curr = vaddq_u32(curr,add);
   return curr;
}

int main() {
   uint32x4_t curr = {1,10, 15,21};
   uint32x4_t prev = {0,0,0,1};
   uint32x4_t delta = Delta(curr,prev);
   printf("%u %u %u %u \n",delta[0],delta[1],delta[2],delta[3]);
   uint32x4_t back = PrefixSum(delta, prev);
   printf("%u %u %u %u \n",back[0],back[1],back[2],back[3]);
}
