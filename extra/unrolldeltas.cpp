//g++-fsf-4.7   -O3 -o unrolldeltas unrolldeltas.cpp

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
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


void delta(vector<int> & data) {
      for (size_t i = data.size() - 1; i > 0; --i) {
         data[i] = data[i] - data[i - 1] - 1;
      }
}


template<size_t width>
void deltaUnrolled(vector<int> & data) {
      size_t i;
      for (i = data.size() - 1; i > width-1; i-=width) {
      	for(size_t j = 0; j < width; ++j) {
      		data[i-j] = data[i-j] - data[i - 1 - j] - 1;
      	}
      }
      for (; i > 0; --i) {
         data[i] = data[i] - data[i - 1] - 1;
      }

}

void inverseDelta(vector<int> & data) {
      for (size_t i = 1; i < data.size(); ++i) {
         data[i] = data[i] + data[i - 1] + 1;
      }
}

template<size_t width>
void inverseDeltaUnrolled(vector<int> & data) {
      size_t i;
      for (i = 1; i < data.size(); i+=width) {
      	for(size_t j = 0; j< width; ++j)
         data[i + j] = data[i + j] + data[i + j - 1] + 1;
      }
      for (size_t i = 1; i < data.size(); ++i) {
         data[i] = data[i] + data[i - 1] + 1;
      }
}


int main() {
	cout << "reporting speed in million of integers per second "<<endl;
    WallClockTimer time;
    size_t N = 50 * 1000 * 1000 ;
    for(int t = 0; t<5;++t) {
      vector<int> data = givemeanarray(N) ;
      time.reset();
      delta(data);
      cout<<"delta speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDelta(data);
      cout<<"inverse delta speed "<<N/(1000.0*time.split())<<endl;   


      time.reset();
      deltaUnrolled<1>(data);
      cout<<"delta unrolled 1 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<1>(data);
      cout<<"inverse delta 1 speed "<<N/(1000.0*time.split())<<endl;   

      time.reset();
      deltaUnrolled<2>(data);
      cout<<"delta unrolled 2 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<2>(data);
      cout<<"inverse delta 2 speed "<<N/(1000.0*time.split())<<endl;   

      time.reset();
      deltaUnrolled<4>(data);
      cout<<"delta unrolled 4 speed "<<N/(1000.0*time.split())<<endl;   
      time.reset();
      inverseDeltaUnrolled<4>(data);
      cout<<"inverse delta 4 speed "<<N/(1000.0*time.split())<<endl;   


      cout<<endl;
    }

}