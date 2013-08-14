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
        printf("Welcome!");
cout<<"test"<<endl;
	uint32_t max = 1<<20;
    UniformDataGenerator udg;
	for(uint32_t N = max/2048; N < max; N*=2)     
    {
        cout<<"N = "<<N*1.0/max;
        cout.flush();
            	if(N*1024 > max) {
            		cout<<"bitmapmode"<<endl;
            	}

        ZTimer t;
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniform(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt = t.split();
        cout<<" ran in  "<<dt;
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniformBitmap(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt2 = t.split();
        cout<<" ran in  "<<dt2;
        for (int i=0; i<10; ++i) {
                vector<uint32_t> array = udg.generateUniformHash(N,max);
                if(array.size() != N) throw runtime_error("bug");
        }
        int dt3 = t.split();

        cout<<" ran in  "<<dt3<<endl;

	}

    return 0;
}