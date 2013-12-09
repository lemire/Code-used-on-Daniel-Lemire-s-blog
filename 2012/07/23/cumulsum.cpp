/**
* Fast computations of cumulative sums. 
* D. Lemire, July 2012
*
* Best results with:
* 

$ g++-4.7  -funroll-loops -O3 -o cumulsum cumulsum.cpp 

$ ./unrolldeltas 

but we want to avoid the -funroll-loops flag.
*/


// This may require GCC 4.4/


#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>

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




int* givemeanarray_simple(size_t N) {
	int* bigarray;
	bigarray=new int[N];
	for(unsigned int k=0;k<N;k++)
	  bigarray[k]=(k+k/3);
	return bigarray;
}

vector<int>  givemeanarray(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k+k/3);
	return bigarray;
}


void straightsum(int * data, size_t size) {
      if(size == 0) return;
      for (size_t i = 1; i != size; ++i) {
         data[i] += data[i - 1] ;
      }
}

void slowishSum(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = 1; i != data.size(); ++i) {
         data[i] += data[i - 1] ;
      }
}


// By David Stocking
// (reported as buggy?)
void iterSum(vector<int> & data) {
        if(data.size() == 0) return;
 
        for ( vector<int>::iterator i=data.begin(); i != data.end(); ++i ) {
                *(i+1) += *i;
        }
}


// by Nathanaël Schaeffer
void sum2(vector<int> & data) {
	size_t n = data.size();
      if(n == 0) return;
      int s0 = 0;
      int s1 = 0;
      size_t i = 0;
      for (; i < n - 1; i+=2) {		// keep things aligned, starting at 0 (not 1)
		int x0 = data[i];
		int x1 = data[i+1];
		s0 = s1+x0;
		s1 += (x1+x0);
		data[i] = s0;
		data[i+1] = s1;
	  }
	  for (; i<n; i++) {
		  data[i] += data[i-1];
	  }
}

// by Nathanaël Schaeffer
void sum3(vector<int> & data) {
	size_t n = data.size();
      if(n == 0) return;
      int s0 = 0;
      int s1 = 0;
      int s2 = 0;
      size_t i = 0;
      for (; i < n - 2; i+=3) {		// keep things aligned, starting at 0 (not 1)
		int x0 = data[i];
		int x1 = data[i+1];
		int x2 = data[i+2];
		s0 = s2 + x0;			// three independant adds
		s1 = s2 + (x1+x0);		// are performed in parallel
		s2 = s0 + (x1+x2);		// on modern x86.
		data[i] = s0;
		data[i+1] = s1;
		data[i+2] = s2;
	  }
	  for (; i<n; i++) {
		  data[i] += data[i-1];
	  }
}



// By Vasily Volkov, improved by Daniel Lemire
void fastSum(vector<int> & data) {
    if(data.size() == 0) return;

    const size_t UnrollQty = 4;
    size_t sz0 = (data.size() / UnrollQty)  * UnrollQty; // equal to 0, if data.size() < UnrollQty
    size_t i = 1;

    if (sz0>=UnrollQty) {
        int a = data[0];
        for (; i < sz0 - UnrollQty ; i += UnrollQty) {
          a = (data[i] += a);
          a = (data[i + 1] += a);
          a = (data[i + 2] += a);
          a = (data[i + 3] += a);
       }
    }
    for (; i != data.size(); ++i) {
       data[i] += data[i- 1] ;
    }
}

// loop unrolling helps avoid the need for -funroll-loops
void sum(vector<int> & data) {
      if(data.size() == 0) return;
      size_t i = 1;
      for (; i < data.size() - 1; i+=2) {
         data[i] += data[i - 1];
         data[i+1] += data[i ];
       }
      for (; i != data.size(); ++i) {
         data[i] += data[i - 1];
      }
}



void test(size_t N ) {
    WallClockTimer time;
    int bogus = 0;
    cout<<"N = "<<N<<endl;
    for(int t = 0; t<2;++t) {
      cout <<" test # "<< t<<endl;
      vector<int> data = givemeanarray(N) ;
      vector<int> copydata(data);
      
      time.reset();
      straightsum(&data[0],N);
      cout<<"straight sum (C-like) "<<N/(1000.0*time.split())<<endl;   
      bogus += data.back();
      data = copydata;

 
      time.reset();
      slowishSum(data);
      cout<<"basic sum (C++-like) "<<N/(1000.0*time.split())<<endl;   
      bogus += data.back();
      data = copydata;
     
      time.reset();
      iterSum(data);
      cout<<"iterator-based sum (C++-like) "<<N/(1000.0*time.split())<<endl;   
      bogus += data.back();
      data = copydata;


      time.reset();
      sum2(data);
      cout<<"smarter sum2 "<<N/(1000.0*time.split())<<endl;   
 
 
      time.reset();
      sum3(data);
      cout<<"smarter sum3 "<<N/(1000.0*time.split())<<endl;   
 

      time.reset();
      sum(data);
      cout<<"smarter sum "<<N/(1000.0*time.split())<<endl;   
 
      bogus += data.back();
      data = copydata;

      time.reset();
      fastSum(data);
      cout<<"fast sum "<<N/(1000.0*time.split())<<endl;   
 
      bogus += data.back(); 
      cout<<endl<<endl<<endl;

    }
    cout<<"ignore this: "<<bogus<<endl;

}



int main() {
	cout << "reporting speed in million of integers per second "<<endl;
    size_t N = 50 * 1000 * 1000 ;
    test(N);
    cout<<"============"<<endl;
    test(N);
}

