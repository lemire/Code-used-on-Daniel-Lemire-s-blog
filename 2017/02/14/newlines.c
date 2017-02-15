// gcc -O3 -o newlines newlines.c -march=native
// inspired by https://github.com/llogiq/newlinebench



#include "benchmark.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "x86intrin.h"

// from https://github.com/llogiq/newlinebench/blob/master/src/lib.rs
char * get_random(size_t size) {
    char * answer = (char*) malloc(size);
    for(size_t i = 0; i < size; i++) {
        answer[i] = (6364136223846793005 * i) >> 56;
    }
    return answer;
}

char * get_realistic(size_t size) {
    char * answer = (char*) malloc(size);
    for(size_t i = 0; i < size; i++) {
        if(rand() % 100 == 57)
            answer[i] = '\n';
        else
            answer[i] = rand();
    }
    return answer;
}


size_t basiccount(char * buffer, size_t size) {
    size_t cnt = 0;
    for(size_t i = 0; i < size; i++)
        if(buffer[i] == '\n') cnt ++;
    return cnt;
}


size_t memchrcount(char * buffer, size_t size) {
    size_t cnt = 0;
    char * ptr = buffer;
    char * last = buffer + size;
    while((ptr = memchr(ptr,'\n', last - ptr))) {
      cnt ++;
      ptr ++;
    }
    return cnt;
}



#define haszero(v) (((v) - 0x0101010101010101llu) & ~(v) & 0x8080808080808080llu) // magick provided by https://graphics.stanford.edu/~seander/bithacks.html 

size_t swarcount(char * buffer, size_t size) {
    size_t cnt = 0;
    const uint64_t nls = 0x0a0a0a0a0a0a0a0allu;
    size_t i=0;
    while (i + 8 < size) {
        const uint64_t v = *(uint64_t*)(buffer + i);
        if (haszero(v ^ nls)) {
            for (size_t j=0; j < 8; j++) { 
                if (buffer[i + j] == '\n') cnt ++;
            }
        }

        i += 8;
    }

    for(/**/; i < size; i++)
        if(buffer[i] == '\n') cnt ++;

    return cnt;
}
size_t avxcount(char * buffer, size_t size) {
    size_t answer = 0;
    __m256i cnt = _mm256_setzero_si256();
    __m256i newline = _mm256_set1_epi8('\n');
    size_t i = 0;
    uint8_t tmpbuffer[sizeof(__m256i)];
    while( i + 32 <= size ) {
        size_t remaining = size - i;
        size_t howmanytimes =  remaining / 32;
        if(howmanytimes > 256) howmanytimes = 256;
        const __m256i * buf = (const __m256i *) (buffer + i);
        for (size_t j = 0; j <  howmanytimes; j++) {
            __m256i newdata = _mm256_lddqu_si256(buf + j);
            __m256i cmp = _mm256_cmpeq_epi8(newline,newdata);
            cnt = _mm256_add_epi8(cnt,cmp);
        }
        i += howmanytimes * 32;
        cnt = _mm256_subs_epi8(_mm256_setzero_si256(),cnt);
        _mm256_storeu_si256((__m256i *) tmpbuffer,cnt);
        for(int k = 0; k < sizeof(__m256i); ++k) answer += tmpbuffer[k];
        cnt = _mm256_setzero_si256();
    }
    for(; i < size; i++)
        if(buffer[i] == '\n') answer ++;
    return answer;
}

size_t avxcountuu(char * buffer, size_t size) {
    size_t answer = 0;
    __m256i cnt = _mm256_setzero_si256();
    __m256i newline = _mm256_set1_epi8('\n');
    size_t i = 0;
    uint8_t tmpbuffer[sizeof(__m256i)];
    while( i + 32 <= size ) {
        size_t remaining = size - i;
        size_t howmanytimes =  remaining / 32;
        if(howmanytimes > 256) howmanytimes = 256;
        const __m256i * buf = (const __m256i *) (buffer + i);
        size_t j = 0;
        for (; j + 1 <  howmanytimes; j+= 2) {
            __m256i newdata1 = _mm256_lddqu_si256(buf + j);
            __m256i newdata2 = _mm256_lddqu_si256(buf + j + 1);
            __m256i cmp1 = _mm256_cmpeq_epi8(newline,newdata1);
            __m256i cmp2 = _mm256_cmpeq_epi8(newline,newdata2);
            __m256i cnt1 = _mm256_add_epi8(cmp1,cmp2);
            cnt = _mm256_add_epi8(cnt,cnt1);
        }
        for (; j <  howmanytimes; j++) {
            __m256i newdata = _mm256_lddqu_si256(buf + j);
            __m256i cmp = _mm256_cmpeq_epi8(newline,newdata);
            cnt = _mm256_add_epi8(cnt,cmp);
        }
        i += howmanytimes * 32;
        cnt = _mm256_subs_epi8(_mm256_setzero_si256(),cnt);
        _mm256_storeu_si256((__m256i *) tmpbuffer,cnt);
        for(int k = 0; k < sizeof(__m256i); ++k) answer += tmpbuffer[k];
        cnt = _mm256_setzero_si256();
    }
    for(; i < size; i++)
        if(buffer[i] == '\n') answer ++;
    return answer;
}



size_t avxcountu(char * buffer, size_t size) {
    size_t answer = 0;
    __m256i cnt = _mm256_setzero_si256();
    __m256i newline = _mm256_set1_epi8('\n');
    size_t i = 0;
    uint8_t tmpbuffer[sizeof(__m256i)];
    while( i + 32 <= size ) {
        size_t remaining = size - i;
        size_t howmanytimes =  remaining / 32;
        if(howmanytimes > 256) howmanytimes = 256;
        const __m256i * buf = (const __m256i *) (buffer + i);
        size_t j = 0;
        for (; j + 3 <  howmanytimes; j+= 4) {
            __m256i newdata1 = _mm256_lddqu_si256(buf + j);
            __m256i newdata2 = _mm256_lddqu_si256(buf + j + 1);
            __m256i newdata3 = _mm256_lddqu_si256(buf + j + 2);
            __m256i newdata4 = _mm256_lddqu_si256(buf + j + 3);
            __m256i cmp1 = _mm256_cmpeq_epi8(newline,newdata1);
            __m256i cmp2 = _mm256_cmpeq_epi8(newline,newdata2);
            __m256i cmp3 = _mm256_cmpeq_epi8(newline,newdata3);
            __m256i cmp4 = _mm256_cmpeq_epi8(newline,newdata4);
            __m256i cnt1 = _mm256_add_epi8(cmp1,cmp2);
            __m256i cnt2 = _mm256_add_epi8(cmp3,cmp4);
            cnt = _mm256_add_epi8(cnt,cnt1);
            cnt = _mm256_add_epi8(cnt,cnt2);
        }
        for (; j <  howmanytimes; j++) {
            __m256i newdata = _mm256_lddqu_si256(buf + j);
            __m256i cmp = _mm256_cmpeq_epi8(newline,newdata);
            cnt = _mm256_add_epi8(cnt,cmp);
        }
        i += howmanytimes * 32;
        cnt = _mm256_subs_epi8(_mm256_setzero_si256(),cnt);
        _mm256_storeu_si256((__m256i *) tmpbuffer,cnt);
        for(int k = 0; k < sizeof(__m256i); ++k) answer += tmpbuffer[k];
        cnt = _mm256_setzero_si256();
    }
    for(; i < size; i++)
        if(buffer[i] == '\n') answer ++;
    return answer;
}



int main() {
    size_t N = 100000;
    char * buffer = get_random(N);
    size_t tc = basiccount(buffer,N);
    const int repeat = 5;
    BEST_TIME(basiccount(buffer,N),tc, , repeat, N, true);
    BEST_TIME(memchrcount(buffer,N),tc, , repeat, N, true);
    BEST_TIME(swarcount(buffer,N),tc, , repeat, N, true);
    BEST_TIME(avxcount(buffer,N),tc, , repeat, N, true);
    BEST_TIME(avxcountu(buffer,N),tc, , repeat, N, true);
    BEST_TIME(avxcountuu(buffer,N),tc, , repeat, N, true);
    free(buffer);
}
