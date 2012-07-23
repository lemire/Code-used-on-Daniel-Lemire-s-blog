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
        return ((t2.tv_sec - t1.tv_sec) * 1000) + ((t2.tv_usec - t1. tv_usec)
                / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};




vector<int>  givemeanarray(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k+k/3);
	return bigarray;
}


void slowishSum(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = 1; i != data.size(); ++i) {
         data[i] += data[i - 1] ;
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
    for(int t = 0; t<2;++t) {
      cout <<" test # "<< t<<endl;
      vector<int> data = givemeanarray(N) ;
      vector<int> copydata(data);
      
      time.reset();
      slowishSum(data);
      cout<<"basic sum "<<N/(1000.0*time.split())<<endl;   
      
      data = copydata;

      time.reset();
      sum(data);
      cout<<"smarter sum "<<N/(1000.0*time.split())<<endl;   
 
      data = copydata;

      time.reset();
      fastSum(data);
      cout<<"fast sum "<<N/(1000.0*time.split())<<endl;   
 
      cout<<endl<<endl<<endl;
    }

}



int main() {
	cout << "reporting speed in million of integers per second "<<endl;
    size_t N = 50 * 1000 * 1000 ;
    test(N);
    cout<<"============"<<endl;
    test(N);
}

