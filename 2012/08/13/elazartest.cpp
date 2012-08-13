/**
* Inspired by Elazar Leibovich.
*
*  g++ -O0 -g3 -funroll-loops  -o elazartest elazertest.cpp
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
    double elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1.0) + ((t2.tv_usec - t1. tv_usec) /( 1000.0 * 1000.0)
                );
    }
    double split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

int totalsum(const int * data, const size_t length) {
	int sum = 0;
	for (size_t i=0; i<length;i++) sum+=data[i];
	return sum;
}
template <int offset>
int sum(const int * data, const size_t length) {
	int sum = 0;
	for (size_t i=0; i<length;i+=offset) sum+=data[i];
	return sum;
}

int test(const size_t N) {
	int *  a = new int[N];
	for(size_t k = 0; k< N; ++k)
	  a[k] = k - 2 + k * k;
	int fakecounter = 0;
	cout<<" Buffer size = "<< N*sizeof(int) /(1024.0*1024.0)<<" MB "<<endl;

	WallClockTimer t;
	double besttime1 = numeric_limits<double>::max();
	double besttime2 = numeric_limits<double>::max();
	double besttime3 = numeric_limits<double>::max();
	for(int k = 0; k<20;++k) {
		t.reset();
		fakecounter += totalsum(a,N);
		double thistime1 = t.split();
		if(thistime1 < besttime1) besttime1 = thistime1;
		t.reset();
		fakecounter += sum<2>(a,N);
		double thistime2 = t.split();
		if(thistime2 < besttime2) besttime2 = thistime2;
		t.reset();
		fakecounter += sum<16>(a,N);
		double thistime3 = t.split();
		if(thistime3 < besttime3) besttime3 = thistime3;
	}
    cout<<" total sum speed = "<<N/(1000*1000*besttime1) <<" mis or "<< N*sizeof(int)/(1024.0*1024.0*besttime1)<<" MB/s"<<endl;
    cout<<" partial sum speed = "<<N/(1000*1000*besttime2) <<" mis or "<< N*sizeof(int)/(1024.0*1024.0*besttime2)<<" MB/s"<<endl;
    cout<<" speed ratio = "<< besttime1 /besttime2<<endl;
    cout<<" partial sum speed = "<<N/(1000*1000*besttime3) <<" mis or "<< N*sizeof(int)/(1024.0*1024.0*besttime3)<<" MB/s"<<endl;
    cout<<" speed ratio = "<< besttime1 /besttime3<<endl;
    return fakecounter;
}

int main() {
	int total = 0;
	cout<<" L2 cache"<<endl;
	total += test(20000);
	cout << endl;

	cout<<" L3 cache"<<endl;
	total += test( 400 * 1000);
	cout << endl;
	
	cout<<" RAM cache"<<endl;
	total += test(500 * 1000 * 1000);
	cout << endl;
	return total;
}
