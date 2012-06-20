//g++-fsf-4.7  -std=c++11 -O3 -o integerlog integerlog.cpp

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <cstdint>
using namespace std;

class WallClockTimer {
public:
    struct timeval t1, t2;
    WallClockTimer() :
        t1(), t2() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    int elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000) + ((t2.tv_usec - t1. tv_usec)
                / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};


int runtestnice(size_t N) {
	vector<int> bigarray;
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k);
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}


int runtestsafe(size_t N) {
	vector<int> bigarray(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray[k] = k;
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}

int runtestunsafe(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray[k] = k;// unsafe
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}
int runtestclassic(size_t N) {
	int * bigarray = new int[N];
	for(unsigned int k = 0; k<N; ++k)
	  bigarray[k] = k;
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	delete [] bigarray;
	return sum;
}
int runtestnoalloc(size_t N, int * bigarray) {
	for(unsigned int k = 0; k<N; ++k)
	  bigarray[k] = k;// unsafe
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	delete [] bigarray;
	return sum;
}





uint32_t gccbits(const uint32_t v) {
    return v == 0 ? 0 : 32 - __builtin_clz(v);
}



__attribute__ ((const))
uint32_t asmbits(const uint32_t v) {
    if (v == 0)
        return 0;
    uint32_t answer;
    __asm__("bsr %1, %0;" :"=r"(answer) :"r"(v));
    return answer + 1;
}


__attribute__ ((const))
uint32_t slowbits(uint32_t v) {
    uint32_t r = 0;
    while (v) {
        r++;
        v = v >> 1;
    }
    return r;
}

__attribute__ ((const))
uint32_t bits(uint32_t v) {
    uint32_t r(0);
    if (v >= (1U << 31)) {
        return 32;
    }
    if (v >= (1U << 15)) {
        v >>= 16;
        r += 16;
    }
    if (v >= (1U << 7)) {
        v >>= 8;
        r += 8;
    }
    if (v >= (1U << 3)) {
        v >>= 4;
        r += 4;
    }
    if (v >= (1U << 1)) {
        v >>= 2;
        r += 2;
    }
    if (v >= (1U << 0)) {
        v >>= 1;
        r += 1;
    }
    return r;
}


int main() {
	
    WallClockTimer time;
    size_t N = 50 * 1000 * 1000 ;
    vector<uint32_t> bigarray(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray[k] = k;
    bigarray[0]=1U<<31;
    
    int sum = 0;
    time.reset();
    for(unsigned int k = 0; k<N; ++k)
	  sum+= bigarray[k];
    cout<<"nothing speed "<<N/(1000.0*time.split())<<endl;
    
    
    time.reset();
    for(unsigned int k = 0; k<N; ++k)
	  sum+= gccbits(bigarray[k]);
    cout<<"gcc speed "<<N/(1000.0*time.split())<<endl;
    for(unsigned int k = 0; k<N; ++k)
	  assert(gccbits(bigarray[k]) == bits(bigarray[k]));

    time.reset();
    for(unsigned int k = 0; k<N; ++k)
	  sum+= bits(bigarray[k]);
    cout<<"bits speed "<<N/(1000.0*time.split())<<endl;


    time.reset();
    for(unsigned int k = 0; k<N; ++k)
	  sum+= slowbits(bigarray[k]);
    cout<<"slowbits speed "<<N/(1000.0*time.split())<<endl;
for(unsigned int k = 0; k<N; ++k)
	  assert(slowbits(bigarray[k]) == bits(bigarray[k]));

    time.reset();
    for(unsigned int k = 0; k<N; ++k)
	  sum+= asmbits(bigarray[k]);
    cout<<"asmbits speed "<<N/(1000.0*time.split())<<endl;
    for(unsigned int k = 0; k<N; ++k)
	  assert(asmbits(bigarray[k]) == bits(bigarray[k]));



 	  
    return sum;
}
