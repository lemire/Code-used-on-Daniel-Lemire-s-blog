#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
uint16_t wyhash16_x;    ///< State for wyhash16 

uint32_t hash16(uint32_t input, uint32_t key) {
    uint32_t hash = input * key;
    return ((hash >> 16) ^ hash) & 0xFFFF;
}

void wyhash16_seed(const uint16_t seed) {
    wyhash16_x = seed;
}

uint16_t wyhash16(void) {
    wyhash16_x += 0xfc15;
    return hash16(wyhash16_x, 0x2ab);
}

uint16_t rand_range16(const uint16_t s) {
    uint16_t x = wyhash16();
    uint32_t m = (uint32_t)x * (uint32_t)s;
    uint16_t l = (uint16_t)m;
    if (l < s) {
        uint16_t t = -s % s;
        while (l < t) {
            x = wyhash16();
            m = (uint32_t)x * (uint32_t)s;
            l = (uint16_t)m;
        }
    }
    return m >> 16;
}

int main() {
    wyhash16_seed(1);
    for(int k = 0; k < 40; k++) {
        printf("%d\n",rand_range16(6000));
    }
    return EXIT_SUCCESS;
}
