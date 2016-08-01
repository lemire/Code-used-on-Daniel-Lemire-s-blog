#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

uint64_t mix64(uint64_t z) {
    z = (z ^ (z >> 33)) * UINT64_C(0xff51afd7ed558ccd);
    z = (z ^ (z >> 33)) * UINT64_C(0xc4ceb9fe1a85ec53);
    return z ^ (z >> 33);
}



uint32_t murmur32(uint32_t h) {
    h ^= h >> 16;
    h *= UINT32_C(0x85ebca6b);
    h ^= h >> 13;
    h *= UINT32_C(0xc2b2ae35);
    h ^= h >> 16;
    return h;
}

int murmurhash(int value, uint64_t key) {
    return mix64(value)&1;
}


int murmurhash32(int value, uint64_t key) {
    return murmur32(value)&1;
}
int hash(int value, uint64_t key) {
    return (key >> value) & 1;
}


typedef int (*hasher)(int value, uint64_t key);


void display(const int input_bits,uint64_t key, hasher h) {
    for( int  value = 0; value < (1<< input_bits); value++) {
        int base = h(value,key);//(key >> value) & 1;
        printf("%d -> %d \n ",value,base);
    }
}

int bias(const int input_bits, uint64_t key, hasher h) {
    int * counter = (int *) malloc(input_bits);
    for( int  k = 0; k < input_bits; k++) {
        counter[k] = 0;
    }

    for( int  value = 0; value < (1<< input_bits); value++) {
        int base = h(value,key);//(key >> value) & 1;
        //printf("%d -> %d --- ",value,base);

        for ( int flip = 0; flip < input_bits; ++ flip) {
            int flippedvalue = value ^ ( 1<< flip);
            int flippedhashvalue = h(flippedvalue,key);
//printf(" cmp %d -> %d ",flippedvalue,flippedhashvalue);

            if(flippedhashvalue != base) {
                counter[flip] ++;
            }
        }
        //      printf("\n");
    }
    int target = (1<< input_bits) / 2;
    int maxdist = 0;
//printf("target = %d ",target);
    for( int  k = 0; k < input_bits; k++) {
        //    printf("%d ",counter[k]);
        int dist = abs(counter[k] - target);
        if(dist > maxdist) maxdist = dist;
    }
    //printf("\n");
    free(counter);
    return maxdist;
}

void demo(int input_bits) {
    printf("**input bits = %d \n", input_bits);
    //assert((1 << input_bits) <= 64);
    uint64_t bestkey = 0;
    int bestdist = bias(input_bits, bestkey,hash);

    int perfectcount = 0;
    int count = 0;
    int numberofbitsinkey = 1<< input_bits;

    for( uint64_t  key = 0; key < (INT64_C(1) <<numberofbitsinkey); key++) {
        if(__builtin_popcountll(key) != numberofbitsinkey/2) continue;
        int dist = bias(input_bits, key,hash);
        count++;
        //printf("key = %d dist = %d \n", (int) key, dist);
        if(dist < bestdist) {
            bestdist = dist;
            bestkey = key;
        }
        if(dist == 0) {
            perfectcount++;
            break;
        }

    }
    printf("perfectcount = %d out of %d  \n", perfectcount, count);

    printf("bestkey = %d bestdist = %d target = %d \n", (int) bestkey, bestdist,(1<< input_bits) / 2);
//    display(input_bits,bestkey, hash);

}
void murmurdemo(int input_bits) {
    printf("murmurdemo **input bits = %d \n", input_bits);
    int bestdist = bias(input_bits, 0,murmurhash32);
    //    display(input_bits,0, murmurhash32);

    printf("bestdist = %d target = %d \n", bestdist,(1 << input_bits) / 2);

}

void murmur64demo(int input_bits) {
    printf("murmur64demo **input bits = %d \n", input_bits);
    int bestdist = bias(input_bits, 0,murmurhash);
    printf("bestdist = %d target = %d \n", bestdist,(1 << input_bits) / 2);
}

int main() {
    for(int input_bits = 2; input_bits<=4; ++input_bits) {
        murmurdemo(input_bits);
        //    murmur64demo(input_bits);
        demo(input_bits);
        printf("\n");

    }
}
