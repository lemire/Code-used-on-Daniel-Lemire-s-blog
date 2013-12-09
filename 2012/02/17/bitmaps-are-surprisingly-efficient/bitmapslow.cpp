/**
*
* Daniel Lemire, February 2012, http://lemire.me/
*
* Bitmaps are surprisingly efficient
* http://lemire.me/blog/archives/2012/02/17/bitmaps-are-surprisingly-efficient/
*
* Suppose that you want to copy an array over
* another, except for some exceptions. 
* What is faster?
*
*  g++ -Ofast -o bitmapslow bitmapslow.cpp 

*
* Published on blog. 
* Revised on Feb. 27 2011 after bug report by Martin Trenkmann
*/


#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

using namespace std;

typedef unsigned int uint;
typedef unsigned char byte;




class ZTimer
{
public:
    struct timeval t1, t2;
public:
    ZTimer() :  t1(), t2() { gettimeofday(&t1,0); t2 = t1; }
    void reset() {gettimeofday(&t1,0); t2 = t1;}
    int elapsed() { return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000); }
    int split() { gettimeofday(&t2,0); return elapsed(); }
};


/**
* This copies the data from begin to end. We write to "out".
*/
__attribute__ ((noinline)) void justcopy(const uint * const  __restrict__ begin, 
		const uint * const  end, 
	uint * __restrict__ out) {
	for(const uint *  __restrict__ i = begin; i!=end;++i) {
		*(out++) = *i;
	}
	/* we could use this instead of the previous loop, but it is no faster:
	 memcpy ( out, begin, (end-begin)*sizeof(uint) );
	 at least under gcc 4.6
	*/
}

/**
* This copies the data from begin to end, except at some locations
* given by exceptionpos where it must use the values from
* exceptionval. We write to "out".
*/ 
__attribute__ ((noinline)) void patchedcopy(const uint * const  __restrict__ begin, 
		const uint * const  end, 
		const  uint * __restrict__ exceptionval, 
		const vector<uint> & exceptionpos, 
	uint * __restrict__ out) {
	uint * __restrict__ tmpout = out;
	for(const uint *  __restrict__ i = begin; i!=end;++i) {
		*(tmpout++) = *i;
	}
	for(vector<uint>::const_iterator pos = exceptionpos.begin(); pos!= exceptionpos.end();++pos) {
		*(out + *pos) = *(exceptionval++);
	}
}

/**
* This copies the data from begin to end, except at some locations
* given by exceptionpos where it must use the values from
* exceptionval. We write to "out".
*
* Proposed by Luc Golden
*/ 
__attribute__ ((noinline)) void patchedcopy2(const uint * const  __restrict__ begin, 
		const uint * const  end, 
		const  uint * __restrict__ exceptionval, 
		const vector<uint> & exceptionpos, 
	uint * __restrict__ out) {
	vector<uint>::const_iterator pos = exceptionpos.begin();
	const uint *  __restrict__ i = begin;
	for(; i!=end;++i) {
		if(i-begin == *pos) {
			*(out++) =  *(exceptionval++);
			++pos;
			if(pos==exceptionpos.end()) {
			  // no more exceptions
			  for(;i!=end;++i) {
				*(out++) = *i;
			  }
			  break;
			}
		} else
			*(out++) = *i;
	}
	
}



/**
* This copies the data from begin to end, except at some locations
* given by exceptionbitmap where it must use the values from
* exceptionval. We write to "out".
*/
__attribute__ ((noinline)) void bitmapcopy(const uint * const  __restrict__ begin, 
		const uint * const  end, 
		const  uint * __restrict__ exceptionval, 
		const vector<uint> & exceptionbitmap, 
	uint * __restrict__ out) {
	vector<uint>::const_iterator j = exceptionbitmap.begin();
	for(const uint *  __restrict__ i = begin; i<end;++i) {
		for(uint k = 0; k<32;++k,++i)
			*(out++) = ((*j) & (1U<<k))? *(exceptionval++) : *i;
		++j;--i;
	}
}

/**
* This copies the data from begin to end, except at some locations
* given by exceptionbitmap where it must use the values from
* exceptionval. We write to "out".
*/
__attribute__ ((noinline)) void bitmapcopy8bits(const uint * const  __restrict__ begin, 
		const uint * const  end, 
		const  uint * __restrict__ exceptionval, 
		const vector<uint> & exceptionbitmap, 
	uint * __restrict__ out) {
	const byte * __restrict__ j = reinterpret_cast<const byte *>(&exceptionbitmap[0]);
	for(const uint *  __restrict__ i = begin; i<end;++i) {
		for(uint k = 0; k<8;++k,++i)
			*(out++) = ((*j) & (1U<<k))? *(exceptionval++) : *i;
		++j;--i;
	}
}




vector<uint> generateArray(uint N) {
	vector<uint> ans(N);
	for(uint k = 0; k<N;++k)
	  ans[k] = rand();
	return ans;
}

pair<vector<uint>, vector<uint> > generateExceptLocations(uint N, uint density) {
	vector<uint> positions;
	vector<uint> bitmap(N/32);
	for(uint k = 0; k<N;k+= density) {
		positions.push_back(k);
		bitmap[k/32] |= (1<<(k%32));
	}
	return pair<vector<uint>, vector<uint> >(positions,bitmap);
}
int main() {
	uint N = 1000000*32;//1024*1024*100;
	uint density = 100;
	vector<uint> data = generateArray(N);
	vector<uint> out(N);
	ZTimer z;

	vector<uint> exceptionvals = generateArray(N);// more than we need
	cout <<" density(%)\t time with patching \t Luc Golden\t time with bitmaps\t time with 8-bit bitmaps\t justcopy"<<endl;
	for(uint density = 1; density<50; density+=5) {
		pair<vector<uint>, vector<uint> > exceptionpos = generateExceptLocations( N, density); 
	
		cout<<100.0/density<<"\t\t\t";

		
		z.reset();
		patchedcopy(& data[0], 
		(& data[0]) + N,
		&  exceptionvals[0],
		exceptionpos.first,
		&out[0]);
		cout<<z.split()<<"\t\t\t";

		z.reset();
		patchedcopy2(& data[0], 
		(& data[0]) + N,
		&  exceptionvals[0],
		exceptionpos.first,
		&out[0]);
		cout<<z.split()<<"\t\t\t";

		z.reset();
		bitmapcopy(& data[0], 
		(& data[0]) + N,
		&  exceptionvals[0],
		exceptionpos.second,
		&out[0]);	
		cout<<z.split()<<"\t\t\t";


		z.reset();
		bitmapcopy8bits(& data[0], 
		(& data[0]) + N,
		&  exceptionvals[0],
		exceptionpos.second,
		&out[0]);	
		cout<<z.split()<<"\t\t\t";


		z.reset();
		justcopy(& data[0], 
		(& data[0]) + N,
		&out[0]);	
		cout<<z.split()<<"\t";

		
		cout<<endl;
	}
	return 0;
}



/**

  Processor Name:	Intel Core i7
  Processor Speed:	1.8 GHz
  
$ g++-4 --version
g++-4 (GCC) 4.6.2
$ g++-4 -funroll-loops -Ofast -o bitmapslow bitmapslow.cpp 
lemire@Daniels-MacBook-Air ~/CVS/privatesvnrepos/BitFor/code/Cpp $ ./bitmapslow 
 density(%)	 time with patching 	 Luc Golden	 time with bitmaps	 time with 8-bit bitmaps	 justcopy
100			59			62			48			26			25	
16.6667			46			40			31			27			25	
9.09091			47			40			32			27			24	
6.25			45			42			30			26			24	
4.7619			43			40			31			26			24	
3.84615			41			40			30			26			25	
3.22581			38			41			32			26			24	
2.77778			37			41			30			26			24	
2.43902			35			41			30			26			25	
2.17391			35			43			29			26			25	
*/