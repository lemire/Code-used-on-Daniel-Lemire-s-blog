
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <climits>
#include <limits>
#include <vector>

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


int test(size_t N) {
	vector<int> x;
	for(size_t i = 0 ; i < N ; ++i ) {
		x.push_back(i);
	}
	return x.back();
}


int testManual(size_t N, const int multiplier, const int div = 1, const int constant = 0) {
	int * x = new int[1];
	size_t s = 1;
	for(size_t i = 0 ; i < N ; ++i ) {
		if(i == s) {
			int * nx = new int[ multiplier * s / div + constant ];
			memcpy(nx,x,s*sizeof(int));
			delete[] x;
			x = nx;
			s = multiplier * s / div + constant;
                        if( s <= i) cerr << "bug" << endl;
		}
		x[i] = i;
	}
	return x[N-1];
}



int overall(size_t N) {
	int bogus = 0;
	WallClockTimer t;
	t.reset();
    bogus += test(N);
    int delay = t.split();
    cout << "STL vector " << N /(delay * 1000.0) << endl;
    
    for(size_t factor = 1; factor <= 6; ++ factor) {
        delay = INT_MAX;
        for(size_t T = 0 ; T < 10 ; ++T ) {
          t.reset();
    	  bogus += testManual(N,2+factor,2,factor&1==0 ? 0 : 1);
    	  int tdelay = t.split();
          if(tdelay < delay) delay = tdelay;
        }
    	cout << "pointer-based "<< (factor +2)/2.0<< " : " << N /(delay * 1000.0) << endl;
    }
    return bogus;
}

int main() {
	int bogus = overall(1024*1024);
	bogus += overall(1024*1024);
	bogus += overall(1024*1024*64);
	bogus += overall(1024*1024*64);

	return bogus;
}
