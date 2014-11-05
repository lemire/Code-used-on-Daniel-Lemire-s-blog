//  g++ -O2 -o test test.cpp

#include <stdint.h>
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
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};





template <class T>
void runtest() {
    size_t N = 10 * 1000 * 1000 ;
	int repeat = 20;
    WallClockTimer timer;
    const bool paranoid = false;
    cout<<" processing word of size "<<sizeof(T)<<endl;
	for(unsigned int offset = 0; offset<sizeof(T);++offset) {
	    vector<T> bigarray(N+2);
	    cout<<"offset = "<<offset<<endl;
		T * const begin =   reinterpret_cast<T *> (reinterpret_cast<uintptr_t>(&bigarray[0]) + offset);
		assert(offset + reinterpret_cast<uintptr_t>(&bigarray[0])  == reinterpret_cast<uintptr_t>(begin)  );
		T * const end = begin + N;
		if(paranoid) assert(reinterpret_cast<uintptr_t>(end)<reinterpret_cast<uintptr_t>(&bigarray.back()));
		int sumt = 0;
		cout<<" ignore this: ";
		for(int k = 0 ; k < repeat; ++k) {
		  timer.reset();
		  for(size_t i = 0; i <N;++i) {
	           begin[i] = static_cast<T>( i );
		  }
		  T val = 1;
		  for(size_t i = 0; i <N;++i) {
	           val += begin[i] * val  + 33;
		  }
		  int time = timer.split();
		  sumt += time;
		  cout<<val;
		}
		cout<<endl;
		cout<<" average time for offset "<<(offset%sizeof(T))<<" is "<<sumt * 1.0 /repeat<<endl;
	}

}



int main() {
	runtest<int>();
	cout<<endl;
	runtest<long>();
	cout<<endl;
	runtest<long long>();
	cout<<endl;

	return 0;
}
