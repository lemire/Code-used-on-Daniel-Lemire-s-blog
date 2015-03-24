/**
* whether to interleave key and value in a sorted data structure
g++ -O2 -march=native -o interleaved interleaved.cpp && ./interleaved

*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <x86intrin.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

struct __attribute__ ((__packed__)) keyvalue {
    uint16_t key;
    uint16_t value;
};

bool operator<  (const keyvalue& lhs, const keyvalue& rhs) { return lhs.key<rhs.key ; }
bool operator<  (const uint16_t& lhs, const keyvalue& rhs) { return lhs<rhs.key ; }
bool operator<  (const keyvalue& lhs, const uint16_t& rhs) { return lhs.key<rhs ; }

// values must be sorted
uint16_t interleaved (keyvalue * inter, size_t number)  {
    uint16_t maxvalue = inter[number - 1].key;
    uint16_t answer = 0;
    for(uint16_t query = 0; query <= maxvalue; ++query) {
        keyvalue * val = lower_bound(inter, inter+number, query);
        if(val->key == query) answer += val->value;
    }
    return answer;
}

// values must be sorted
uint16_t noninterleaved (uint16_t * keys, uint16_t * values, size_t number)  {
    uint16_t maxvalue = keys[number - 1];
    uint16_t answer = 0;
    for(uint16_t query = 0; query <= maxvalue; ++query) {
        uint16_t * val = lower_bound(&keys[0], (&keys[0])+number, query);
        if(*val == query) answer += values[val-keys];
    }
    return answer;
}

int main() {
    const size_t number = 1024;
    uint16_t keys[number];
    uint16_t values[number];
    keys[0] = 0;
    values[0] = (uint16_t) rand(); 
    for(size_t i = 1; i < number; ++i) {
        keys[i] = keys[i-1] + (rand() % (65536/number));
        values[i] = (uint16_t) rand();
    }
    vector<keyvalue> inter(number);
    for(size_t k = 0; k < number ; ++k) {
        inter[k].key = keys[k];
        inter[k].value = values[k];
    }
    
    const size_t repeat = 100;

    for(int k = 0; k < 10; ++k) {
      const clock_t S0 = clock();
      uint16_t bogus1 = 0;   
      for(int T=0; T<repeat; ++T) {
        bogus1 += interleaved( inter.data(), inter.size());
      }
      const clock_t S1 = clock();
      uint16_t bogus2 = 0;   
      for(int T=0; T<repeat; ++T) {
        bogus2 += noninterleaved(keys,values,number);
      }
      const clock_t S2 = clock();
      cout<<"ignore:"<<bogus1<<" "<<bogus2<<endl;
      cout<<"interleaved: "<<S1-S0<<endl;
      cout<<"non-interleaved: "<<S2-S1<<endl;
      cout<<endl;
    }
}
