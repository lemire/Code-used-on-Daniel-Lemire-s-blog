#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdexcept>
#include <iostream>

#include "mersenne.h"
#include "synthetic.h"

class ZTimer 
{
    clock_t t1;
    clock_t t2;
public:
    ZTimer() : t1(::clock()), t2(t1) {}
    int elapsed() { return (int)(((t2 - t1) * 1000.0) / CLOCKS_PER_SEC); }
    int split() { t2 = ::clock(); return elapsed(); }
};

int main(int ac,char** av) 
{

	uint32_t max = 1<<20;
    UniformDataGenerator udg;
	for(uint32_t N = max/2048; N < max; N*=2)     
    {
        cout<<max*1.0/N<<" ";
        cout.flush();
            	

        ZTimer t;
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniform(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt1 = t.split();
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniformBitmap(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt2 = t.split();
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniformHash(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt3 = t.split();


        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniformReservoirSampling(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt4 = t.split();

        cout<<dt1<<" "<<dt2<<" "<<dt3<<" "<<dt4<<endl;

	}

    return 0;
}