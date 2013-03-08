//  g++ -O3 -std=c++11 -o testvector testvector.cpp

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

int runtestemplacewreserve(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.emplace_back(k);
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

template <typename T>
struct iota_iterator : std::iterator<std::forward_iterator_tag, T> {
	iota_iterator(T value = T()) : value(value) { }

	iota_iterator& operator ++() {
		++value;
		return *this;
	}

	iota_iterator operator ++(int) {
		iota_iterator copy = *this;
		++*this;
		return copy;
	}

	T const& operator *() const {
		return value;
	}

	T const* operator ->() const {
		return &value;
	}

	friend bool operator ==(iota_iterator const& lhs, iota_iterator const& rhs) {
		return lhs.value == rhs.value;
	}

	friend bool operator !=(iota_iterator const& lhs, iota_iterator const& rhs) {
		return not (lhs == rhs);
	}

private:
	T value;
};

int runtestgenerator(size_t N) {
	// Extra parentheses to prevent most vexing parse.
	vector<int> bigarray((iota_iterator<int>()), iota_iterator<int>(N));
	int sum = 0;
	for(unsigned int k = 0; k<N; ++k)
	  sum += bigarray[k];
	return sum;
}




int main() {
	
    CPUBenchmark time;
    const size_t N = 100 * 1000 * 1000 ;
    time.start();
    cout.precision(3);
    cout<<" report speed in CPU cycles per integer"<<endl;
    cout<<endl<<"ignore this:"<<runtestnice(N)<<endl;
    cout<<"with push_back:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestnicewreserve(N)<<endl;
    cout<<"with push_back and reserve:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestemplacewreserve(N)<<endl;
    cout<<"with emplace_back and reserve:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestsafe(N)<<endl;
    cout<<"init first:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestunsafe(N)<<endl;
    cout<<"reserve first:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestclassic(N)<<endl;
    cout<<"C++ new:"<<(time.stop()*1.0/N)<<endl;
 	int * bigarray = new int[N];
    time.start();
    cout<<endl<<"ignore this:"<<runtestnoalloc(N,bigarray)<<endl;
    cout<<"without alloc:"<<(time.stop()*1.0/N)<<endl;
    time.start();
    cout<<endl<<"ignore this:"<<runtestgenerator(N)<<endl;
    cout<<"generator iterators:"<<(time.stop()*1.0/N)<<endl;

    return 0;
}
