
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <immintrin.h>
#include <cassert>
#include <iostream>
#include <sstream>
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
    int elapsed() {
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};




int sum(int *  data, size_t length) {
	int answer = 0;
	for(size_t i = 0; i < length;++i){
		answer += data[i];
	}
	return answer;
}
int resum(int *  data, int length) {
	int answer = 0;
	for(int i = length - 1; i >=0 ;--i){
		answer += data[i];
	}
	return answer;
}
#ifdef __SSE

template <class T>
string stringSIMD(__m128i x, bool hexflag=false) {
    typedef union {
        __m128i v;
        T a[16/sizeof(T)];
   } U;
   U y = {x};
   ostringstream convert;
   if(hexflag) convert<<hex;
   convert<<"{";
   for(size_t i = 0; i < 16/sizeof(T);++i) {
       if(sizeof(T)==1)
           convert<<(hexflag?"0x":"")<<static_cast<int>(y.a[i]);
       else
           convert<<(hexflag?"0x":"")<<y.a[i];
       if(i+1!=16/sizeof(T)) convert<<",";
   }
   convert<<"}";
   return convert.str();
}

static  const __m128i shufflemask =  _mm_set_epi8 (128,128,128,128,128,128,128,128,128,128,128,128,15,14,13,12);

int SSEsum(int * data, size_t length) {
	assert(sizeof(int) == 4);// hidden assumption
	assert(length %  4 == 0);// for simplicity
	int answer = 0;
	__m128i* p = reinterpret_cast<__m128i*>(data);
	__m128i  u = _mm_setzero_si128 ();
	for(size_t i = 0; i < length / 4 ; ++i) {
		u = _mm_add_epi32(_mm_load_si128(p++),u);
		u = _mm_add_epi32( u , _mm_slli_si128(u,4));
		u = _mm_add_epi32( u , _mm_slli_si128(u,8));
		//cout<<"before "<<stringSIMD<int>(u)<<endl;
		
		u = _mm_shuffle_epi8(u,shufflemask);
		//cout<<"after "<<stringSIMD<int>(u)<<endl;
	}
	return _mm_extract_epi32 (u,0);
}
#endif

/*
// fun test. Requires AVX2 but I lack a compatible processor
int AVXsum(int * data, size_t length) {
	assert(sizeof(int) == 4);// hidden assumption
	assert(length %  8 == 0);// for simplicity
	__m256i* p = reinterpret_cast<__m256i*>(data);
	__m256i  init = _mm256_setzero_si256 ();
	__m256i u;
	for(size_t i = 0; i < length / 8 ; ++i) {
		u = _mm256_loadu_si256(p);
		u = _mm256_add_epi32( u, init);
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,4));
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,8));
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,16));
        init =  _mm256_shuffle_epi32 (u, 0xFF);
	}
	int tmp[8];
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(tmp),u);
	return tmp[7];
}*/

int main() {
	const size_t N = 1024*1024*32*4;
	vector<int> x(N);
	for( size_t i = 0; i < N; ++i)
	  x[i] = i;
        int a1 = 0;
        int a2 = 0;
	WallClockTimer t;
	t.reset();
	for(int k =  0; k < 100 ; ++k) a1 += sum(&x[0],N);
	cout<<"time = "<<t.split()<<endl;
	cout<<"a1 = "<<a1<<endl;
	t.reset();
	for(int k =  0; k < 100 ; ++k) a2 += resum(&x[0],N);
	cout<<"re time = "<<t.split()<<endl;
	cout<<"a2 = "<<a2<<endl;
	t.reset();
#ifdef __SSE
	int a2 = SSEsum(&x[0],N);
	cout<<"time = "<<t.split()<<endl;
	cout<<"a2 = "<<a2<<endl;
	if(a1!=a2) cerr<<"bug"<<endl;
#endif

}
