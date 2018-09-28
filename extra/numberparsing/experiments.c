// gcc -O3 -o experiments experiments.c
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

bool is_made_of_four_digits(const unsigned char * chars) {
    for(size_t j = 0; j < 4; j++)
          if((chars[j] < 0x30) || (chars[j] > 0x39)) return false;
    return true;
}

bool is_made_of_four_digits_fast(const unsigned char  * chars) {
    uint32_t val;
    memcpy(&val, chars, 4);
    return ((( val & 0xF0F0F0F0 ) | (( (val + 0x06060606) & 0xF0F0F0F0 ) >> 4)) == 0x33333333);
}


bool is_made_of_eight_digits(const unsigned char * chars) {
    for(size_t j = 0; j < 8; j++)
          if((chars[j] < 0x30) || (chars[j] > 0x39)) return false;
    return true;
}

bool is_made_of_eight_digits_fast(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);
    return ((( val & 0xF0F0F0F0F0F0F0F0 ) | (( (val + 0x0606060606060606) & 0xF0F0F0F0F0F0F0F0 ) >> 4)) == 0x3333333333333333);
}

/***
 * Mula:
 * 
 *    const __m128i ascii0 = _mm_set1_epi8('0');
 *    const __m128i ascii9 = _mm_set1_epi8('9');
 *
 *    for (i=0; i < size/16; i += 16) {
 *
 *        const __m128i v   = _mm_loadu_si128((__m128i*)string);
 *        const __m128i lt0 = _mm_cmplt_epi8(string, ascii0);
 *        const __m128i gt9 = _mm_cmpgt_epi8(string, ascii9);
 *
 *        const __m128i outside = _mm_or_si128(lt0, gt9);
*/



size_t count_4_digits() {
    size_t count = 0;
    for(uint32_t val = 1; val != 0; val++) {
        const unsigned char * chars = (const unsigned char  *)(& val);
        if(is_made_of_four_digits(chars)) count++;
    }
    return count;
}


size_t count_4_digits_fast() {
    size_t count = 0;
    for(uint32_t val = 1; val != 0; val++) {
        const unsigned char * chars = (const unsigned char  *)(& val);
        if(is_made_of_four_digits_fast(chars)) count++;
    }
    return count;
}


size_t count_8_digits() {
    size_t count = 0;
    for(uint64_t val = 0x3030303000000000; val != 0x3030303100000000; val++) {
        const unsigned char * chars = (const unsigned char  *)(& val);
        if(is_made_of_eight_digits(chars)) count++;
    }
    return count;
}


size_t count_8_digits_fast() {
    size_t count = 0;
    for(uint64_t val = 0x3030303000000000; val != 0x3030303100000000; val++) {
        const unsigned char * chars = (const unsigned char  *)(& val);
        if(is_made_of_eight_digits_fast(chars)) count++;
    }
    return count;
}



bool unit() {
        size_t checks = 0;

        for(size_t i = 0; i <= 0xFFFFFFFF; i++) {
        uint32_t val = i;
        uint8_t * chars = (uint8_t *)(& val);
        bool hasnumbers = true;
        for(size_t j = 0; j < 4; j++)
          if((chars[j] < 0x30) || (chars[j] > 0x39)) hasnumbers = false;
        bool numberdetect = ((( val & 0xF0F0F0F0 ) | (( (val + 0x06060606) & 0xF0F0F0F0 ) >> 4)) == 0x33333333);
        if(hasnumbers != numberdetect) {
            printf("0%x \n",val);
            return false;
        }
        checks++;
    }
    printf("%zu checks\n", checks);
    return true;
}




uint32_t parse_eight_digits(const unsigned char  * chars) {
    uint32_t x = chars[0] - '0';
    for(size_t j = 1; j < 8; j++) x = x * 10 + (chars[j] - '0');
    return x;
}

uint32_t parse_eight_digits_unrolled(const unsigned char  * chars) {
    uint32_t x = 
      10000000 * (chars[0] - '0')
    + 1000000 * (chars[1] - '0')
    + 100000 * (chars[2] - '0')
    + 10000 * (chars[3] - '0')
    + 1000 * (chars[4] - '0')
    + 100 * (chars[5] - '0')
    + 10 * (chars[6] - '0')
    + (chars[7] - '0');
    return x;
}





#include <x86intrin.h>

uint32_t parse_eight_digits_ssse3(const unsigned char  * chars) {
    const __m128i ascii0      = _mm_set1_epi8('0');
    const __m128i mul_1_10    = _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
    const __m128i mul_1_100   = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1); 
    const __m128i mul_1_10000 = _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);

    // =--------------

    const __m128i input   = _mm_sub_epi8(_mm_loadu_si128((__m128i*)chars), ascii0);

    const __m128i t1      = _mm_maddubs_epi16(input, mul_1_10);
    const __m128i t2      = _mm_madd_epi16(t1, mul_1_100);
    const __m128i t3      = _mm_packus_epi32(t2, t2);
    const __m128i t4      = _mm_madd_epi16(t3, mul_1_10000);

    return _mm_cvtsi128_si32(input);
}

// mula
uint64_t parse_ssse3(const char* s) {
    //assert(strlen(s) >= 16);

    const __m128i ascii0      = _mm_set1_epi8('0');
    const __m128i mul_1_10    = _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
    const __m128i mul_1_100   = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1); 
    const __m128i mul_1_10000 = _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);

    // =--------------

    const __m128i input   = _mm_sub_epi8(_mm_loadu_si128((__m128i*)s), ascii0);

    const __m128i t1      = _mm_maddubs_epi16(input, mul_1_10);
    const __m128i t2      = _mm_madd_epi16(t1, mul_1_100);
    const __m128i t3      = _mm_packus_epi32(t2, t2);
    const __m128i t4      = _mm_madd_epi16(t3, mul_1_10000);

    uint32_t p[4];
    _mm_storeu_si128((__m128i*)p, t4);

    return (uint64_t)(p[0]) * 100000000u + (uint64_t)(p[1]);
}

uint32_t parse_eight_digits_mula(const unsigned char  * chars) {
    uint32_t x = 
      10000000 * (chars[0] - '0')
    + 1000000 * (chars[1] - '0')
    + 100000 * (chars[2] - '0')
    + 10000 * (chars[3] - '0')
    + 1000 * (chars[4] - '0')
    + 100 * (chars[5] - '0')
    + 10 * (chars[6] - '0')
    + 1 * (chars[7] - '0');
    return x;
}

unsigned char * buildcollection(size_t length) {
    unsigned char * answer = (unsigned char *) malloc(length);
    for(size_t i = 0; i < length; i++) {
      answer[i] = (unsigned char) (i + (i >> 4));
    } 
    return answer;
}
size_t sum_8_digits(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits(source + i);
    } 
    return s;
}

size_t sum_8_digits_unrolled(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits_unrolled(source + i);
    } 
    return s;
}



size_t sum_8_digits_ssse3(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits_ssse3(source + i);
    } 
    return s;
}

int main() {
    size_t N = 1000000000;
    unsigned char * text = buildcollection(N);
    printf("slow\n");
    long long sbef1 = timeInMilliseconds();
    size_t shw1 = sum_8_digits(text, N);
    long long saft1 = timeInMilliseconds();

    printf("fast\n");
    long long sbef2 = timeInMilliseconds();
    size_t shw2 = sum_8_digits_unrolled(text, N);
    long long saft2 = timeInMilliseconds();
    if(shw1 != shw2) printf("bug\n");


    printf("ssse3\n");
    long long sbef3 = timeInMilliseconds();
    size_t shw3 = sum_8_digits_unrolled(text, N);
    long long saft3 = timeInMilliseconds();
    if(shw1 != shw3) printf("bug\n");
    printf("%lld -- %lld -- %lld \n", saft1-sbef1, saft2-sbef2,  saft3-sbef3);

    free(text);

    printf("slow\n");
    long long bef1 = timeInMilliseconds();
    size_t hw1 = count_8_digits();
    long long aft1 = timeInMilliseconds();

    printf("fast\n");
    long long bef2 = timeInMilliseconds();
    size_t hw2 = count_8_digits_fast();
    long long aft2 = timeInMilliseconds();
    if(hw1 != hw2) printf("bug\n");
    

    printf("%lld -- %lld  \n", aft1-bef1, aft2-bef2);

    return unit() ? EXIT_SUCCESS : EXIT_FAILURE;



}
