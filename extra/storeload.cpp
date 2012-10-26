#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#include <iostream>

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



__attribute__ ((noinline)) 
int storeTest(vector<int> & data, vector<int> & bigdata, size_t N) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		bigdata.insert(bigdata.end(), data.begin(), data.end());
		bogus += bigdata.back();
	}
	return bogus;
}

__attribute__ ((noinline)) 
int loadTest(vector<int> & data, vector<int> & bigdata, size_t N) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		data.assign(bigdata.begin() + x * data.size(), bigdata.begin() + (x+1) * data.size());
		bogus += data.back();
	}
	return bogus;
}


int main() {
	WallClockTimer timer;
	vector<int> data;
	int bogus;
	size_t M = 2048 * 4;
	size_t N = 2048 * 8;
	for(size_t i = 0; i<M; ++i)
	  data.push_back(i);
	vector<int> bigdata;
	bigdata.reserve(M * N);
	for(size_t t = 0; t< 3; ++t) {
		timer.reset();
		bogus += storeTest(data,bigdata,N);
		if(t>0) cout<<" store time = "<<timer.split()<<endl;
		timer.reset();
    	bogus += loadTest(data,bigdata,N);
		if(t>0) cout<<" load time = "<<timer.split()<<endl;
		bigdata.clear();
	}
    return bogus;
}