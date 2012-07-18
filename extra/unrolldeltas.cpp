//g++-fsf-4.7   -O3 -o unrolldeltas unrolldeltas.cpp

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

template <int mindist>
void delta(vector<int> & data) {
	  if(data.size() == 0) return;
      for (size_t i = data.size() - 1; i > 0; --i) {
         data[i] -=  data[i - 1] + mindist;
      }
}


template<size_t width, int mindist>
void deltaUnrolled(vector<int> & data) {
	  if(data.size() == 0) return;
      size_t i;
      for (i = data.size() - 1; i > width-1; i-=width) {
        // for width sufficiently small, the compiler my unroll this next loop:
      	for(size_t j = 0; j < width; ++j) {
      		data[i-j] -=  data[i - 1 - j] + mindist;
      	}
      }
      for (; i > 0; --i) {
         data[i] -=  data[i - 1] + mindist;
      }

}


template <int mindist>
void inverseDelta(vector<int> & data) {
      for (size_t i = 1; i < data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}

template<size_t width, int mindist>
void inverseDeltaUnrolled(vector<int> & data) {
      size_t i = 1;
      if(data.size()>=width)
        for (; i <= data.size() - width; i+=width) {
        	// for width sufficiently small, the compiler my unroll this next loop:
        	for(size_t j = 0; j < width; ++j)
              data[i + j] += data[i + j - 1] + mindist;
        }
      for (; i < data.size(); ++i) {
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
      delta<1>(data);
      cout<<"delta speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDelta<1>(data);
      cout<<"inverse delta speed "<<N/(1000.0*time.split())<<endl;   
      if(data != copydata) throw runtime_error("bug!");
      cout<<endl;


      time.reset();
      deltaUnrolled<1,1>(data);
      cout<<"delta unrolled 1 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<1,1>(data);
      cout<<"inverse delta unrolled 1 speed "<<N/(1000.0*time.split())<<endl;   
      if(data!= copydata) throw runtime_error("bug!"); 
      cout<<endl;

      time.reset();
      deltaUnrolled<2,1>(data);
      cout<<"delta unrolled 2 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<2,1>(data);
      cout<<"inverse delta unrolled 2 speed "<<N/(1000.0*time.split())<<endl;   
      if(data!= copydata) throw runtime_error("bug!");
      cout<<endl;
      
      time.reset();
      deltaUnrolled<4,1>(data);
      cout<<"delta unrolled 4 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<4,1>(data);
      cout<<"inverse delta unrolled 4 speed "<<N/(1000.0*time.split())<<endl;   
      if(data!= copydata) throw runtime_error("bug!");

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