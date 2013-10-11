/**
* reports transfert rate.
*
*  g++ -O0 -mavx -g3  -o benchmem benchmem.cpp
*/
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <limits>
#include <immintrin.h>
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

void fastcopy(const int * in , int * out, size_t N) {// assumes N is a multiple of 4
	const __m128i * min = reinterpret_cast<const __m128i *>(in); 
	__m128i * mout = reinterpret_cast<__m128i *>(out);
	
	for(size_t k = 0; k < N/4; ++k) {
		      _mm_store_si128(mout++,  _mm_load_si128(min++));
	}
}

int run(const size_t N) {
	cout<<"N = "<<N<<endl;
	int *  a = new int[N];
	int *  b = new int[N];

	WallClockTimer t;
	int total = 0;
	double besttime0 = numeric_limits<double>::max();
	double besttime1 = numeric_limits<double>::max();
	double besttime2 = numeric_limits<double>::max();
	for(int k = 0; k<10;++k) {
		t.reset();
		fastcopy(b,a,N);
		double thistime0 = t.split()/1000.0;
		if(thistime0 < besttime0) besttime0 = thistime0;
		t.reset();
		memset(a,k,N*sizeof(int));
		double thistime1 = t.split()/1000.0;
		if(thistime1 < besttime1) besttime1 = thistime1;
		t.reset();
		memcpy(b,a,N*sizeof(int));
		double thistime2 = t.split()/1000.0;
		if(thistime2 < besttime2) besttime2 = thistime2;
		total += b[N-2];
	}
        cout<<" SIMD memcpy speed = "<<N/(1000.0*1000.0*besttime0) <<" mis or "<< N*4/(1024.0*1024.0*besttime0)<<" MB/s"<<endl;

        cout<<" memset speed = "<<N/(1000.0*1000.0*besttime1) <<" mis or "<< N*4/(1024.0*1024.0*besttime1)<<" MB/s"<<endl;
        cout<<" memcpy speed = "<<N/(1000.0*1000.0*besttime2) <<" mis or "<< N*4/(1024.0*1024.0*besttime2)<<" MB/s"<<endl;
        return total;

}

int main() {
    run(4000000);
	run(20000000);
	run(200000000);
	return 0;
}
