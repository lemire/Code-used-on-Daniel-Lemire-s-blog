/**
* Usage 
* g++ -O3 -o storeload storeload.cpp
*/
/**
* Reference: 
* http://electronics.stackexchange.com/questions/17549/i-know-why-dram-is-slower-to-write-than-to-read-but-why-is-the-l1-l2-cache-ra
*
* source: Reverend Eric Ha
*/

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

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



__attribute__ ((noinline)) 
int storeTest(vector<int> & data, vector<int> & bigdata, size_t N) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		bigdata.insert(bigdata.end(), data.begin(), data.end());
		bogus += bigdata.back();
	}
	return bogus;
}

__attribute__ ((noinline)) 
int loadTest(vector<int> & data, vector<int> & bigdata, size_t N) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		data.assign(bigdata.begin() + x * data.size(), bigdata.begin() + (x+1) * data.size());
		bogus += data.back();
	}
	return bogus;
}


__attribute__ ((noinline)) 
int storeTestC(int * data, int * bigdata, size_t N, size_t M) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		memcpy(bigdata,data,M*sizeof(int));
		bigdata += M;
		bogus += bigdata[-1];
	}
	return bogus;
}

__attribute__ ((noinline)) 
int loadTestC(int * data, int * bigdata, size_t N, size_t M) {
	int bogus = 0;
	for(size_t x = 0; x<N; ++x) {
		memcpy(data,bigdata,M*sizeof(int));
		bigdata += M;
		bogus += data[M-1];
	}
	return bogus;
}



int testStoreLoad(size_t M =  2048 * 4, size_t N = 2048 * 8, size_t repeat = 1) {
	WallClockTimer timer;
	vector<int> data;
	int bogus;
	for(size_t i = 0; i<M; ++i)
	  data.push_back(i);
	vector<int> bigdata;
	bigdata.reserve(M * N);
	for(size_t t = 0; t< 3; ++t) {
		timer.reset();
		for (size_t r = 0; r < repeat; ++r)
			bogus += storeTest(data,bigdata,N);
		if(t>0) cout<<" store time = "<<timer.split()<<endl;
		timer.reset();
		for (size_t r = 0; r < repeat; ++r)
	    	bogus += loadTest(data,bigdata,N);
		if(t>0) cout<<" load time = "<<timer.split()<<endl;
		bigdata.clear();
		for(int i = 0; i<M; ++i)
	  		assert(data[i] == i);
	}
    return bogus;
}

int testStoreLoadC(size_t M =  2048 * 4, size_t N = 2048 * 8, size_t repeat = 1) {
	WallClockTimer timer;
	vector<int> data;
	int bogus;
	for(size_t i = 0; i<M; ++i)
	  data.push_back(i);
	vector<int> bigdata;
	bigdata.resize(M * N);
	for(size_t t = 0; t< 3; ++t) {
		timer.reset();
		for (size_t r = 0; r < repeat; ++r)
			bogus += storeTestC(&data[0],&bigdata[0],N,M);
		if(t>0) cout<<" store time = "<<timer.split()<<endl;
		timer.reset();
		for (size_t r = 0; r < repeat; ++r)
	    	bogus += loadTestC(&data[0],&bigdata[0],N,M);
		if(t>0) cout<<" load time = "<<timer.split()<<endl;
		for(int i = 0; i<M; ++i)
	  		assert(data[i] == i);
	}
    return bogus;
}


__attribute__ ((noinline)) 
void pack(bool * uncompressed, char * compressed, size_t N) {
	for(size_t i = 0; i < N/8; ++i) {
		size_t x = i * 8;
		compressed[i] = uncompressed[x] | 
		                uncompressed[x+1]<<1 |
		                uncompressed[x+2]<<2 |
		                uncompressed[x+3]<<3 |
		                uncompressed[x+4]<<4 |
		                uncompressed[x+5]<<5 |
		                uncompressed[x+6]<<6 |
		                uncompressed[x+7]<<7 ;
	}
}

__attribute__ ((noinline)) 
void unpack(char * compressed, bool * uncompressed, size_t N) {
	for(size_t i = 0; i < N/8; ++i) {
		size_t x = i * 8;
		uncompressed[x] = compressed[i] & 1;
		uncompressed[x+1] = compressed[i] & 2;
		uncompressed[x+2] = compressed[i] & 4;
		uncompressed[x+3] = compressed[i] & 8;
		uncompressed[x+4] = compressed[i] & 16;
		uncompressed[x+5] = compressed[i] & 32;
		uncompressed[x+6] = compressed[i] & 64;
		uncompressed[x+7] = compressed[i] & 128;
	}
}


void testPackUnpackC(size_t N =  2048 * 32 * 2048) {
	WallClockTimer timer;
	bool* data = new bool[N];
	for(size_t i = 0; i<N; ++i)
	  data[i] = static_cast<bool>(i & 1);
	vector<char> comp(N/8);
	for(size_t t = 0; t< 3; ++t) {
		timer.reset();
		pack(data, &comp[0], N);
		cout<<" pack time = "<<timer.split()<<endl;
		timer.reset();
		unpack(&comp[0], data, N);
		cout<<" unpack time = "<<timer.split()<<endl;
		for(size_t i = 0; i<N; ++i) 
			assert(data[i] == static_cast<bool>(i & 1));	  
	}
	delete[] data;
}


int main() {
	cout<<"pack-unpack test:"<<endl;
	testPackUnpackC();

	cout<<"cache-cache test:"<<endl;

	cout<<"ignore:"<<testStoreLoad(2048,32,2048)<<endl;

	cout<<"cache-RAM test:"<<endl;

	cout<<"ignore:"<<testStoreLoad()<<endl;


	cout<<"cache-cache test:"<<endl;

	cout<<"ignore:"<<testStoreLoad(2048,32,2048)<<endl;

	cout<<"cache-RAM test:"<<endl;

	cout<<"ignore:"<<testStoreLoad()<<endl;


	return 0;
}
