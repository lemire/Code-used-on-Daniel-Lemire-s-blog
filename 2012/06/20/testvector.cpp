//  g++ -O2 -o test test.cpp


#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
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


int runtestnice(size_t N) {
	vector<int> bigarray;
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k);
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}

int runtestnicewreserve(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
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
	return sum;
}






int main() {
	
    WallClockTimer time;
    const size_t N = 100 * 1000 * 1000 ;
    time.reset();
    cout.precision(3);
    cout<<" report speed in millions of integers per second"<<endl;
    cout<<endl<<"ignore this:"<<runtestnice(N)<<endl;
    cout<<"with push_back:"<<N/(1000.0*time.split())<<endl;
    time.reset();
    cout<<endl<<"ignore this:"<<runtestnicewreserve(N)<<endl;
    cout<<"with push_back and reserve:"<<N/(1000.0*time.split())<<endl;
    time.reset();
    cout<<endl<<"ignore this:"<<runtestsafe(N)<<endl;
    cout<<"init first:"<<N/(1000.0*time.split())<<endl;
    time.reset();
    cout<<endl<<"ignore this:"<<runtestunsafe(N)<<endl;
    cout<<"reserve first:"<<N/(1000.0*time.split())<<endl;
    time.reset();
    cout<<endl<<"ignore this:"<<runtestclassic(N)<<endl;
    cout<<"C++ new:"<<N/(1000.0*time.split())<<endl;
    int * bigarray = new int[N];
    time.reset();
    cout<<endl<<"ignore this:"<<runtestnoalloc(N,bigarray)<<endl;
    cout<<"without alloc:"<<N/(1000.0*time.split())<<endl;

    return 0;
}
