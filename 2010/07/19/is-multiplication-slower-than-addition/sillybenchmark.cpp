
/**
* Is multiplication slower than addition?
* http://lemire.me/blog/archives/2010/07/19/is-multiplication-slower-than-addition/
*/

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
using namespace std;

//g++ -O2 -o sillybenchmark sillybenchmark.cpp


typedef unsigned long long uint64;
class ZTimer
{
public:
    struct timeval t1, t2;
public:
    ZTimer() { gettimeofday(&t1,0); t2 = t1; }
    void reset() {gettimeofday(&t1,0); t2 = t1;}
    int elapsed() { return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000); }
    int split() { gettimeofday(&t2,0); return elapsed(); }
};


uint64 test1() {// standard scalar product
	const int N =1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k<N;++k)
	 answer += vector1[k] * vector2[k];
	return answer;
}

uint64 test2() {//  scalar product 2x2
	const int N = 1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k+1<N;k+=2)
	 answer += vector1[k] * vector2[k]+vector1[k+1] * vector2[k+1];
	return answer;
}

uint64 test3() {// not a scalar product (fewer multiplications)
	const int N = 1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k+1<N;k+=2)
	 answer += ( vector1[k] + vector2[k] ) * ( vector1[k+1] + vector2[k+1] );
	return answer;
}

uint64 test4() {// standard scalar product with squares
	const int N =1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k<N;++k) {
	 answer += (vector1[k] + vector2[k])* (vector1[k] + vector2[k]);
	}
	return answer;
}

uint64 test5() {//  scalar product 4x4
	const int N = 1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k+1<N;k+=4)
	 answer += vector1[k] * vector2[k]+vector1[k+1] * vector2[k+1]+vector1[k+2] * vector2[k+2]+vector1[k+3] * vector2[k+3];
	return answer;
}

uint64 test6() {//  just additions
	const int N = 1024*32;
	uint64 vector1[N];
	uint64 vector2[N];
	uint64 answer;
	for(int k =0; k+1<N;k+=4)
	 answer += vector1[k] + vector2[k];
	return answer;
}

int main(int params, char ** args) {
	ZTimer t;
	uint64 fakesum;
	double timeelapsed;
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test1();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test2();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test3();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;	
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test4();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test5();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;
	t.reset();
	for(int loop = 1; loop<10000;++loop) fakesum += test6();
	timeelapsed = t.split()/(1000.0);
	cout<<timeelapsed<<endl;
	cout<<endl;
	cout<<fakesum<<endl;
}