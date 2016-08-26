// gcc -O3 -o linearinverse linearinverse.c -Wall -Wextra -march=native
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>


enum {w = 5};
const uint64_t MAX = UINT64_C(1) << w;
const uint64_t MAX2 = UINT64_C(1) << (2 * w);
const uint64_t mask = ( UINT64_C(1) << w)  - 1;


uint64_t hash(uint64_t a, uint64_t b, uint64_t x) {
    return ((a * x + b) >> w) & mask;// could be >> (w+1)
}

void clearbitset(uint64_t * bitsetbuffer) {
    memset(bitsetbuffer, 0, MAX / 8);
}

// return true if bit is changed
bool setbit(uint64_t * bitsetbuffer, uint64_t i) {
    assert(i < MAX);
    uint64_t bitmask = (UINT64_C(1) << (i & 63 ));
    if((bitsetbuffer[i / 64] & bitmask) != 0) return false;
    bitsetbuffer[i / 64] |= bitmask;
    return true;
}

bool is_full(uint64_t * bitsetbuffer) {
    for(size_t i = 0 ; i < MAX / 64; ++i) {
        if(bitsetbuffer[i] != ~UINT64_C(0)) return false;
    }
    return true;
}


bool is_invertible(uint64_t a, uint64_t b, uint64_t * bitsetbuffer) {
    clearbitset(bitsetbuffer);
    for (uint64_t i = 0; i < MAX; ++i) {
        uint64_t h = hash(a,b,i);
        assert(h < MAX); 
        if(!setbit(bitsetbuffer,h)) return false;
    }
    return true;
}


uint64_t * enumeratems(size_t * total) {
    uint64_t * bitsetbuffer = malloc(MAX/8 + 8);

    size_t count = 0;
    for(uint64_t a = 0; a < MAX2 ; a++) {
        uint64_t b = 0;
        if(is_invertible(a,b,bitsetbuffer)) count ++;
    }
    *total = count;

    uint64_t * answer = malloc( sizeof(uint64_t) * count);
    answer[*total - 1] = 0;
    count = 0;
    for(uint64_t a = 0; a < MAX2 ; a++) {
        uint64_t b = 0;
        if(is_invertible(a,b,bitsetbuffer)) {
            answer[ count ] = a;
            count ++;
        }
    }
    free(bitsetbuffer);
    return answer;
}

uint64_t * oddenumeratems(size_t * total) {
   *total = MAX / 2;

    uint64_t * answer = malloc( sizeof(uint64_t) * MAX/2);
    size_t count = 0;
    for(uint64_t a = 0; a < MAX / 2 ; a++) {
        answer [ count ++ ] = ( 2 * a + 1 ) << w;
   }
   return answer;
}


void check_indms(bool odd) {
    printf("check_indms...\n");

    double maxprob = 0;
    uint64_t badv1 = 0, badv2 = 0;
    uint64_t bady1 = 0, bady2 = 0;
    size_t cases;
    printf("precomputing cases...\n");
    uint64_t * buffer = odd ? oddenumeratems(&cases) : enumeratems(&cases);
    printf("precomputing cases...ok %zu \n", cases);
    for(uint64_t v1 = 0; v1 < MAX ; v1++) {
        for(uint64_t v2 = v1 + 1; v2 < MAX ; v2++) {
            for(uint64_t y1 = 0; y1 < MAX ; y1++) {
                for(uint64_t y2 = 0; y2 < MAX ; y2++) {
                    size_t total = 0;
                    size_t collisions = 0;
                    for(; total < cases; ++total) {
                                 assert(total < cases);
                        uint64_t a = buffer[total];
                        uint64_t b = 0;
                        if((hash(a,b,v1) == y1) && (hash(a,b,v2) == y2)) ++ collisions;
                    }

                    /*                    for(uint64_t a = 0; a < MAX2 ; a++) {
                                            uint64_t b = 0;//for(uint64_t b = 0; b < MAX2 ; b++) {
                                            if(is_invertible(a,b,bitsetbuffer)) {
                                                total++;
                                                if((hash(a,b,v1) == y1) && (hash(a,b,v2) == y2)) ++ collisions;
                                            }
                                        }*/
                    double thisprob = collisions*1.0/total;
                    if(thisprob > maxprob) {
                        maxprob = thisprob;
                        badv1 = v1;
                        badv2 = v2;
                        bady1 = y1;
                        bady2 = y2;

                    }
                }
            }

        }
    }
    free(buffer);

    printf("%zu %zu  %zu %zu\n", badv1,badv2, bady1,bady2);

    printf("%f %f\n", maxprob, 1.0/MAX);
}

uint64_t * enumerate(size_t * total) {
    uint64_t * bitsetbuffer = malloc(MAX/8 + 8);

    size_t count = 0;
    for(uint64_t a = 0; a < MAX2 ; a++) {
        for(uint64_t b = 0; b < MAX2 ; b++) {
            if(is_invertible(a,b,bitsetbuffer)) count ++;
        }
    }
    *total = count;

    uint64_t * answer = malloc( 2 * sizeof(uint64_t) * count);
    count = 0;
    for(uint64_t a = 0; a < MAX2 ; a++) {
        for(uint64_t b = 0; b < MAX2 ; b++) {
            if(is_invertible(a,b,bitsetbuffer)) {
                answer[2 * count ] = a;
                answer[2 * count + 1] = b;
                count ++;
            }
        }
    }
    free(bitsetbuffer);
    return answer;
}

void check_ind() {
    printf("check_ind...\n");

    double maxprob = 0;
    uint64_t badv1 = 0, badv2 = 0;
    uint64_t bady1 = 0, bady2 = 0;
    size_t cases;
    printf("precomputing cases...\n");

    uint64_t * buffer = enumerate(&cases);
    printf("precomputing cases... ok %zu \n", cases);

    for(uint64_t v1 = 0; v1 < MAX ; v1++) {
        for(uint64_t v2 = v1 + 1; v2 < MAX ; v2++) {
            for(uint64_t y1 = 0; y1 < MAX ; y1++) {
                for(uint64_t y2 = 0; y2 < MAX ; y2++) {
                    size_t total = 0;
                    size_t collisions = 0;
                    for(; total < cases; ++total) {
                        uint64_t a = buffer[2* total];
                        uint64_t b = buffer[2* total + 1];
                        if((hash(a,b,v1) == y1) && (hash(a,b,v2) == y2)) ++ collisions;
                    }
                    /*
                                        for(uint64_t a = 0; a < MAX2 ; a++) {
                                            for(uint64_t b = 0; b < MAX2 ; b++) {
                                                if(is_invertible(a,b,bitsetbuffer)) {
                                                    total++;
                                                    if((hash(a,b,v1) == y1) && (hash(a,b,v2) == y2)) ++ collisions;
                                                }
                                            }
                                        }*/
                    double thisprob = collisions*1.0/total;
                    if(thisprob > maxprob) {
                        maxprob = thisprob;
                        badv1 = v1;
                        badv2 = v2;
                        bady1 = y1;
                        bady2 = y2;

                    }
                }
            }

        }
    }
    free(buffer);
    printf("%zu %zu  %zu %zu\n", badv1,badv2, bady1,bady2);

    printf("%f %f\n", maxprob, 1.0/MAX);
}

int main() {
    //check_universal();
    check_indms(false);
    check_indms(true);
     check_ind();

    uint64_t * bitsetbuffer = malloc(MAX/8+8);
    size_t counter = 0;
    printf("%zu \n",(size_t)MAX);
    for(uint64_t a = 0; a < MAX2 ; a++) {
        if(is_invertible(a,0,bitsetbuffer)) {
            printf("%u ",a % MAX);
            counter ++;
        }
    }
    printf("\n");
    printf("%zu \n",counter);
    for(uint64_t a = 0; a < MAX/2 ; a++) {
        if(!is_invertible((2*a+1)<<w,0,bitsetbuffer)) {
            printf("not invertible %u \n",(unsigned int) ((2*a+1)<<w));
        }
    }
    counter = 0;
    for(uint64_t a = 0; a < MAX2 ; a++) {
        for(uint64_t b = 0; b < MAX2 ; b++) {
            if(is_invertible(a,b,bitsetbuffer)) {
                printf("%u ",a % MAX);
                counter ++;
                break;
            }
        }
    }
    printf("%zu \n",counter);
    free(bitsetbuffer);
    return EXIT_SUCCESS;
}
