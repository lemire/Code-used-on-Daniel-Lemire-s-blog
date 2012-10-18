// fun test. Requires AVX2 but I lack a compatible processor

#include <cassert>
#include <immintrin.h>
#include <iostream>

int sum(int *  data, size_t length) {
	int answer = 0;
	for(size_t i = 0; i < length;++i){
		answer += data[i];
	}
	return answer;
}


int AVXsum(int * data, size_t length) {
	assert(sizeof(int) == 4);// hidden assumption
	assert(length %  8 == 0);// for simplicity
	__m256i* p = reinterpret_cast<__m256i*>(data);
	__m256i  init = _mm256_setzero_si256 ();
	__m256i u;
	for(size_t i = 0; i < length / 8 ; ++i) {
		u = _mm256_loadu_si256(p);
		u = _mm256_add_epi32( u, init);
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,4));
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,8));
		u = _mm256_add_epi32( u , _mm256_slli_si256(u,16));
        init =  _mm256_shuffle_epi32 (u, 0xFF);
	}
	int tmp[8];
	_mm256_storeu_si256(reinterpret_cast<__m256i*>(tmp),u);
	return tmp[7];
}

int main() {
	int x[8];
	for( size_t i = 0; i < 8; ++i)
	  x[i] = i;
	std::cout<<sum(x,8)<<std::endl;
	std::cout<<AVXsum(x,8)<<std::endl;
}