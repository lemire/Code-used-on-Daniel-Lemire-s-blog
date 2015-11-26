#include <stdio.h>
#include <stdint.h>



/* 
* given an odd number a, this returns
* the 32-bit inverse b such that a*b =1

 It will essentially compute
 
 (2- a) * (1 + (a-1)) * (1 + (a-1)^2)  * (1 + (a-1)^4) *...
 
 E.g., see 
 Dumas, On Newton-Raphson iteration for multiplicative inverses modulo prime powers
 and
 E.V. Krishnamurthy and Venu K. Murthy. Fast iterative division of p-adic
numbers. IEEE Transactions on Computers, C-32(4):396–398, April 1983

Of some relevance : https://github.com/ridiculousfish/libdivide

*/

uint32_t inverse(uint32_t a) {
  uint32_t U = 2 - a;
  uint32_t am = a - 1;
  am *= am;
  while(am != 0) {
    U *= (am + 1);
    am *= am;
  }
  return U;
}

int testinverse() {
    for(uint32_t i = 0; i < 100; ++i) {
        uint32_t x = 2 * i + 1;
        uint32_t y = inverse(x);
        if(x * y != 1) {
            printf("bug\n");
            return -1;
        } 
        if(0xFFFF * x * y != 0xFFFF) {
            printf("bug\n");
            return -1;
        } 
        if(0xFFFFFFFF * x * y != 0xFFFFFFFF) {
            printf("bug\n");
            return -1;
        } 
    }
    printf("code is probably ok.\n");
    return 0;
}



int main() {
    int r = testinverse();
    if(r < 0)
      return -1;
    return 0;
}
