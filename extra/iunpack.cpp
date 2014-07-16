
//$ g++ -std=c++11 -march=corei7 -DIACA -O3 -o iunpack iunpack.cpp
//$ /opt/intel/iaca/bin/iaca.sh -mark 1 -64 iunpack
//$ /opt/intel/iaca/bin/iaca.sh -mark 2 -64 iunpack

#include <cstdint>
#include <x86intrin.h>


#include </opt/intel/iaca/include/iacaMarks.h>

__m128i PrefixSum(__m128i curr, __m128i prev) {
        return _mm_add_epi32(curr, prev);
}


// unpack and compute the prefix sum in one go
__m128i iunpack8(__m128i  initOffset, const  __m128i   *in, uint32_t    *_out) {
IACA_START;
    __m128i   *out = reinterpret_cast<__m128i *>(_out);
    __m128i    InReg = _mm_load_si128(in);
    __m128i    OutReg;
    __m128i     tmp;
    const __m128i mask =  _mm_set1_epi32((1U << 8) - 1);



    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    OutReg = PrefixSum(OutReg, initOffset);
    initOffset = OutReg;
    _mm_store_si128(out++, OutReg);
IACA_END;

    return initOffset;

}

// unpack and then computes the prefix sum
__m128i iunpack8_twostep(__m128i  initOffset, const  __m128i   *in, uint32_t    *_out) {
IACA_START;
    __m128i   *out = reinterpret_cast<__m128i *>(_out);
    __m128i    InReg = _mm_load_si128(in);
    __m128i    OutReg;
    __m128i     tmp;
    const __m128i mask =  _mm_set1_epi32((1U << 8) - 1);



    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    
    
    _mm_store_si128(out++, OutReg);

    __m128i *pCurr = reinterpret_cast<__m128i *>(_out);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
            initOffset = PrefixSum(_mm_load_si128(pCurr), initOffset);
            _mm_store_si128(pCurr++, initOffset);
IACA_END;
    return initOffset;

}
// unpack without prefix sum
__m128i iunpack8_justfirststep(__m128i  initOffset, const  __m128i   *in, uint32_t    *_out) {
IACA_START;
    __m128i   *out = reinterpret_cast<__m128i *>(_out);
    __m128i    InReg = _mm_load_si128(in);
    __m128i    OutReg;
    __m128i     tmp;
    const __m128i mask =  _mm_set1_epi32((1U << 8) - 1);



    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    ++in;    InReg = _mm_load_si128(in);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = InReg;
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 8);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 16);
    OutReg = _mm_and_si128(tmp, mask);
    
    
    _mm_store_si128(out++, OutReg);

    tmp = _mm_srli_epi32(InReg, 24);
    OutReg = tmp;
    
    
    _mm_store_si128(out++, OutReg);
IACA_END;
    return initOffset;

}


int main() {
    return 0;
}
