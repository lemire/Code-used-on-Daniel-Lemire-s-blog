#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

/**
Stafford constants...

MurmurHash3 mixer	33	0xff51afd7ed558ccd	33	0xc4ceb9fe1a85ec53	33
Mix01	31	0x7fb5d329728ea185	27	0x81dadef4bc2dd44d	33
Mix02	33	0x64dd81482cbd31d7	31	0xe36aa5c613612997	31
Mix03	31	0x99bcf6822b23ca35	30	0x14020a57acced8b7	33
Mix04	33	0x62a9d9ed799705f5	28	0xcb24d0a5c88c35b3	32
Mix05	31	0x79c135c1674b9add	29	0x54c77c86f6913e45	30
Mix06	31	0x69b0bc90bd9a8c49	27	0x3d5e661a2a77868d	30
Mix07	30	0x16a6ac37883af045	26	0xcc9c31a4274686a5	32
Mix08	30	0x294aa62849912f0b	28	0x0a9ba9c8a5b15117	31
Mix09	32	0x4cd6944c5cc20b6d	29	0xfc12c5b19d3259e9	32
Mix10	30	0xe4c7e495f4c683f5	32	0xfda871baea35a293	33
Mix11	27	0x97d461a8b11570d9	28	0x02271eb7c6c4cd6b	32
Mix12	29	0x3cd0eb9d47532dfb	26	0x63660277528772bb	33
Mix13	30	0xbf58476d1ce4e5b9	27	0x94d049bb133111eb	31
Mix14	30	0x4be98134a5976fd3	29	0x3bc0993a5ad19a13	31


*/

uint64_t mix64(uint64_t z) {
    z = (z ^ (z >> 33)) * UINT64_C(0xff51afd7ed558ccd);
    z = (z ^ (z >> 33)) * UINT64_C(0xc4ceb9fe1a85ec53);
    return z ^ (z >> 33);
}


//http://zimbry.blogspot.ca/2011/09/better-bit-mixing-improving-on.html
uint64_t bettermix64(uint64_t z) {
    z = (z ^ (z >> 31)) * UINT64_C(0x7fb5d329728ea185);
    z = (z ^ (z >> 27)) * UINT64_C(0x81dadef4bc2dd44d);
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

int bmurmurhash(int value, uint64_t key) {
    return bettermix64(value)&1;
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
      //  display(input_bits,0, murmurhash32);

    printf("bestdist = %d target = %d \n", bestdist,(1 << input_bits) / 2);

}

void murmur64demo(int input_bits) {
    printf("murmur64demo **input bits = %d \n", input_bits);
    int bestdist = bias(input_bits, 0,murmurhash);
    display(input_bits,0, murmurhash);
    printf("bestdist = %d target = %d \n", bestdist,(1 << input_bits) / 2);
}

void bmurmur64demo(int input_bits) {
    printf("bmurmur64demo **input bits = %d \n", input_bits);
    int bestdist = bias(input_bits, 0,bmurmurhash);
    display(input_bits,0, bmurmurhash);
    printf("bestdist = %d target = %d \n", bestdist,(1 << input_bits) / 2);
}

int main() {
    for(int input_bits = 2; input_bits<=3; ++input_bits) {
        murmurdemo(input_bits);
        murmur64demo(input_bits);
        bmurmur64demo(input_bits);
        demo(input_bits);
        printf("\n");

    }
}
