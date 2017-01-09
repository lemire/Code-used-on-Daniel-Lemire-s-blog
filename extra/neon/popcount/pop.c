// gcc -O3 -o pop pop.c  -mfpu=neon
// cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq 
#define __USE_POSIX199309
#define __USE_XOPEN
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <arm_neon.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
uint8_t lookup8bit[256] = {
    /* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
    /* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
    /* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
    /* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4,
    /* 10 */ 1, /* 11 */ 2, /* 12 */ 2, /* 13 */ 3,
    /* 14 */ 2, /* 15 */ 3, /* 16 */ 3, /* 17 */ 4,
    /* 18 */ 2, /* 19 */ 3, /* 1a */ 3, /* 1b */ 4,
    /* 1c */ 3, /* 1d */ 4, /* 1e */ 4, /* 1f */ 5,
    /* 20 */ 1, /* 21 */ 2, /* 22 */ 2, /* 23 */ 3,
    /* 24 */ 2, /* 25 */ 3, /* 26 */ 3, /* 27 */ 4,
    /* 28 */ 2, /* 29 */ 3, /* 2a */ 3, /* 2b */ 4,
    /* 2c */ 3, /* 2d */ 4, /* 2e */ 4, /* 2f */ 5,
    /* 30 */ 2, /* 31 */ 3, /* 32 */ 3, /* 33 */ 4,
    /* 34 */ 3, /* 35 */ 4, /* 36 */ 4, /* 37 */ 5,
    /* 38 */ 3, /* 39 */ 4, /* 3a */ 4, /* 3b */ 5,
    /* 3c */ 4, /* 3d */ 5, /* 3e */ 5, /* 3f */ 6,
    /* 40 */ 1, /* 41 */ 2, /* 42 */ 2, /* 43 */ 3,
    /* 44 */ 2, /* 45 */ 3, /* 46 */ 3, /* 47 */ 4,
    /* 48 */ 2, /* 49 */ 3, /* 4a */ 3, /* 4b */ 4,
    /* 4c */ 3, /* 4d */ 4, /* 4e */ 4, /* 4f */ 5,
    /* 50 */ 2, /* 51 */ 3, /* 52 */ 3, /* 53 */ 4,
    /* 54 */ 3, /* 55 */ 4, /* 56 */ 4, /* 57 */ 5,
    /* 58 */ 3, /* 59 */ 4, /* 5a */ 4, /* 5b */ 5,
    /* 5c */ 4, /* 5d */ 5, /* 5e */ 5, /* 5f */ 6,
    /* 60 */ 2, /* 61 */ 3, /* 62 */ 3, /* 63 */ 4,
    /* 64 */ 3, /* 65 */ 4, /* 66 */ 4, /* 67 */ 5,
    /* 68 */ 3, /* 69 */ 4, /* 6a */ 4, /* 6b */ 5,
    /* 6c */ 4, /* 6d */ 5, /* 6e */ 5, /* 6f */ 6,
    /* 70 */ 3, /* 71 */ 4, /* 72 */ 4, /* 73 */ 5,
    /* 74 */ 4, /* 75 */ 5, /* 76 */ 5, /* 77 */ 6,
    /* 78 */ 4, /* 79 */ 5, /* 7a */ 5, /* 7b */ 6,
    /* 7c */ 5, /* 7d */ 6, /* 7e */ 6, /* 7f */ 7,
    /* 80 */ 1, /* 81 */ 2, /* 82 */ 2, /* 83 */ 3,
    /* 84 */ 2, /* 85 */ 3, /* 86 */ 3, /* 87 */ 4,
    /* 88 */ 2, /* 89 */ 3, /* 8a */ 3, /* 8b */ 4,
    /* 8c */ 3, /* 8d */ 4, /* 8e */ 4, /* 8f */ 5,
    /* 90 */ 2, /* 91 */ 3, /* 92 */ 3, /* 93 */ 4,
    /* 94 */ 3, /* 95 */ 4, /* 96 */ 4, /* 97 */ 5,
    /* 98 */ 3, /* 99 */ 4, /* 9a */ 4, /* 9b */ 5,
    /* 9c */ 4, /* 9d */ 5, /* 9e */ 5, /* 9f */ 6,
    /* a0 */ 2, /* a1 */ 3, /* a2 */ 3, /* a3 */ 4,
    /* a4 */ 3, /* a5 */ 4, /* a6 */ 4, /* a7 */ 5,
    /* a8 */ 3, /* a9 */ 4, /* aa */ 4, /* ab */ 5,
    /* ac */ 4, /* ad */ 5, /* ae */ 5, /* af */ 6,
    /* b0 */ 3, /* b1 */ 4, /* b2 */ 4, /* b3 */ 5,
    /* b4 */ 4, /* b5 */ 5, /* b6 */ 5, /* b7 */ 6,
    /* b8 */ 4, /* b9 */ 5, /* ba */ 5, /* bb */ 6,
    /* bc */ 5, /* bd */ 6, /* be */ 6, /* bf */ 7,
    /* c0 */ 2, /* c1 */ 3, /* c2 */ 3, /* c3 */ 4,
    /* c4 */ 3, /* c5 */ 4, /* c6 */ 4, /* c7 */ 5,
    /* c8 */ 3, /* c9 */ 4, /* ca */ 4, /* cb */ 5,
    /* cc */ 4, /* cd */ 5, /* ce */ 5, /* cf */ 6,
    /* d0 */ 3, /* d1 */ 4, /* d2 */ 4, /* d3 */ 5,
    /* d4 */ 4, /* d5 */ 5, /* d6 */ 5, /* d7 */ 6,
    /* d8 */ 4, /* d9 */ 5, /* da */ 5, /* db */ 6,
    /* dc */ 5, /* dd */ 6, /* de */ 6, /* df */ 7,
    /* e0 */ 3, /* e1 */ 4, /* e2 */ 4, /* e3 */ 5,
    /* e4 */ 4, /* e5 */ 5, /* e6 */ 5, /* e7 */ 6,
    /* e8 */ 4, /* e9 */ 5, /* ea */ 5, /* eb */ 6,
    /* ec */ 5, /* ed */ 6, /* ee */ 6, /* ef */ 7,
    /* f0 */ 4, /* f1 */ 5, /* f2 */ 5, /* f3 */ 6,
    /* f4 */ 5, /* f5 */ 6, /* f6 */ 6, /* f7 */ 7,
    /* f8 */ 5, /* f9 */ 6, /* fa */ 6, /* fb */ 7,
    /* fc */ 6, /* fd */ 7, /* fe */ 7, /* ff */ 8
};

int  builtinpop(const uint8_t* data, const size_t size) {
    size_t i = 0;
    uint64_t word;
    int sum;
    for (; i + 7 < size; i += 8) {
        memcpy((void *)(&word),(const void *)(data+i),sizeof(uint64_t));
        sum+= __builtin_popcountll(word);
    }
    for (; i<size; i++) {
        sum += lookup8bit[data[i]];
    }
    return sum;
}
uint64_t popcnt_neon_vcnt0(const uint8_t* data, const size_t size)
{
    const size_t chunk_size = 16 * 4 * 1;

    const uint8_t* ptr = data;

    const size_t n = size / chunk_size;
    const size_t k = size % chunk_size;

    uint32x4_t sum = vcombine_u32(vcreate_u32(0), vcreate_u32(0));
    size_t i = 0;
    size_t j;
    for (; i < n; i++, ptr += chunk_size) {

        uint8x16x4_t input0 = vld4q_u8(ptr + 0 * 16 * 4);


        uint8x16_t t0   = vcntq_u8(input0.val[0]);
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[1]));
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[2]));
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[3]));

        const uint16x8_t t1 = vpaddlq_u8(t0);

        sum = vpadalq_u16(sum, t1);
    }

    uint32_t scalar = 0;
    uint32_t tmp[4];

    vst1q_u32(tmp, sum);
    for (i=0; i < 4; i++) {
        scalar += tmp[i];
    }

    for (j=0; j < k; j++) {
        scalar += lookup8bit[ptr[j]];
    }

    return scalar;
}




uint64_t popcnt_neon_vcnt(const uint8_t* data, const size_t size)
{
    const size_t chunk_size = 16 * 4 * 2;

    const uint8_t* ptr = data;

    const size_t n = size / chunk_size;
    const size_t k = size % chunk_size;

    uint32x4_t sum = vcombine_u32(vcreate_u32(0), vcreate_u32(0));
    size_t i = 0;
    size_t j;
    for (; i < n; i++, ptr += chunk_size) {

        uint8x16x4_t input0 = vld4q_u8(ptr + 0 * 16 * 4);
        uint8x16x4_t input1 = vld4q_u8(ptr + 1 * 16 * 4);

        uint8x16_t t0   = vcntq_u8(input0.val[0]);
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[1]));
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[2]));
        t0 = vaddq_u8(t0, vcntq_u8(input0.val[3]));

        t0 = vaddq_u8(t0, vcntq_u8(input1.val[0]));
        t0 = vaddq_u8(t0, vcntq_u8(input1.val[1]));
        t0 = vaddq_u8(t0, vcntq_u8(input1.val[2]));
        t0 = vaddq_u8(t0, vcntq_u8(input1.val[3]));

        const uint16x8_t t1 = vpaddlq_u8(t0);

        sum = vpadalq_u16(sum, t1);
    }

    uint32_t scalar = 0;
    uint32_t tmp[4];

    vst1q_u32(tmp, sum);
    for (i=0; i < 4; i++) {
        scalar += tmp[i];
    }

    for (j=0; j < k; j++) {
        scalar += lookup8bit[ptr[j]];
    }

    return scalar;
}


int main() {
    const int N = 1024;
    uint8_t * array = malloc(N);
    int k, t;
    for(k = 0; k < N; k++) array[k] = 12 + 3 * k;
    uint64_t expected = builtinpop(array,N); //popcnt_neon_vcnt(array,N);


    struct timespec now;
    struct timespec last;
    long nanosec_diff;
    long minneon = 9999999;
    for(t = 0; t < 5000; ++t) {
        uint64_t answer;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
        answer = popcnt_neon_vcnt(array,N);
        if( answer != expected) { printf("bug"); break;}
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
        nanosec_diff = now.tv_nsec - last.tv_nsec;
        if(nanosec_diff < minneon) minneon = nanosec_diff;
        printf("Elapsed time (neon): %d ns.\n", nanosec_diff);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
        answer = popcnt_neon_vcnt0(array,N);
        if( answer != expected) printf("bug");
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
        nanosec_diff = now.tv_nsec - last.tv_nsec;
        printf("Elapsed time (neon0): %d ns.\n", nanosec_diff);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &last);
        answer = builtinpop(array,N);
        if( answer != expected) printf("bug");
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
        nanosec_diff = now.tv_nsec - last.tv_nsec;
        printf("Elapsed time (builtin): %d ns.\n", nanosec_diff);
        printf("\n");
    }
    printf("minneon = %d ns \n", minneon);
    printf("minneon = %f ns/byte \n", minneon*1.0/N);
     free(array);
}
