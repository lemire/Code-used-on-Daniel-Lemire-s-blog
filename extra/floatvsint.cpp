/**
* Testing the speed of floats vs. the speed of ints.
* D. Lemire, Sept. 2012
*

$ g++  -funroll-loops -O3 -o floatvsint floatvsint.cpp

$ ./unrolldeltas 
*/


// This may require GCC 4.4/


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
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

template <class C>
void  givemeanarray(C & bigarray, size_t N) {
	bigarray.reserve(N);
	for(unsigned int k = 0; k < N; ++k)
	  bigarray.push_back(k+k/3);
}


template <class T>
void  multiply(T* begin, T* end, T slope, T b0) {
	for(T * i = begin; i != end; ++i) {
		*i = slope * (*i) + b0; 
	}
}


template <class T>
T test(size_t N) {
	vector<T> array;
	givemeanarray(array, N) ;
	WallClockTimer time;
	time.reset();
	multiply(&array[0], (&array[array.size()-1])+1, static_cast<T>(3),static_cast<T>(7));
	cout<<"speed "<<N/(1000.0*time.split())<<endl;   
	return array.back();

}

int main() {
	size_t N = 50 * 1000 * 1000 ;
	int b1 = test<int>(N);

	float b2 = test<float>(N);

	cout<<b1<<b2<<endl;
}


