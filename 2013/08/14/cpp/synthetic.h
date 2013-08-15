/**
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 * (c) Daniel Lemire
 */

#ifndef SYNTHETIC_H_
#define SYNTHETIC_H_

#include <stdint.h>
#include <vector>
#include <cassert>
#include <set>
#include <unordered_set>// assumes C++11
#include <algorithm>
#include "boolarray.h"
#include "mersenne.h"

using namespace std;




class UniformDataGenerator {
public:
    UniformDataGenerator(uint32_t seed = time(NULL)) :
        rand(seed) {
    }
    /**
     * fill the vector with N numbers uniformly picked from  from 0 to Max, not including Max
     * if it is not possible, an exception is thrown
     */
    vector<uint32_t> generateUniformHash(uint32_t N, uint32_t Max) {
        if(Max < N) throw runtime_error("can't generate enough distinct elements in small interval");
        vector < uint32_t > ans;
        if(N==0) return ans; // nothing to do
        ans.reserve(N);
        assert(Max >= 1);

        if (2*N > Max) {
            unordered_set <uint32_t> s;
            while (s.size() < Max - N )
                    s.insert(rand.getValue(Max - 1) );
            vector<uint32_t> tmp(s.begin(), s.end());
            s.clear();
            sort(tmp.begin(),tmp.end());
            tmp.push_back(Max);
            ans.resize(N);
            size_t i = 0;
            size_t c = 0;
            for(size_t j = 0; j < tmp.size() ; ++j) {
            	const uint32_t v = tmp[j];
            	for(; i<v; ++i)
            	  ans[c++] = i;
            	++i;
            }
            assert(c == ans.size());
        } else {
        	unordered_set <uint32_t> s;
        	while (s.size() < N )
                s.insert(rand.getValue(Max - 1) );
        	ans.assign(s.begin(), s.end());
        	sort(ans.begin(),ans.end());
            assert(N == ans.size());
        }
        return ans;
    }

    /**
     * fill the vector with N numbers uniformly picked from  from 0 to Max, not including Max
     * if it is not possible, an exception is thrown
     */
    vector<uint32_t> generateUniform(uint32_t N, uint32_t Max) {
        if(Max < N) throw runtime_error("can't generate enough distinct elements in small interval");
        vector < uint32_t > ans;
        if(N==0) return ans; // nothing to do
        ans.reserve(N);
        assert(Max >= 1);

        if (2*N > Max) {
            set <uint32_t> s;
            while (s.size() < Max - N )
                    s.insert(rand.getValue(Max - 1) );
            s.insert(Max);
            ans.resize(N);
            size_t i = 0;
            size_t c = 0;
            for(uint32_t v : s) {
            	for(; i<v; ++i)
            	  ans[c++] = i;
            	++i;
            }
            assert(c == ans.size());
        } else {
        	set <uint32_t> s;
        	while (s.size() < N )
                s.insert(rand.getValue(Max - 1) );
        	ans.assign(s.begin(), s.end());
            assert(N == ans.size());
        }
        return ans;
    }
    vector<uint32_t> fastgenerateUniform(uint32_t N, uint32_t Max) {
    	if(N*1024 > Max) {
    	  return generateUniformBitmap(N,Max);
    	}
    	return generateUniform(N,Max);
    }

    vector<uint32_t> generateUniformBitmap(uint32_t N, uint32_t Max) {
        if(Max < N) throw runtime_error("can't generate enough distinct elements in small interval");
        assert(Max >= 1);
        BoolArray bs(Max);
        uint32_t card = 0;
        while(card < N) {
        	uint32_t v = rand.getValue(Max - 1) ;
        	if(!bs.get(v)) {
        		bs.set(v);
        		++card;
        	}
        }
        vector<uint32_t> ans(N);
        bs.toArray(ans);
        return  ans;
    }
    
      vector<uint32_t> generateUniformReservoirSampling(uint32_t N, uint32_t Max) {
        if(Max < N) throw runtime_error("can't generate enough distinct elements in small interval");
        assert(Max >= 1);
        vector<uint32_t> ans;
        ans.resize(N);
        for (uint32_t k = 0; k < N; ++k)
                ans[k]=k;
        for(uint32_t k = N ; k < Max; ++k) {
        	uint32_t v = rand.getValue(k+1) ;
        	if(v < N) {
        		ans[v] = k;
        	}
        }
        sort(ans.begin(),ans.end());
        return ans;
    }

    // Max value is excluded from range
    vector<uint32_t> generate(uint32_t N, uint32_t Max) {
    	return generateUniform(N,Max);
    }
    ZRandom rand;

};


#endif /* SYNTHETIC_H_ */
