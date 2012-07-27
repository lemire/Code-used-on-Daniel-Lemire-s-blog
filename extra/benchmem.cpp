/**
* reports transfert rate.
*
*  g++ -O0 -g3  -o benchmem benchmem.cpp
*/
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>

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
	double bestspeed1 = 0;
	double bestspeed2 = 0;
	for(int k = 0; k<10;++k) {
		t.reset();
		memset(a,k,N*sizeof(int));
		double thisspeed1 = N/(t.split()*1000.0);
		if(thisspeed1 > bestspeed1) bestspeed1 = thisspeed1;
		t.reset();
		memcpy(b,a,N*sizeof(int));
		double thisspeed2 = N/(t.split()*1000.0);
		if(thisspeed2 > bestspeed2) bestspeed2 = thisspeed2;
		total += b[N-2];
	}
	cout<<bestspeed1<<" "<<bestspeed2 <<endl;
	return total;
}