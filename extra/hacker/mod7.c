#include <stdio.h>
#include <stdint.h>

uint32_t mod7(uint32_t a) {
  return a % 7;
}

uint32_t fastmod7(uint32_t a) {
    return (((4908534053*a) & ((1ULL<<35)-1)) * 7) >> 35;
}

int main() {
  for(uint32_t x = 1; x !=0; x++) {
    if(mod7(x) != fastmod7(x)) printf("%x\n",x);
  }
}
