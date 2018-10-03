// gcc -O3 -o experiments experiments.c
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "benchmark.h"
//
// conventional solution
//

bool is_made_of_eight_digits(const unsigned char * chars) {
    for(size_t j = 0; j < 8; j++)
          if((chars[j] < 0x30) || (chars[j] > 0x39)) return false;
    return true;
}

// Mula http://0x80.pl/articles/swar-digits-validate.html
// Unclear whether it is correct.
bool is_made_of_eight_digits_mula(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);

    if ((val & 0xf0f0f0f0f0f0f0f0llu) == 0x3030303030303030llu) { // any nibble different
        return false;
    }
    if ((val + 0x7676767676767676llu) & 0x8080808080808080llu) { // any value 0x3a .. 0x3f
        return false;
    }
    return true;
}


// SWAR
bool is_made_of_eight_digits_branchy(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);
    return (( val & 0xF0F0F0F0F0F0F0F0 ) == 0x3030303030303030)
    && (( (val + 0x0606060606060606) & 0xF0F0F0F0F0F0F0F0 ) == 0x3030303030303030); 
}

bool is_made_of_eight_digits_branchless(const unsigned char  * chars) {
    uint64_t val;
    memcpy(&val, chars, 8);
    // simpler version due to Travis Downs.
    return ( (val | (val + 0x0606060606060606)) & 0xF0F0F0F0F0F0F0F0 ) == 0x3030303030303030;
    //return ((( val & 0xF0F0F0F0F0F0F0F0 ) 
    //| (( (val + 0x0606060606060606) & 0xF0F0F0F0F0F0F0F0 ) >> 4)) == 0x3333333333333333);
}

///////////////////


size_t generatefloats(unsigned char * stroutput, size_t length) {
  size_t pos = 0;
  for(size_t i = 0; i < length; i++) {
    double output = rand() * 1.0 / RAND_MAX; // between 0 and 1
    pos += sprintf((char *)stroutput + pos, "%.16f,",output);
  }
  stroutput[pos]='\0';
  pos++;
  return pos;
}

size_t generatevarfloats(unsigned char * stroutput, size_t length) {
  size_t pos = 0;
  for(size_t i = 0; i < length; i++) {
    double output = rand() * 1.0 / RAND_MAX; // between 0 and 1
    pos += sprintf((char *)stroutput + pos, "%.*f,", rand() % 20 + 1, output);
    //pos += sprintf((char *)stroutput + pos, "%.12f,",output);
  }
  stroutput[pos]='\0';
  pos++;
  return pos;
}

// testing
bool compare8gramsOfDigits(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        bool b0 = is_made_of_eight_digits(str + i);
        bool b1 = is_made_of_eight_digits_branchy(str + i);
        bool b2 = is_made_of_eight_digits_branchless(str + i);

        if((b0 != b1) || (b1 != b2)) {
            printf("%.8s\n", str+i);
            return false;
        }
    }
    return true;
}

size_t count8gramsOfDigits(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        if(is_made_of_eight_digits(str + i)) counter++;
    }
    return counter;
}

size_t count8gramsOfDigits_mula(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        if(is_made_of_eight_digits_mula(str + i)) counter++;
    }
    return counter;
}

size_t count8gramsOfDigits_branchy(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        if(is_made_of_eight_digits_branchy(str + i)) counter++;
    }
    return counter;
}

size_t count8gramsOfDigits_branchless(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        if(is_made_of_eight_digits_branchless(str + i)) counter++;
    }
    return counter;
}


size_t count8gramsOfDigits_branchless2_step(const unsigned char* str, size_t length) {
    size_t counter = 0;
    for(size_t i = 0; i < length - 7; i++) {
        if(is_made_of_eight_digits_branchless(str + i)) counter++;
    }
    return counter;
}

bool testfloats(size_t N) {

    unsigned char* stroutput = (unsigned char*) malloc(N * 128); // it should never be more than 128 bytes per number
    size_t length ;
    length = generatefloats(stroutput, N);
    if( ! compare8gramsOfDigits(stroutput, length) ) {
        printf("They disagree.\n");
        free(stroutput);
        return false;
    }
    int repeat = 5;
    size_t expected = count8gramsOfDigits(stroutput,length);
    BEST_TIME(count8gramsOfDigits(stroutput,length), expected, , repeat, length - 7, true) ;
    BEST_TIME(count8gramsOfDigits_branchy(stroutput,length), expected, , repeat, length - 7, true) ;
    BEST_TIME(count8gramsOfDigits_branchless(stroutput,length), expected, , repeat, length - 7, true) ;


    length = generatevarfloats(stroutput, N);
    if( ! compare8gramsOfDigits(stroutput, length) ) {
        printf("They disagree.\n");
        free(stroutput);
        return false;
    }
    printf("\n");
    expected = count8gramsOfDigits(stroutput,length);
    BEST_TIME(count8gramsOfDigits(stroutput,length), expected, , repeat, length - 7, true) ;
    BEST_TIME(count8gramsOfDigits_branchy(stroutput,length), expected, , repeat, length - 7, true) ;
    BEST_TIME(count8gramsOfDigits_branchless(stroutput,length), expected, , repeat, length - 7, true) ;

    free(stroutput);
    return true;
}

int main() {
 if(!testfloats(1000000)) return EXIT_FAILURE;
 return EXIT_SUCCESS;
}

