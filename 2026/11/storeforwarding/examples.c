// Small C patterns that store a value and then read it back.
// Compile with: cc -O3 -S examples.c
//
// A same-size reload is usually deleted by the compiler: it returns the
// register it just stored. A narrower store followed by a wider load is not.
// The other bytes were already in memory, so the compiler has to emit the load.
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Store 8 bytes and read those same 8 bytes.
uint64_t reload_same(char *p, uint64_t x) {
    memcpy(p, &x, 8);
    uint64_t w;
    memcpy(&w, p, 8);
    return w;
}

// Store 4 bytes and read 8. The upper half is whatever was already there.
uint64_t reload_wider(char *p, uint32_t v) {
    memcpy(p, &v, 4);
    uint64_t w;
    memcpy(&w, p, 8);
    return w;
}

// Store one byte and read 8.
uint64_t reload_byte(char *p, unsigned char v) {
    // Copy one byte from v into the buffer.
    memcpy(p, &v, 1);
    uint64_t w;
    // Copy eight bytes from the buffer into w.
    memcpy(&w, p, 8);
    return w;
}

// Store 8 bytes, then read 8 bytes starting 4 bytes later.
// The two ranges overlap. Neither contains the other.
uint64_t reload_overlap(char *p, uint64_t x) {
    memcpy(p, &x, 8);
    uint64_t w;
    memcpy(&w, p + 4, 8);
    return w;
}

// Two 4-byte stores, then one 8-byte load.
uint64_t reload_two(char *p, uint32_t lo, uint32_t hi) {
    memcpy(p, &lo, 4);
    memcpy(p + 4, &hi, 4);
    uint64_t w;
    memcpy(&w, p, 8);
    return w;
}

// Same byte update, done in a register. The caller uses that register.
uint64_t update_byte(char *p, unsigned char v) {
    uint64_t x;
    memcpy(&x, p, 8);
    x = (x & ~0xffull) | v;
    memcpy(p, &x, 8);
    return x;
}

// Two halves joined in a register. Nothing is reloaded.
uint64_t join_halves(uint32_t lo, uint32_t hi) {
    return (uint64_t)lo | ((uint64_t)hi << 32);
}

int main(void) {
    unsigned char buf[16];
    memset(buf, 0x11, sizeof buf);

    uint64_t wider = reload_wider((char *)buf, 0xAABBCCDDu);
    memset(buf, 0x11, sizeof buf);
    uint64_t byte = reload_byte((char *)buf, 0xAB);
    memset(buf, 0x11, sizeof buf);
    uint64_t two = reload_two((char *)buf, 0xAABBCCDDu, 0x11223344u);
    uint64_t fixed = update_byte((char *)buf, 0xAB);

    printf("wider       %016llx\n", (unsigned long long)wider);
    printf("byte        %016llx\n", (unsigned long long)byte);
    printf("two         %016llx\n", (unsigned long long)two);
    printf("update_byte %016llx\n", (unsigned long long)fixed);
    printf("join        %016llx\n",
           (unsigned long long)join_halves(0xAABBCCDDu, 0x11223344u));
    return 0;
}
