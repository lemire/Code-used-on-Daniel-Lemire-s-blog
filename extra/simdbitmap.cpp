//g++-4.7  -std=c++11 -O3 -o simdbitmap simdbitmap.cpp

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>

#include <immintrin.h>

#include <iso646.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>

using namespace std;

    void inplaceIntersect(vector<uint64_t> & buffer, const vector<uint64_t> & other) {
        assert(buffer.size() == other.size());
        for(size_t i = 0; i < buffer.size(); ++i)
            buffer[i] &= other[i];
    }


    void SIMDinplaceIntersect(vector<uint64_t> & buffer, const vector<uint64_t> & other) {
        assert(buffer.size() == other.size());
        __m128i * bin = reinterpret_cast<__m128i *>(buffer.data()); 
        const  __m128i * bo = reinterpret_cast<const  __m128i *>(other.data()); 
        for(size_t i = 0; i < buffer.size()/2; ++i) {
        	__m128i p1 = _mm_load_si128 (bin + 1);
        	__m128i p2 = _mm_load_si128 (bo + 1);
        	__m128i andp1p2 = _mm_and_si128 (p1,p2);
        	 _mm_storeu_si128 (bin + i, andp1p2);
        }
        for(size_t i = buffer.size()/2 * 2; i < buffer.size(); ++i)
            buffer[i] &= other[i];
    }
    
    void intersect(vector<uint64_t> & buffer, const vector<uint64_t> & other, vector<uint64_t> & output) {
        output.resize(buffer.size());
        for(size_t i = 0; i < buffer.size(); ++i)
            output[i] = buffer[i] & other[i];
    }

    void SIMDintersect(const vector<uint64_t> & buffer, const vector<uint64_t> & other, vector<uint64_t> & output) {
        output.resize(buffer.size());
        const  __m128i * bin = reinterpret_cast<const  __m128i *>(buffer.data()); 
        const  __m128i * bo = reinterpret_cast<const  __m128i *>(other.data()); 
        __m128i * bout = reinterpret_cast<__m128i *>(output.data()); 
        
        for(size_t i = 0; i < buffer.size()/2; ++i) {
        	__m128i p1 = _mm_load_si128 (bin + 1);
        	__m128i p2 = _mm_load_si128 (bo + 1);
        	__m128i andp1p2 = _mm_and_si128 (p1,p2);
        	 _mm_storeu_si128 (bout + i, andp1p2);
        }
        for(size_t i = buffer.size()/2*2; i < buffer.size(); ++i)
            output[i] = buffer[i] & other[i];
    }


class WallClockTimer {
public:
#ifdef _WIN32
    typedef qpc_clock clock;
#else
    typedef std::chrono::high_resolution_clock clock;
#endif

    std::chrono::time_point<clock> t1, t2;
    WallClockTimer() :
        t1(), t2() {
        t1 = clock::now();
        t2 = t1;
    }
    void reset() {
        t1 = clock::now();
        t2 = t1;
    }
    uint64_t elapsed() {
    	std::chrono::microseconds delta = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        return delta.count();
    }
    uint64_t split() {
        t2 = clock::now();
        return elapsed();
    }
};


int main() {
	size_t N = 1024*1024;
	vector<uint64_t> in1(N,10);
	vector<uint64_t> in2(N,10);
	vector<uint64_t> out(N);
	WallClockTimer t;
	t.reset();
	inplaceIntersect(in1,in2);
	cout<<t.split()<<endl;
	t.reset();
	SIMDinplaceIntersect(in1,in2);
	cout<<t.split()<<endl;	
	t.reset();
	intersect(in1,in2,out);
	cout<<t.split()<<endl;
	t.reset();
	SIMDintersect(in1,in2,out);
	cout<<t.split()<<endl;	

}
