/**
g++ -O2 -march=native -o simdextract simdextract.cpp && ./simdextract
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <stdint.h>
#include <x86intrin.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;


const static __m128i shuffle_mask[4] = {
        _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,3,2,1,0),
        _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,7,6,5,4),
        _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,11,10,9,8),
        _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,15,14,13,12),
};

uint32_t branchlessextract (__m128i out, int i)  {
  return _mm_cvtsi128_si32(_mm_shuffle_epi8(out,shuffle_mask[i]));
}
uint32_t branchingextract (__m128i out, int i)  {
              switch (i) {                                     
                case 0: return _mm_extract_epi32(out, 0);                   
                case 1: return _mm_extract_epi32(out, 1);                   
                case 2: return _mm_extract_epi32(out, 2);                   
                case 3: return _mm_extract_epi32(out, 3);                   
                default: assert("problem");                      
              }
              return 0;                                                             
}


int main() {
    __m128i out = _mm_set_epi32(4,3,2,1);
    const size_t repeat = 10000;
    for(int T=0; T<4; ++T) {
        assert(branchlessextract (out,T) ==  branchingextract (out,T));
    }
 
    for(int k = 0; k < 3; ++k) {
      const clock_t S0 = clock();
      uint32_t bogus1 = 0;   
      for(int T=0; T<repeat; ++T) {
        bogus1 += branchlessextract (out,T%4) ;
      }
      const clock_t S1 = clock();
      uint32_t bogus2 = 0;   
      for(int T=0; T<repeat; ++T) {
        bogus2 +=  branchingextract (out,T%4) ;

      }
      const clock_t S2 = clock();
      cout<<"ignore:"<<bogus1<<" "<<bogus2<<endl;
      cout<<"branchless: "<<S1-S0<<endl;
      cout<<"branching: "<<S2-S1<<endl;
      cout<<endl;
    }
}
