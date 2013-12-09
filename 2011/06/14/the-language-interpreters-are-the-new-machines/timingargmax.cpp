/**
* The language interpreters are the new machines
* http://lemire.me/blog/archives/2011/06/14/the-language-interpreters-are-the-new-machines/
*/

#include <limits.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>


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
using namespace std;
int main() {
  const int N = 100 * 1000 * 1000;
  vector<int> array;
  array.resize(N);
  for(int k = 0; k<N;++k)
    array[k]=k;
  random_shuffle(array.begin(), array.end());
  ZTimer z;
  for(int j = 0; j<10;++j) {
	  int max = INT_MIN;
	  int bestk = 0;
	  for (int k = 0; k<N; ++k) {
		if(array[k]>max) {
			max = array[k];
			bestk = k;
		}
	  }
      cout<<" "<< bestk<<endl;
  }
  cout<<z.split()<<endl;
  z.reset();
  for(int j = 0; j<10;++j) {
	  int max = INT_MIN;
	  for (int k = 0; k<N; ++k) {
		if(array[k]>max) max = array[k];
	  }
	  for (int k = 0; k<N;++k) 
	   if(array[k] == max) {
	        cout<<" "<< k<<endl;
	        break;
	   }
  }
  cout<<z.split()<<endl;
  return 0;
}