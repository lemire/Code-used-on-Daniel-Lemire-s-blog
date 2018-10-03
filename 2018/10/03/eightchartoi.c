// gcc -O3 -o experiments experiments.c
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "benchmark.h"



__attribute__ ((noinline))
uint32_t parse_eight_digits(const unsigned char  * chars) {
    uint32_t x = chars[0] - '0';
    for(size_t j = 1; j < 8; j++) x = x * 10 + (chars[j] - '0');
    return x;
}

__attribute__ ((noinline))
uint32_t parse_eight_digits_unrolled(const unsigned char  * chars) {
    uint32_t x =
      10000000 * (chars[0] )
    + 1000000 * (chars[1])
    + 100000 * (chars[2])
    + 10000 * (chars[3])
    + 1000 * (chars[4])
    + 100 * (chars[5])
    + 10 * (chars[6])
    + (chars[7]);
    return x - '0' *(1+10+100+1000+10000+100000+1000000+10000000);
}




__attribute__ ((noinline))
uint32_t parse_eight_digits_swar(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);
    val = __builtin_bswap64(val);// because we are under little endian
    val = val - 0x3030303030303030;
    uint64_t byte10plus = ((val * (0xa + (1<<8))) >> 8)&  0x00FF00FF00FF00FF; 
    uint64_t short100plus = ((byte10plus * (0x64 + (1<<16)) >> 16)) & 0x0000FFFF0000FFFF ;
    short100plus *= (10000 + (1ULL<<32));
    return short100plus >> 32;
}

#include <x86intrin.h>

__attribute__ ((noinline))
uint32_t parse_eight_digits_ssse3(const unsigned char *chars) {
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i mul_1_10 =
      _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
  const __m128i mul_1_100 = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1);
  const __m128i mul_1_10000 =
      _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);
  const __m128i input = _mm_sub_epi8(_mm_loadl_epi64((__m128i *)chars), ascii0);
  const __m128i t1 = _mm_maddubs_epi16(input, mul_1_10);// a0 + 10 * a1,  a2 + 10 * a3,  a4 + 10 * a5,  a6 + 10 * a7,...
  const __m128i t2 = _mm_madd_epi16(t1, mul_1_100);//a0 + 10 * a1 + 100 * a2 + 1000 * a3,  a4 + 10 * a5 + 100*  a6 + 1000 * a7,...
  const __m128i t3 = _mm_packus_epi32(t2, t2);
  const __m128i t4 = _mm_madd_epi16(t3, mul_1_10000);
  return _mm_cvtsi128_si32(
      t4); // only captures the sum of the first 8 digits, drop the rest
}

__attribute__ ((noinline))
uint64_t parse_eight_digits_avx(const unsigned char* s) {
        __m256i multiplier = _mm256_setr_epi32(10000000,1000000,100000,10000,1000,100,10,1);
       __m256i x = _mm256_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)s));
       __m256i xtimesdec = _mm256_mullo_epi32(x,multiplier);
       __m256i flipped =_mm256_shuffle_epi32(xtimesdec,0b01001110);
       __m256i sum1 = _mm256_add_epi32(xtimesdec,flipped);
       __m256i sum2 = _mm256_hadd_epi32(sum1, sum1);
      return _mm256_extract_epi32(sum2,0) + _mm256_extract_epi32(sum2,4)  - '0' *(1 + 10 + 100 + 1000 + 10000 + 100000 + 1000000 + 10000000);
}


//

size_t sum_8_digits(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 7 < length; i++) {
        s += parse_eight_digits(source + i);
    } 
    return s;
}

size_t sum_8_digits_unrolled(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 7 < length; i++) {
        s += parse_eight_digits_unrolled(source + i);
    } 
    return s;
}

size_t sum_8_digits_swar(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 7 < length; i++) {
        s += parse_eight_digits_swar(source + i);
    } 
    return s;
}


size_t sum_8_digits_ssse3(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 7 < length; i++) {
        s += parse_eight_digits_ssse3(source + i);
    } 
    return s;
}


size_t sum_8_digits_avx(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 7 < length; i++) {
        s += parse_eight_digits_avx(source + i);
    } 
    return s;
}

unsigned char * buildcollection(size_t length) {
    unsigned char * answer = (unsigned char *) malloc(length);
    for(size_t i = 0; i < length; i++) {
      answer[i] = 0x30 + (rand() % 10);
    } 
    return answer;
}

bool testparse(size_t N) {
    unsigned char * text = buildcollection(N);
    size_t expected = sum_8_digits(text,N);
    size_t volume = N - 7;
    int repeat = 5;
    BEST_TIME(sum_8_digits(text,N), expected, , repeat, volume, true) ;
    BEST_TIME(sum_8_digits_unrolled(text,N), expected, , repeat, volume, true) ;
    BEST_TIME(sum_8_digits_swar(text,N), expected, , repeat, volume, true) ;
    BEST_TIME(sum_8_digits_ssse3(text,N), expected, , repeat, volume, true) ;
    BEST_TIME(sum_8_digits_avx(text,N), expected, , repeat, volume, true) ;

    free(text);
    return true;
}
int main() {
 if(!testparse(1000000)) return EXIT_FAILURE;
 return EXIT_SUCCESS;
}

