#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>      

#include "mersenne.h"
#include "synthetic.h"




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




int lowestFirst(const void *_a, const void *_b) {
    int32_t a = * (int32_t *) _a;
    int32_t b = * (int32_t *) _b;
    return a - b;
}

void fillFromReservoir(uint32_t *buf, uint32_t N, uint32_t Max) {
    for (uint32_t k = 0; k < N; ++k) buf[k] = k;

    for(uint32_t k = N ; k < Max; ++k) {
        uint32_t r = rand() % k;
        if(r < N)  buf[r] = k;
    }

    qsort(buf, N, sizeof(*buf), lowestFirst);

    return;
}



int main(int ac,char** av) 
{

	uint32_t max = 1<<20;
    UniformDataGenerator udg;
    uint32_t TIMES = 10;
    cout<<"reporting speed"<<endl;
    cout<<"tree -- bitmap -- hash -- reservoirsampling -- reservoirsamplingC"<<endl;
	for(uint32_t N = max/2048; N < max; N*=2)     
    {
        cout<<std::setprecision(6)<<max*1.0/N<<"\t\t\t";
        cout.flush();
            	

        WallClockTimer t;
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniform(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt1 = t.split();
        t.reset();
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformBitmap(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt2 = t.split();
        t.reset();
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformHash(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt3 = t.split();
		t.reset();

        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformReservoirSampling(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt4 = t.split();

		t.reset();
        for (int i=0; i<TIMES; ++i) {
        	
  				uint32_t * buffer = (uint32_t*) malloc (sizeof(uint32_t) * N);
  				if (buffer==NULL) exit (1);
  				fillFromReservoir(buffer, N, max);
  				if(buffer[N-1] <= buffer[N-2]) throw runtime_error("bug"); 
				free (buffer);
        }
        int dt5 = t.split();


        cout<<std::setprecision(3)<<TIMES*N*1.0/dt1 <<"\t\t"<<TIMES*N*1.0/dt2<<"\t\t"<<TIMES*N*1.0/dt3<<"\t\t"<<TIMES*N*1.0/dt4<<"\t\t"<<TIMES*N*1.0/dt5<<endl;

	}

    return 0;
}