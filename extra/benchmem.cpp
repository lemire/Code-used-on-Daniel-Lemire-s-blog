/**
* reports transfert rate.
*
*  g++ -O0 -g3  -o benchmem benchmem.cpp
*/
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <limits>

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

int main() {
	const size_t N = 50 * 1000 * 1000;
	int *  a = new int[N];
	int *  b = new int[N];

	WallClockTimer t;
	int total = 0;
	double besttime1 = numeric_limits<double>::max();
	double besttime2 = numeric_limits<double>::max();
	for(int k = 0; k<10;++k) {
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
        cout<<" memset speed = "<<N/(1000.0*1000.0*besttime1) <<" mis or "<< N*4/(1024.0*1024.0*besttime1)<<" MB/s"<<endl;
        cout<<" memcpy speed = "<<N/(1000.0*1000.0*besttime2) <<" mis or "<< N*4/(1024.0*1024.0*besttime2)<<" MB/s"<<endl;
        return total;
}
