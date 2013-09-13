#include <sys/types.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <iomanip>


#include <iostream>
#include <vector>



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
    uint64_t elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1. tv_usec));
    }
    uint64_t split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};




using namespace std;
int main() {
	   size_t S = 1024 * 64;
	   vector<uint8_t> counter8(S);
	   vector<uint32_t> counter32(S);
	   vector<uint64_t> counter64(S);
		
        WallClockTimer t;
        for(size_t init = 1; init < 1024; ++init) 
        for (size_t i=init; i<S*3; i+=3) {
        	counter8[i%S] += 1;
        }
        int dt1 = t.split();
        t.reset();
        for(size_t init = 1; init < 1024; ++init) 
        for (size_t i=init; i<S*3; i+=3) {
        	counter32[i%S] += 1;
        }
        int dt2 = t.split();
        t.reset();
        for(size_t init = 1; init < 1024; ++init) 
        for (size_t i=init; i<S*3; i+=3) {
        	counter64[i%S] += 1;
        }
        int dt3 = t.split();
         cout<<std::setprecision(4)<<dt1 <<"\t\t"<<dt2<<"\t\t"<<dt3<<endl;
        cout<<"ignore: "<<counter8[12]+counter32[32]+counter64[444]<<endl;

	
}
