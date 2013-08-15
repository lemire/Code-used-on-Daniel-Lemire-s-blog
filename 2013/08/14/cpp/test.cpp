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
    uint32_t TIMES = 10;
    cout<<"reporting speed"<<endl;
    cout<<"tree -- bitmap -- hash -- reservoirsampling"<<endl;
	for(uint32_t N = max/2048; N < max; N*=2)     
    {
        cout<<max*1.0/N<<"\t\t\t";
        cout.flush();
            	

        ZTimer t;
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniform(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt1 = t.split();
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformBitmap(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt2 = t.split();
        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformHash(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt3 = t.split();


        for (int i=0; i<TIMES; ++i) {
                vector<uint32_t> array = udg.generateUniformReservoirSampling(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt4 = t.split();

        cout<<TIMES*N*1.0/dt1<<"\t\t"<<TIMES*N*1.0/dt2<<"\t\t"<<TIMES*N*1.0/dt3<<"\t\t"<<TIMES*N*1.0/dt4<<endl;

	}

    return 0;
}