// cc -O3 -o name name.c && ./name|hexdump -C 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>

// xoroshiro generator taken from
// http://vigna.di.unimi.it/xorshift/xoroshiro128plus.c

uint64_t s[2];

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

uint64_t next(void) {
	const uint64_t s0 = s[0];
	uint64_t s1 = s[1];
	const uint64_t result = s0 + s1;

	s1 ^= s0;
	s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
	s[1] = rotl(s1, 36); // c
	return result;
}
int main(int argc, char **argv) {
    freopen(NULL, "wb", stdout);
    s[0] = 0X922AC4EB35B502D9L;
    s[1] = 0XDA3AA4832B8F1D27L;
    for(int k = 0; k < 5; k++) {
        uint64_t value = next();
        fwrite((void*) &value, sizeof(value), 1, stdout);
    }
}
