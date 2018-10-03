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

void print8swar(uint64_t x) {
 uint8_t buffer[8];
memcpy(buffer, &x, 8 );
printf("[");
for(int i = 0 ; i < 8; i++) {
    printf("%llu,", buffer[i]);
}
printf("]");
 }


void print16swar(uint64_t x) {
 uint16_t buffer[4];
 memcpy(buffer, &x, 8 );
printf("[");
for(int i = 0 ; i < 4; i++) {
    printf("%llu,", buffer[i]);
}
printf("]");
 }


void print32swar(uint64_t x) {
 uint32_t buffer[2];
 memcpy(buffer, &x, 8 );
printf("[");
for(int i = 0 ; i < 2; i++) {
    printf("%llu,", buffer[i]);
}
printf("]");
 }




__attribute__ ((noinline))
uint32_t parse_eight_digits_swar(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);
    val = __builtin_bswap64(val);// because we are under little endian
    val = val - 0x3030303030303030;
    //printf("\n");
    //print8swar(val);printf("\n");// 1 digit per byte, max val is 9 out of 255
    uint64_t byte10plus = ((val * (0xa + (1<<8))) >> 8)&  0x00FF00FF00FF00FF; 
    //print16swar(byte10plus);printf("\n");// 2 digits per short (max val is 99 out of 65536)
    uint64_t short100plus = ((byte10plus * (0x64 + (1<<16)) >> 16)) & 0x0000FFFF0000FFFF ;
    // print32swar(short100plus);printf("\n");// 4 digits per int (max val is 9999 out of 4 billions)
    short100plus *= (10000 + (1ULL<<32));
    return short100plus >> 32;
}

#include <x86intrin.h>
/*
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

    return _mm_cvtsi128_si32(t4);
}*/

void print8(__m128i x) {
 uint8_t buffer[16];
_mm_storeu_si128((__m128i *)buffer,x);
printf("[");
for(int i = 0 ; i < 16; i++) {
    printf("%d,", buffer[i]);
}
printf("]");
 }


void print16(__m128i x) {
 uint16_t buffer[8];
_mm_storeu_si128((__m128i *)buffer,x);
printf("[");
for(int i = 0 ; i < 8; i++) {
    printf("%d,", buffer[i]);
}
printf("]");
 }


void print32(__m128i x) {
 uint32_t buffer[4];
_mm_storeu_si128((__m128i *)buffer,x);
printf("[");
for(int i = 0 ; i < 4; i++) {
    printf("%d,", buffer[i]);
}
printf("]");
 }

__attribute__ ((noinline))
uint32_t parse_eight_digits_ssse3(const char *chars) {
  // this actually computes *16* values so we are being wasteful.
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i mul_1_10 =
      _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
  const __m128i mul_1_100 = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1);
  const __m128i mul_1_10000 =
      _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);
  const __m128i input = _mm_sub_epi8(_mm_loadu_si128((__m128i *)chars), ascii0);
  //printf("\n");
  //printf("input:");print8(input);printf("\n");
  const __m128i t1 = _mm_maddubs_epi16(input, mul_1_10);// a0 + 10 * a1,  a2 + 10 * a3,  a4 + 10 * a5,  a6 + 10 * a7,...
  //printf("multiplies10:");print16(t1);printf("\n");
  const __m128i t2 = _mm_madd_epi16(t1, mul_1_100);//a0 + 10 * a1 + 100 * a2 + 1000 * a3,  a4 + 10 * a5 + 100*  a6 + 1000 * a7,...
    //printf("multiplies100:");print32(t2);printf("\n");

  const __m128i t3 = _mm_packus_epi32(t2, t2);

      //printf("packedmultiplies100:");print16(t3);printf("\n");

  const __m128i t4 = _mm_madd_epi16(t3, mul_1_10000);
        //printf("multiplies10000:");print32(t4);printf("\n");
        //printf("result = %llu\n", _mm_cvtsi128_si32(t4));
//printf("\n");
  return _mm_cvtsi128_si32(
      t4); // only captures the sum of the first 8 digits, drop the rest
}
__attribute__ ((noinline))
uint32_t parse_eight_digits_ssse3_l64(const char *chars) {
  // this actually computes *16* values so we are being wasteful.
  const __m128i ascii0 = _mm_set1_epi8('0');
  const __m128i mul_1_10 =
      _mm_setr_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
  const __m128i mul_1_100 = _mm_setr_epi16(100, 1, 100, 1, 100, 1, 100, 1);
  const __m128i mul_1_10000 =
      _mm_setr_epi16(10000, 1, 10000, 1, 10000, 1, 10000, 1);
  const __m128i input = _mm_sub_epi8(_mm_loadl_epi64((__m128i *)chars), ascii0);
  //printf("\n");
  //printf("input:");print8(input);printf("\n");
  const __m128i t1 = _mm_maddubs_epi16(input, mul_1_10);// a0 + 10 * a1,  a2 + 10 * a3,  a4 + 10 * a5,  a6 + 10 * a7,...
  //printf("multiplies10:");print16(t1);printf("\n");
  const __m128i t2 = _mm_madd_epi16(t1, mul_1_100);//a0 + 10 * a1 + 100 * a2 + 1000 * a3,  a4 + 10 * a5 + 100*  a6 + 1000 * a7,...
    //printf("multiplies100:");print32(t2);printf("\n");

  const __m128i t3 = _mm_packus_epi32(t2, t2);

      //printf("packedmultiplies100:");print16(t3);printf("\n");

  const __m128i t4 = _mm_madd_epi16(t3, mul_1_10000);
        //printf("multiplies10000:");print32(t4);printf("\n");
        //printf("result = %llu\n", _mm_cvtsi128_si32(t4));
//printf("\n");
  return _mm_cvtsi128_si32(
      t4); // only captures the sum of the first 8 digits, drop the rest
}

// mula
__attribute__ ((noinline))
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
__attribute__ ((noinline))
uint64_t parse_eight_digits_avx(const char* s) {
        __m256i multiplier = _mm256_setr_epi32(10000000,1000000,100000,10000,1000,100,10,1);
       __m256i x = _mm256_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)s));
       __m256i xtimesdec = _mm256_mullo_epi32(x,multiplier);
       __m256i flipped =_mm256_shuffle_epi32(xtimesdec,0b01001110);
       __m256i sum1 = _mm256_add_epi32(xtimesdec,flipped);
       __m256i sum2 = _mm256_hadd_epi32(sum1, sum1);
      return _mm256_extract_epi32(sum2,0) + _mm256_extract_epi32(sum2,4)  - '0' *(1 + 10 + 100 + 1000 + 10000 + 100000 + 1000000 + 10000000);
}

/*
uint32_t parse3(char* s) {

    const uint32_t input = bswap(*reinterpret_cast<uint32_t*>(s));

    const uint32_t t1 = input - (uint32_t)('0' );
    const uint32_t t2 = (t1 * 10) >> 8;

    const uint32_t t3 = t1 + t2;
    const uint32_t t4 = t3 & 0x00ff00ff;

    return (t4 * (100 + 65536)) >> 16;
}*/


unsigned char * buildcollection(size_t length) {
    unsigned char * answer = (unsigned char *) malloc(length);
    for(size_t i = 0; i < length; i++) {
      answer[i] = 0x30 + (rand() % 10);
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

size_t sum_8_digits_swar(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits_swar(source + i);
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

size_t sum_8_digits_ssse3_l64(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits_ssse3_l64(source + i);
    } 
    return s;
}


size_t sum_8_digits_avx(unsigned char *  source, size_t length) {
    uint32_t s = 0;
    for(size_t i = 0; i + 8 < length; i++) {
        s += parse_eight_digits_avx(source + i);
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
    printf("%lld  \n", saft1-sbef1);


    printf("unrolled\n");
    long long sbef2 = timeInMilliseconds();
    size_t shw2 = sum_8_digits_unrolled(text, N);
    long long saft2 = timeInMilliseconds();
    if(shw1 != shw2) printf("bug\n");
    printf("%lld  \n", saft2-sbef2);

    printf("ssse3\n");
    long long sbef3 = timeInMilliseconds();
    size_t shw3 = sum_8_digits_ssse3(text, N);
    long long saft3 = timeInMilliseconds();
    if(shw1 != shw3) printf("bug\n");
    printf("%lld  \n", saft3-sbef3);


    printf("swar\n");
    long long sbef4 = timeInMilliseconds();
    size_t shw4 = sum_8_digits_swar(text, N);
    long long saft4 = timeInMilliseconds();
    if(shw1 != shw4) printf("bug\n");
    printf("%lld  \n", saft4-sbef4);


    printf("ssse3 l64\n");
    long long sbef5 = timeInMilliseconds();
    size_t shw5 = sum_8_digits_ssse3_l64(text, N);
    long long saft5 = timeInMilliseconds();
    if(shw1 != shw5) printf("bug\n");
    printf("%lld  \n", saft5-sbef5);

    printf("avx\n");

    long long sbef6 = timeInMilliseconds();
    size_t shw6 = sum_8_digits_avx(text, N);
    long long saft6 = timeInMilliseconds();
    if(shw1 != shw6) printf("bug\n");
    printf("%lld \n", saft6-sbef6);




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
