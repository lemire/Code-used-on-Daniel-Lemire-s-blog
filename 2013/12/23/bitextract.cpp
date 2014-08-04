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
      WallClockTimer() : t1(), t2(){ gettimeofday(&t1,0); t2 = t1; }
      void reset() {gettimeofday(&t1,0); t2 = t1;}
      int elapsed() { return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000); }
      int split() { gettimeofday(&t2,0); return elapsed(); }
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
            for(int offset = 0; offset<64;++offset) {
                    if((myword >> offset) == 0) break;
                    offset+=__builtin_ctzl((myword >> offset));
                    out[pos++]=64 * k + offset;
                }
        }
        return pos;
    }
   
 unsigned char hugearray[256][10] ={{0,8},
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
{8,0,1,2,3,4,5,6,7,0}}; 
  
  
   int bitscan4(long *bitmap, int bitmapsize, int *out) {
        int *initout = out;
        unsigned char * b8 = (unsigned char *) bitmap;
        for (int k = 0; k < bitmapsize*sizeof(long); ++k) {
            unsigned char* codes = hugearray[b8[k]];
            for(int offset = 0; offset<codes[0];++offset){
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
        for (int k = 0; k < bitmapsize*sizeof(long); k+=8) {
            out += hugearray[b8[k]][0];
            out += hugearray[b8[k+1]][0];
            out += hugearray[b8[k+2]][0];
            out += hugearray[b8[k+3]][0];
            out += hugearray[b8[k+4]][0];
            out += hugearray[b8[k+5]][0];
            out += hugearray[b8[k+6]][0];
            out += hugearray[b8[k+7]][0];
        }
        return out-initout;
    }
  int main() {
  	  assert(sizeof(long)==8);
  	  assert(sizeof(int)==4);
  	  WallClockTimer timer;
  	  int N = 100000;
  	  for(int sb = 1; sb<=32;sb*=2) {
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
		  for(int t1=0;t1<100;++t1)
		    c0 = bitscan0(&bitmap[0],N,&output[0]);
		  int ti0 = timer.split();
		  timer.reset();
		  int c1 = 0;
		  for(int t1=0;t1<100;++t1)
		    c1 = bitscan1(&bitmap[0],N,&output[0]);
		  assert(c1 == c0);
		  int ti1 = timer.split();
		  timer.reset();
		  int c2 = 0;
		  for(int t1=0;t1<100;++t1)
		    c2 = bitscan2(&bitmap[0],N,&output[0]);
		  int ti2 = timer.split();
		  timer.reset();
		  assert(c1 == c2);
		  int c3 = 0;
		  for(int t1=0;t1<100;++t1)
		    c3 = bitscan3(&bitmap[0],N,&output[0]);
		  int ti3 = timer.split();
		  assert(c1 == c3);

		  timer.reset();
		  int c4 = 0;
		  for(int t1=0;t1<100;++t1)
		    c4 = bitscan4(&bitmap[0],N,&output[0]);
		  int ti4 = timer.split();
		  assert(c4 == c3);
		  timer.reset();
		  int c5 = 0;
		  for(int t1=0;t1<100;++t1)
		    c5 = bitscan4(&bitmap[0],N,&output[0]);
		  int ti5 = timer.split();
		  assert(c5 == c3);

		  if(t>2)
		    cout<<sb<<" " <<bitcount*100.0*0.001 /ti0<<" "<<" " <<bitcount*100.0*0.001 /ti1<<" " <<bitcount*100.0*0.001 /ti2<<" " <<bitcount*100.0*0.001 /ti3<<" " <<bitcount*100.0*0.001 /ti4<<" " <<bitcount*100.0*0.001 /ti5<<endl;
		}
  	  }

  	return 0;
  }
