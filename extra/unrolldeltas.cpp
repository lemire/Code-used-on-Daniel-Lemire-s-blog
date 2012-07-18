/**
* Testing delta coding and decoding.
* D. Lemire, July 2012
*
* Best results with:
* 

$ g++-4.7  -funroll-loops -O3 -o unrolldeltas unrolldeltas.cpp

$ ./unrolldeltas 
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

enum{minoffset=0};
//#pragma GCC optimize("unroll-loops","O3") 
//#pragma GCC push_options

template <int mindist>
void delta(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = data.size() - 1; i > 0; --i) {
         data[i] -=  data[i - 1] + mindist;
      }
}

template <int mindist>
void slowishinverseDelta(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = 1; i != data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}

// loop unrolling helps avoid the need for -funroll-loops
template <int mindist>
void inverseDelta(vector<int> & data) {
      if(data.size() == 0) return;
      size_t i = 1;
      for (; i < data.size() - 1; i+=2) {
         data[i] += data[i - 1] + mindist;
         data[i+1] += data[i ] + mindist;
       }
      for (; i != data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}



template <int mindist>
void test(size_t N ) {
    cout << "min distance between ints is "<<mindist<<endl;
    WallClockTimer time;
    for(int t = 0; t<2;++t) {
      cout <<" test # "<< t<<endl;
      vector<int> data = givemeanarray(N) ;
      vector<int> copydata(data);
      
      time.reset();
      delta<mindist>(data);
      cout<<"delta speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDelta<mindist>(data);
      cout<<"inverse delta speed "<<N/(1000.0*time.split())<<endl;   
      if(data != copydata) throw runtime_error("bug!");
      cout<<endl;


      cout<<endl<<endl<<endl;
    }

}



int main() {
	cout << "reporting speed in million of integers per second "<<endl;
    size_t N = 50 * 1000 * 1000 ;
    test<1>(N);
    cout<<"============"<<endl;
    test<0>(N);
}

//#pragma GCC pop_options
