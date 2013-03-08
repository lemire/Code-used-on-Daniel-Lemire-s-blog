// g++ -O3 -std=c++11 -o testvectorcpp11 testvectorcpp11.cpp 

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned long long ticks;


using namespace std;


static __inline__ unsigned long long startRDTSC (void) {
    unsigned cycles_low, cycles_high;
    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    return (static_cast<unsigned long long>(cycles_high) << 32) | cycles_low;
}

static __inline__ unsigned long long stopRDTSCP (void) {
    unsigned cycles_low, cycles_high;
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax",
            "%rbx", "%rcx", "%rdx");
    return (static_cast<unsigned long long>(cycles_high) << 32) | cycles_low;
}

class CPUBenchmark {
public:
    CPUBenchmark() :
        ticktime(0) {
        start();
    }

    unsigned long long ticktime;

    void start() {
        ticktime = startRDTSC();
    }

    unsigned long long stop() {
        return stopRDTSCP() - ticktime;

    }
};

template<class C>
C runtestnice(size_t N) {
	vector<C> bigarray;
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k);
	C sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}

template<class C>
C runtestnicewreserve(size_t N) {
	vector<C> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k);
	C sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}

template<class C>
C runtestemplacewreserve(size_t N) {
	vector<C> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.emplace_back(k);
	C sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}


// bogus class
class BenchmarkTester {
public:
BenchmarkTester()
: m_myVec(100)
, m_myBool(false)
, m_myPair(24.5, "Happy days are here again")
, X(0)

{
}

BenchmarkTester(int x)
: m_myVec(100)
, m_myBool(false)
, m_myPair(24.5, "Happy days are here again")
, X(x)

{
}

BenchmarkTester& operator+= (const BenchmarkTester & o) {
	X+= o.X;
	return *this;
}

operator int() {
	return X;
}

private:
vector<int> m_myVec;
bool m_myBool;
pair<double,string> m_myPair;
int X;
};



template <class C>
void demo() {
    CPUBenchmark time;
    const size_t N = 2 * 1000 * 1000 ;
    time.start();
    cout.precision(3);
    cout<<" report speed indicator in CPU cycles per integer"<<endl;
    cout<<endl<<"ignore this:"<<runtestnice<C>(N)<<endl;
    cout<<"with push_back:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestnicewreserve<C>(N)<<endl;
    cout<<"with push_back and reserve:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestemplacewreserve<C>(N)<<endl;
    cout<<"with emplace_back and reserve:"<<(time.stop()*1.0/N)<<endl;
    time.start();

}

int main() {
	cout<<"**With int primitive"<<endl;
	demo<int>();
	cout<<endl;
	cout<<"**With BenchmarkTester"<<endl;
	demo<BenchmarkTester>();
    return 0;
}
