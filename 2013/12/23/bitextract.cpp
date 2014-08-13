// g++ -msse4.2 -O3 -o bitextract bitextract.cpp

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#include <iostream>
#include <cassert>

class WallClockTimer
{
public:
    struct timeval t1, t2;
public:
    WallClockTimer() : t1(), t2() {
        gettimeofday(&t1,0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1,0);
        t2 = t1;
    }
    int elapsed() {
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2,0);
        return elapsed();
    }
};

using namespace std;

int bitscan0(long *bitmap, int bitmapsize, int *out) {
    int counter = 0;
    for(int k = 0; k<bitmapsize; ++k) {
        for(int c = 0; c<64; ++c) {
            if (((1l << c) & bitmap[k]) != 0) {
                out[counter++] = k*64+c;
            }
        }
    }
    return counter;
}




// inspired by http://www.steike.com/code/bits/debruijn/
int bitscan1(long *bitmap, int bitmapsize, int *out) {
    int pos = 0;
    for(int k = 0; k < bitmapsize; ++k) {
        long bitset = bitmap[k];
        while (bitset != 0) {
            long t = bitset & -bitset;
            out[pos++] = k * 64 + _mm_popcnt_u32 (t-1);// __builtin_popcountl (t-1);
            bitset ^= t;
        }
    }
    return pos;
}


int bitscan2(long *bitmap, int bitmapsize, int *out) {
    int pos = 0;
    for(int k = 0; k < bitmapsize; ++k) {
        long data = bitmap[k];
        while (data != 0) {
            int ntz =  __builtin_ctzl (data);
            out[pos++] = k * 64 + ntz;
            data ^= (1l << ntz);
        }
    }
    return pos;
}

int bitscan3(long *bitmap, int bitmapsize, int *out) {
    int pos = 0;
    for (int k = 0; k < bitmapsize; ++k) {
        const long myword = bitmap[k];
        for(int offset = 0; offset<64; ++offset) {
            if((myword >> offset) == 0) break;
            offset+=__builtin_ctzl((myword >> offset));
            out[pos++]=64 * k + offset;
        }
    }
    return pos;
}

unsigned char hugearray[256][10] = {{0,8},
    {1,0,7},
    {1,1,6},
    {2,0,1,6},
    {1,2,5},
    {2,0,2,5},
    {2,1,2,5},
    {3,0,1,2,5},
    {1,3,4},
    {2,0,3,4},
    {2,1,3,4},
    {3,0,1,3,4},
    {2,2,3,4},
    {3,0,2,3,4},
    {3,1,2,3,4},
    {4,0,1,2,3,4},
    {1,4,3},
    {2,0,4,3},
    {2,1,4,3},
    {3,0,1,4,3},
    {2,2,4,3},
    {3,0,2,4,3},
    {3,1,2,4,3},
    {4,0,1,2,4,3},
    {2,3,4,3},
    {3,0,3,4,3},
    {3,1,3,4,3},
    {4,0,1,3,4,3},
    {3,2,3,4,3},
    {4,0,2,3,4,3},
    {4,1,2,3,4,3},
    {5,0,1,2,3,4,3},
    {1,5,2},
    {2,0,5,2},
    {2,1,5,2},
    {3,0,1,5,2},
    {2,2,5,2},
    {3,0,2,5,2},
    {3,1,2,5,2},
    {4,0,1,2,5,2},
    {2,3,5,2},
    {3,0,3,5,2},
    {3,1,3,5,2},
    {4,0,1,3,5,2},
    {3,2,3,5,2},
    {4,0,2,3,5,2},
    {4,1,2,3,5,2},
    {5,0,1,2,3,5,2},
    {2,4,5,2},
    {3,0,4,5,2},
    {3,1,4,5,2},
    {4,0,1,4,5,2},
    {3,2,4,5,2},
    {4,0,2,4,5,2},
    {4,1,2,4,5,2},
    {5,0,1,2,4,5,2},
    {3,3,4,5,2},
    {4,0,3,4,5,2},
    {4,1,3,4,5,2},
    {5,0,1,3,4,5,2},
    {4,2,3,4,5,2},
    {5,0,2,3,4,5,2},
    {5,1,2,3,4,5,2},
    {6,0,1,2,3,4,5,2},
    {1,6,1},
    {2,0,6,1},
    {2,1,6,1},
    {3,0,1,6,1},
    {2,2,6,1},
    {3,0,2,6,1},
    {3,1,2,6,1},
    {4,0,1,2,6,1},
    {2,3,6,1},
    {3,0,3,6,1},
    {3,1,3,6,1},
    {4,0,1,3,6,1},
    {3,2,3,6,1},
    {4,0,2,3,6,1},
    {4,1,2,3,6,1},
    {5,0,1,2,3,6,1},
    {2,4,6,1},
    {3,0,4,6,1},
    {3,1,4,6,1},
    {4,0,1,4,6,1},
    {3,2,4,6,1},
    {4,0,2,4,6,1},
    {4,1,2,4,6,1},
    {5,0,1,2,4,6,1},
    {3,3,4,6,1},
    {4,0,3,4,6,1},
    {4,1,3,4,6,1},
    {5,0,1,3,4,6,1},
    {4,2,3,4,6,1},
    {5,0,2,3,4,6,1},
    {5,1,2,3,4,6,1},
    {6,0,1,2,3,4,6,1},
    {2,5,6,1},
    {3,0,5,6,1},
    {3,1,5,6,1},
    {4,0,1,5,6,1},
    {3,2,5,6,1},
    {4,0,2,5,6,1},
    {4,1,2,5,6,1},
    {5,0,1,2,5,6,1},
    {3,3,5,6,1},
    {4,0,3,5,6,1},
    {4,1,3,5,6,1},
    {5,0,1,3,5,6,1},
    {4,2,3,5,6,1},
    {5,0,2,3,5,6,1},
    {5,1,2,3,5,6,1},
    {6,0,1,2,3,5,6,1},
    {3,4,5,6,1},
    {4,0,4,5,6,1},
    {4,1,4,5,6,1},
    {5,0,1,4,5,6,1},
    {4,2,4,5,6,1},
    {5,0,2,4,5,6,1},
    {5,1,2,4,5,6,1},
    {6,0,1,2,4,5,6,1},
    {4,3,4,5,6,1},
    {5,0,3,4,5,6,1},
    {5,1,3,4,5,6,1},
    {6,0,1,3,4,5,6,1},
    {5,2,3,4,5,6,1},
    {6,0,2,3,4,5,6,1},
    {6,1,2,3,4,5,6,1},
    {7,0,1,2,3,4,5,6,1},
    {1,7,0},
    {2,0,7,0},
    {2,1,7,0},
    {3,0,1,7,0},
    {2,2,7,0},
    {3,0,2,7,0},
    {3,1,2,7,0},
    {4,0,1,2,7,0},
    {2,3,7,0},
    {3,0,3,7,0},
    {3,1,3,7,0},
    {4,0,1,3,7,0},
    {3,2,3,7,0},
    {4,0,2,3,7,0},
    {4,1,2,3,7,0},
    {5,0,1,2,3,7,0},
    {2,4,7,0},
    {3,0,4,7,0},
    {3,1,4,7,0},
    {4,0,1,4,7,0},
    {3,2,4,7,0},
    {4,0,2,4,7,0},
    {4,1,2,4,7,0},
    {5,0,1,2,4,7,0},
    {3,3,4,7,0},
    {4,0,3,4,7,0},
    {4,1,3,4,7,0},
    {5,0,1,3,4,7,0},
    {4,2,3,4,7,0},
    {5,0,2,3,4,7,0},
    {5,1,2,3,4,7,0},
    {6,0,1,2,3,4,7,0},
    {2,5,7,0},
    {3,0,5,7,0},
    {3,1,5,7,0},
    {4,0,1,5,7,0},
    {3,2,5,7,0},
    {4,0,2,5,7,0},
    {4,1,2,5,7,0},
    {5,0,1,2,5,7,0},
    {3,3,5,7,0},
    {4,0,3,5,7,0},
    {4,1,3,5,7,0},
    {5,0,1,3,5,7,0},
    {4,2,3,5,7,0},
    {5,0,2,3,5,7,0},
    {5,1,2,3,5,7,0},
    {6,0,1,2,3,5,7,0},
    {3,4,5,7,0},
    {4,0,4,5,7,0},
    {4,1,4,5,7,0},
    {5,0,1,4,5,7,0},
    {4,2,4,5,7,0},
    {5,0,2,4,5,7,0},
    {5,1,2,4,5,7,0},
    {6,0,1,2,4,5,7,0},
    {4,3,4,5,7,0},
    {5,0,3,4,5,7,0},
    {5,1,3,4,5,7,0},
    {6,0,1,3,4,5,7,0},
    {5,2,3,4,5,7,0},
    {6,0,2,3,4,5,7,0},
    {6,1,2,3,4,5,7,0},
    {7,0,1,2,3,4,5,7,0},
    {2,6,7,0},
    {3,0,6,7,0},
    {3,1,6,7,0},
    {4,0,1,6,7,0},
    {3,2,6,7,0},
    {4,0,2,6,7,0},
    {4,1,2,6,7,0},
    {5,0,1,2,6,7,0},
    {3,3,6,7,0},
    {4,0,3,6,7,0},
    {4,1,3,6,7,0},
    {5,0,1,3,6,7,0},
    {4,2,3,6,7,0},
    {5,0,2,3,6,7,0},
    {5,1,2,3,6,7,0},
    {6,0,1,2,3,6,7,0},
    {3,4,6,7,0},
    {4,0,4,6,7,0},
    {4,1,4,6,7,0},
    {5,0,1,4,6,7,0},
    {4,2,4,6,7,0},
    {5,0,2,4,6,7,0},
    {5,1,2,4,6,7,0},
    {6,0,1,2,4,6,7,0},
    {4,3,4,6,7,0},
    {5,0,3,4,6,7,0},
    {5,1,3,4,6,7,0},
    {6,0,1,3,4,6,7,0},
    {5,2,3,4,6,7,0},
    {6,0,2,3,4,6,7,0},
    {6,1,2,3,4,6,7,0},
    {7,0,1,2,3,4,6,7,0},
    {3,5,6,7,0},
    {4,0,5,6,7,0},
    {4,1,5,6,7,0},
    {5,0,1,5,6,7,0},
    {4,2,5,6,7,0},
    {5,0,2,5,6,7,0},
    {5,1,2,5,6,7,0},
    {6,0,1,2,5,6,7,0},
    {4,3,5,6,7,0},
    {5,0,3,5,6,7,0},
    {5,1,3,5,6,7,0},
    {6,0,1,3,5,6,7,0},
    {5,2,3,5,6,7,0},
    {6,0,2,3,5,6,7,0},
    {6,1,2,3,5,6,7,0},
    {7,0,1,2,3,5,6,7,0},
    {4,4,5,6,7,0},
    {5,0,4,5,6,7,0},
    {5,1,4,5,6,7,0},
    {6,0,1,4,5,6,7,0},
    {5,2,4,5,6,7,0},
    {6,0,2,4,5,6,7,0},
    {6,1,2,4,5,6,7,0},
    {7,0,1,2,4,5,6,7,0},
    {5,3,4,5,6,7,0},
    {6,0,3,4,5,6,7,0},
    {6,1,3,4,5,6,7,0},
    {7,0,1,3,4,5,6,7,0},
    {6,2,3,4,5,6,7,0},
    {7,0,2,3,4,5,6,7,0},
    {7,1,2,3,4,5,6,7,0},
    {8,0,1,2,3,4,5,6,7,0}
};


int bitscan4(long *bitmap, int bitmapsize, int *out) {
    int *initout = out;
    unsigned char * b8 = (unsigned char *) bitmap;
    for (int k = 0; k < bitmapsize*sizeof(long); ++k) {
        unsigned char* codes = hugearray[b8[k]];
        for(int offset = 0; offset<codes[0]; ++offset) {
            out[offset]=8 * k + codes[offset +1];
        }
        out += codes[0];
    }
    return out-initout;
}

// just counts
int bitscan5(long *bitmap, int bitmapsize, int *out) {
    int *initout = out;
    unsigned char * b8 = (unsigned char *) bitmap;
    for (int k = 0; k < bitmapsize*sizeof(long); ++k) {
        int off = k*8;
        switch (b8[k]) {
        case 0:
            out+=0;
            break;
        case 1:
            out[0]=off+0;
            out+=1;
            break;
        case 2:
            out[0]=off+1;
            out+=1;
            break;
        case 3:
            out[0]=off+0;
            out[1]=off+1;
            out+=2;
            break;
        case 4:
            out[0]=off+2;
            out+=1;
            break;
        case 5:
            out[0]=off+0;
            out[1]=off+2;
            out+=2;
            break;
        case 6:
            out[0]=off+1;
            out[1]=off+2;
            out+=2;
            break;
        case 7:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out+=3;
            break;
        case 8:
            out[0]=off+3;
            out+=1;
            break;
        case 9:
            out[0]=off+0;
            out[1]=off+3;
            out+=2;
            break;
        case 10:
            out[0]=off+1;
            out[1]=off+3;
            out+=2;
            break;
        case 11:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out+=3;
            break;
        case 12:
            out[0]=off+2;
            out[1]=off+3;
            out+=2;
            break;
        case 13:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out+=3;
            break;
        case 14:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out+=3;
            break;
        case 15:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out+=4;
            break;
        case 16:
            out[0]=off+4;
            out+=1;
            break;
        case 17:
            out[0]=off+0;
            out[1]=off+4;
            out+=2;
            break;
        case 18:
            out[0]=off+1;
            out[1]=off+4;
            out+=2;
            break;
        case 19:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out+=3;
            break;
        case 20:
            out[0]=off+2;
            out[1]=off+4;
            out+=2;
            break;
        case 21:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out+=3;
            break;
        case 22:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out+=3;
            break;
        case 23:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out+=4;
            break;
        case 24:
            out[0]=off+3;
            out[1]=off+4;
            out+=2;
            break;
        case 25:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out+=3;
            break;
        case 26:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out+=3;
            break;
        case 27:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out+=4;
            break;
        case 28:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out+=3;
            break;
        case 29:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out+=4;
            break;
        case 30:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out+=4;
            break;
        case 31:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out+=5;
            break;
        case 32:
            out[0]=off+5;
            out+=1;
            break;
        case 33:
            out[0]=off+0;
            out[1]=off+5;
            out+=2;
            break;
        case 34:
            out[0]=off+1;
            out[1]=off+5;
            out+=2;
            break;
        case 35:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+5;
            out+=3;
            break;
        case 36:
            out[0]=off+2;
            out[1]=off+5;
            out+=2;
            break;
        case 37:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+5;
            out+=3;
            break;
        case 38:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+5;
            out+=3;
            break;
        case 39:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+5;
            out+=4;
            break;
        case 40:
            out[0]=off+3;
            out[1]=off+5;
            out+=2;
            break;
        case 41:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+5;
            out+=3;
            break;
        case 42:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+5;
            out+=3;
            break;
        case 43:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+5;
            out+=4;
            break;
        case 44:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+5;
            out+=3;
            break;
        case 45:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out+=4;
            break;
        case 46:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out+=4;
            break;
        case 47:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+5;
            out+=5;
            break;
        case 48:
            out[0]=off+4;
            out[1]=off+5;
            out+=2;
            break;
        case 49:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+5;
            out+=3;
            break;
        case 50:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+5;
            out+=3;
            break;
        case 51:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 52:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+5;
            out+=3;
            break;
        case 53:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 54:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 55:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+5;
            out+=5;
            break;
        case 56:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+5;
            out+=3;
            break;
        case 57:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 58:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 59:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out+=5;
            break;
        case 60:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out+=4;
            break;
        case 61:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out+=5;
            break;
        case 62:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out+=5;
            break;
        case 63:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+5;
            out+=6;
            break;
        case 64:
            out[0]=off+6;
            out+=1;
            break;
        case 65:
            out[0]=off+0;
            out[1]=off+6;
            out+=2;
            break;
        case 66:
            out[0]=off+1;
            out[1]=off+6;
            out+=2;
            break;
        case 67:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+6;
            out+=3;
            break;
        case 68:
            out[0]=off+2;
            out[1]=off+6;
            out+=2;
            break;
        case 69:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+6;
            out+=3;
            break;
        case 70:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+6;
            out+=3;
            break;
        case 71:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+6;
            out+=4;
            break;
        case 72:
            out[0]=off+3;
            out[1]=off+6;
            out+=2;
            break;
        case 73:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+6;
            out+=3;
            break;
        case 74:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+6;
            out+=3;
            break;
        case 75:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+6;
            out+=4;
            break;
        case 76:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+6;
            out+=3;
            break;
        case 77:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+6;
            out+=4;
            break;
        case 78:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+6;
            out+=4;
            break;
        case 79:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+6;
            out+=5;
            break;
        case 80:
            out[0]=off+4;
            out[1]=off+6;
            out+=2;
            break;
        case 81:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+6;
            out+=3;
            break;
        case 82:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+6;
            out+=3;
            break;
        case 83:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 84:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+6;
            out+=3;
            break;
        case 85:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 86:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 87:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+6;
            out+=5;
            break;
        case 88:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+6;
            out+=3;
            break;
        case 89:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 90:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 91:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out+=5;
            break;
        case 92:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out+=4;
            break;
        case 93:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out+=5;
            break;
        case 94:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out+=5;
            break;
        case 95:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+6;
            out+=6;
            break;
        case 96:
            out[0]=off+5;
            out[1]=off+6;
            out+=2;
            break;
        case 97:
            out[0]=off+0;
            out[1]=off+5;
            out[2]=off+6;
            out+=3;
            break;
        case 98:
            out[0]=off+1;
            out[1]=off+5;
            out[2]=off+6;
            out+=3;
            break;
        case 99:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 100:
            out[0]=off+2;
            out[1]=off+5;
            out[2]=off+6;
            out+=3;
            break;
        case 101:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 102:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 103:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 104:
            out[0]=off+3;
            out[1]=off+5;
            out[2]=off+6;
            out+=3;
            break;
        case 105:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 106:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 107:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 108:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 109:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 110:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 111:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+5;
            out[5]=off+6;
            out+=6;
            break;
        case 112:
            out[0]=off+4;
            out[1]=off+5;
            out[2]=off+6;
            out+=3;
            break;
        case 113:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 114:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 115:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 116:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 117:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 118:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 119:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out+=6;
            break;
        case 120:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out+=4;
            break;
        case 121:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 122:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 123:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out+=6;
            break;
        case 124:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out+=5;
            break;
        case 125:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out+=6;
            break;
        case 126:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out+=6;
            break;
        case 127:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+5;
            out[6]=off+6;
            out+=7;
            break;
        case 128:
            out[0]=off+7;
            out+=1;
            break;
        case 129:
            out[0]=off+0;
            out[1]=off+7;
            out+=2;
            break;
        case 130:
            out[0]=off+1;
            out[1]=off+7;
            out+=2;
            break;
        case 131:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+7;
            out+=3;
            break;
        case 132:
            out[0]=off+2;
            out[1]=off+7;
            out+=2;
            break;
        case 133:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+7;
            out+=3;
            break;
        case 134:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+7;
            out+=3;
            break;
        case 135:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+7;
            out+=4;
            break;
        case 136:
            out[0]=off+3;
            out[1]=off+7;
            out+=2;
            break;
        case 137:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+7;
            out+=3;
            break;
        case 138:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+7;
            out+=3;
            break;
        case 139:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+7;
            out+=4;
            break;
        case 140:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+7;
            out+=3;
            break;
        case 141:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+7;
            out+=4;
            break;
        case 142:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+7;
            out+=4;
            break;
        case 143:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+7;
            out+=5;
            break;
        case 144:
            out[0]=off+4;
            out[1]=off+7;
            out+=2;
            break;
        case 145:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+7;
            out+=3;
            break;
        case 146:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+7;
            out+=3;
            break;
        case 147:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 148:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+7;
            out+=3;
            break;
        case 149:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 150:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 151:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+7;
            out+=5;
            break;
        case 152:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+7;
            out+=3;
            break;
        case 153:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 154:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 155:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+7;
            out+=5;
            break;
        case 156:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+7;
            out+=4;
            break;
        case 157:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+7;
            out+=5;
            break;
        case 158:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+7;
            out+=5;
            break;
        case 159:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+7;
            out+=6;
            break;
        case 160:
            out[0]=off+5;
            out[1]=off+7;
            out+=2;
            break;
        case 161:
            out[0]=off+0;
            out[1]=off+5;
            out[2]=off+7;
            out+=3;
            break;
        case 162:
            out[0]=off+1;
            out[1]=off+5;
            out[2]=off+7;
            out+=3;
            break;
        case 163:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 164:
            out[0]=off+2;
            out[1]=off+5;
            out[2]=off+7;
            out+=3;
            break;
        case 165:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 166:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 167:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 168:
            out[0]=off+3;
            out[1]=off+5;
            out[2]=off+7;
            out+=3;
            break;
        case 169:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 170:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 171:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 172:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 173:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 174:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 175:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+5;
            out[5]=off+7;
            out+=6;
            break;
        case 176:
            out[0]=off+4;
            out[1]=off+5;
            out[2]=off+7;
            out+=3;
            break;
        case 177:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 178:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 179:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 180:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 181:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 182:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 183:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+7;
            out+=6;
            break;
        case 184:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+7;
            out+=4;
            break;
        case 185:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 186:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 187:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+7;
            out+=6;
            break;
        case 188:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+7;
            out+=5;
            break;
        case 189:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+7;
            out+=6;
            break;
        case 190:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+7;
            out+=6;
            break;
        case 191:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+5;
            out[6]=off+7;
            out+=7;
            break;
        case 192:
            out[0]=off+6;
            out[1]=off+7;
            out+=2;
            break;
        case 193:
            out[0]=off+0;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 194:
            out[0]=off+1;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 195:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 196:
            out[0]=off+2;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 197:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 198:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 199:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 200:
            out[0]=off+3;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 201:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 202:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 203:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 204:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 205:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 206:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 207:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 208:
            out[0]=off+4;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 209:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 210:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 211:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 212:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 213:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 214:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 215:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 216:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 217:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 218:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 219:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 220:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 221:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 222:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 223:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 224:
            out[0]=off+5;
            out[1]=off+6;
            out[2]=off+7;
            out+=3;
            break;
        case 225:
            out[0]=off+0;
            out[1]=off+5;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 226:
            out[0]=off+1;
            out[1]=off+5;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 227:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 228:
            out[0]=off+2;
            out[1]=off+5;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 229:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 230:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 231:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 232:
            out[0]=off+3;
            out[1]=off+5;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 233:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 234:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 235:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 236:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 237:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 238:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 239:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+5;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 240:
            out[0]=off+4;
            out[1]=off+5;
            out[2]=off+6;
            out[3]=off+7;
            out+=4;
            break;
        case 241:
            out[0]=off+0;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 242:
            out[0]=off+1;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 243:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 244:
            out[0]=off+2;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 245:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 246:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 247:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 248:
            out[0]=off+3;
            out[1]=off+4;
            out[2]=off+5;
            out[3]=off+6;
            out[4]=off+7;
            out+=5;
            break;
        case 249:
            out[0]=off+0;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 250:
            out[0]=off+1;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 251:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 252:
            out[0]=off+2;
            out[1]=off+3;
            out[2]=off+4;
            out[3]=off+5;
            out[4]=off+6;
            out[5]=off+7;
            out+=6;
            break;
        case 253:
            out[0]=off+0;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 254:
            out[0]=off+1;
            out[1]=off+2;
            out[2]=off+3;
            out[3]=off+4;
            out[4]=off+5;
            out[5]=off+6;
            out[6]=off+7;
            out+=7;
            break;
        case 255:
            out[0]=off+0;
            out[1]=off+1;
            out[2]=off+2;
            out[3]=off+3;
            out[4]=off+4;
            out[5]=off+5;
            out[6]=off+6;
            out[7]=off+7;
            out+=8;
            break;
        default:
            break;
        }
    }
    return out-initout;
}
int main() {
    assert(sizeof(long)==8);
    assert(sizeof(int)==4);
    WallClockTimer timer;
    int N = 10000;
    for(int sb = 1; sb<=64; sb*=2) {
        int setbitsmax = sb*N;
        vector<long> bitmap(N);
        for (int k = 0; k < setbitsmax; ++k) {
            int bit = rand() % (N*64);
            bitmap[bit/64] |= (1L<<(bit%64));
        }
        int bitcount = 0;
        for(int k = 0; k <N; ++k) {
            bitcount += __builtin_popcountl(bitmap[k]);
        }
        vector<int> output(bitcount);
        for(int t = 0; t<5; ++t) {
            timer.reset();
            int c0 = 0;
            for(int t1=0; t1<100; ++t1)
                c0 = bitscan0(&bitmap[0],N,&output[0]);
            int ti0 = timer.split();
            timer.reset();
            int c1 = 0;
            for(int t1=0; t1<100; ++t1)
                c1 = bitscan1(&bitmap[0],N,&output[0]);
            assert(c1 == c0);
            int ti1 = timer.split();
            timer.reset();
            int c2 = 0;
            for(int t1=0; t1<100; ++t1)
                c2 = bitscan2(&bitmap[0],N,&output[0]);
            int ti2 = timer.split();
            timer.reset();
            assert(c1 == c2);
            int c3 = 0;
            for(int t1=0; t1<100; ++t1)
                c3 = bitscan3(&bitmap[0],N,&output[0]);
            int ti3 = timer.split();
            assert(c1 == c3);

            timer.reset();
            int c4 = 0;
            for(int t1=0; t1<100; ++t1)
                c4 = bitscan4(&bitmap[0],N,&output[0]);
            int ti4 = timer.split();
            assert(c4 == c3);
            timer.reset();
            int c5 = 0;
            for(int t1=0; t1<100; ++t1)
                c5 = bitscan4(&bitmap[0],N,&output[0]);
            int ti5 = timer.split();
            assert(c5 == c3);

            if(t>2)
                cout<<c1*1.0/(N*sizeof(long)*8.0)<<" " <<bitcount*100.0*0.001 /ti0<<" "<<" " <<bitcount*100.0*0.001 /ti1<<" " <<bitcount*100.0*0.001 /ti2<<" " <<bitcount*100.0*0.001 /ti3<<" " <<bitcount*100.0*0.001 /ti4<<" " <<bitcount*100.0*0.001 /ti5<<endl;
        }
    }

    return 0;
}
