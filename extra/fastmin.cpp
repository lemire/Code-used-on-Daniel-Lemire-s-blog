/**
g++ -O3 -o fastmin fastmin.cpp && ./fastmin
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <x86intrin.h>
//#include <adxintrin.h>
using namespace std;


uint32_t computeMin(uint32_t * data, size_t length) {
    uint32_t val = 0xFFFFFFFF;
    for(size_t k = 0; k < length ; ++k) {
        if(val > data[k]) val = data[k]; 
    }
    return val;
}

uint32_t computeMinPlusMax(uint32_t * data, size_t length) {
    uint32_t val = 0xFFFFFFFF;
    uint32_t mval = 0;
    for(size_t k = 0; k < length ; ++k) {
        if(val > data[k]) val = data[k]; 
        if(mval < data[k]) mval = data[k]; 
    }
    return val+mval;
}


inline int sex(int x)
 {
  union
   {
    // let us suppose long is twice as wide as int
    long w; 
 
    // should be hi,lo on a big endian machine
    struct { int lo, hi; };
   } z = { .w=x };
 
  return z.hi;
 }

// assumes that input is 31-bit.
uint32_t branchlesscomputeMin(uint32_t * data, size_t length) {
    uint32_t val = 0x7fffffff;
    for(size_t k = 0; k < length ; ++k) {
        val = val + ( ( data[k] - val ) & sex ( data[k] - val )); 
    }
    return val;
}

int main() {
	const size_t N = 100*128;
	uint32_t a[N];
	for(size_t i = 0; i < N; ++i) {
	  a[i] = rand() & 0x7fffffff;
	}
	uint32_t s1, s2, s3;
	const clock_t S1 = clock();

	for(int T=0; T<10000;++T) {
	  s1 = computeMin(a,N);
    }
	const clock_t S2 = clock();
	for(int T=0; T<10000;++T) {
	  s2 = branchlesscomputeMin(a,N);
    }        
    const clock_t S3 = clock();
	for(int T=0; T<10000;++T) {
	  s3 = computeMinPlusMax(a,N);
    }        
    const clock_t S4 = clock();
	
    cout<<"normal time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"branchless time="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
    cout<<"max-min time="<<(double)(S4-S3)/ CLOCKS_PER_SEC<<endl;
 
	cout<<s1<<" "<<s2<<" "<<s3<<endl;
}
