// g++-fsf-4.7 -O3 -o bytevsintcounters  bytevsintcounters.cpp

#include <sys/types.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iomanip>
#include <iostream>
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
    uint64_t elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1. tv_usec));
    }
    uint64_t split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};


template <class uword>
int counter(uint32_t S) {
     vector<uword> counter(S);	    
     uint32_t gap = 3 * S/(64*1024);
     for(uint32_t init = 1; init < 1024; ++init) 
        for (uint32_t i=init; i<S*3; i+=gap) {
        	counter[i%S] += 1;// ignore overflows
        }
     int c = 0;
     for(uint32_t i = 0; i < S; ++i)
        if(counter[i]>2) ++c;
     return c;

}


// ideally S should be a power of two
void test(uint32_t S) {
	    cout<<" Number of counters = "<<S<<endl;

	    int bogus = 0;
        WallClockTimer t;
        bogus += counter<uint8_t>(S);
        int dt1 = t.split();
        t.reset();
        bogus += counter<uint16_t>(S);
        int dt2 = t.split();
        t.reset();
        bogus += counter<uint32_t>(S);
        int dt3 = t.split();
        t.reset();
        bogus += counter<uint64_t>(S);
        int dt4 = t.split();
        uint32_t gap = 3 * S/(64*1024);
        
        double number = (1024.0 * S)/gap;
         cout<<std::setprecision(4)<<number/dt1 <<"\t\t"<<number/dt2<<"\t\t"<<number/dt3<<"\t\t"<<number/dt3<<endl;
        cout<<"ignore: "<<bogus<<endl;
}



using namespace std;
int main() {
   cout<<" speeds for 8-bit, 16-bit, 32-bit and 64-bit counters"<<endl;
   test(1024*64);	
   test(1024*1024*8);	
   test(1024*1024*32);	
   test(1024*1024*64);	

}
