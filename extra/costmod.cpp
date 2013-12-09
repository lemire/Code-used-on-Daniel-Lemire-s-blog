// Daniel Lemire, Nov 18th 2013
/***
$ g++ -O3  -o costmod costmod.cpp
**/
#include <cmath>
#include <vector>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include <cassert>
#include <iostream>
#include <string>
using namespace std;



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




int sixtyfour(long * keys, int * val, int N) {
	long s = 0;
	for(int k = 0; k < N; ++k) {
		s += val[k] * keys[k];
	}
	return (int) (s>>32);
} 


int thirtytwo(int * keys, int * val, int N) {
	int s = 0;
	for(int k = 0; k < N; ++k) {
		s += val[k] * keys[k];
	}
	return s;
} 


int thirtytwomod(int * keys, int * val, int N) {
	int s = 0;
	for(int k = 0; k < N; ++k) {
		s = (s + val[k] * keys[k]) % (- 5);
	}
	return s;
} 



int main() {
	size_t N = 1024*1024*4;
	vector<long> keys(N,4);
	vector<int> vals(N);
	WallClockTimer timer;
	float bogus = 0;
	timer.reset();
	for(int k = 0; k<10;++k)
	bogus += sixtyfour(&keys[0], &vals[0],N);
	int time1 = timer.split();
	timer.reset();
    for(int k = 0; k<10;++k)
	bogus += thirtytwo((int*)&keys[0], &vals[0],N);
    int time2 = timer.split();
	timer.reset();
    for(int k = 0; k<10;++k)
	bogus += thirtytwomod((int*)&keys[0], &vals[0],N);
	int time3 = timer.split();

    cout<<time1<<" "<<time2<<" "<<time3<<endl;
    cout<<bogus<<endl;
}
