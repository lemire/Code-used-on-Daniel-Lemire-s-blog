/*
* various methods to strip whitespace from text
* (aka. despace, leftpack, copy_if)
*
* 'whitespace' is considered the following bytes:
* 0x09 - tab
* 0x20 - space
* 0x0A - line feed
* 0x0D - carriage return
*/

// 3x faster on unpredicatble data vs naive version
void* despacer_bitmap (char* dst, const char* src, size_t count)
{
    const uint64_t bitmap = UINT64_C(0xFFFFFFFEFFFFC1FF);
    if (count != 0) {
        do {
            uint64_t c = *(const unsigned char*)src;
            src++;
            *dst = (char)c;
            dst += ((bitmap >> c) & 1) | ((c + 0xC0) >> 8);
        } while (--count);
    }
    return dst;
}


// SWAR bit-twiddling for sparse whitespace only?
size_t despace_block_mux( void* dst_void, void* src_void, size_t length )
{
	uint64_t* src = (uint64_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const uint64_t mask_09 = 0x0909090909090909;
	const uint64_t mask_0A = 0x0A0A0A0A0A0A0A0A;
	const uint64_t mask_0D = 0x0D0D0D0D0D0D0D0D;
	const uint64_t mask_20 = 0x2020202020202020;
	const uint64_t mask_7F = 0x7F7F7F7F7F7F7F7F;

	for( ; length >= 8; length-=8 ){
		uint64_t asrc = *src++;
		size_t ws_cnt = 0;

		uint64_t mask = asrc & mask_7F;
		mask =
			((mask ^ mask_09) + mask_7F) &
			((mask ^ mask_20) + mask_7F) &
			((mask ^ mask_0A) + mask_7F) &
			((mask ^ mask_0D) + mask_7F);
		mask = ~mask & ~asrc & ~mask_7F;
		// mask = bit7 of each byte is set if space

		if(mask != 0){
			do{
				uint64_t pattern = (mask ^ (mask - 1)); // lowest set bit and below
				asrc = (((asrc << 8) & pattern) | (asrc & ~pattern)); // blend
				ws_cnt++; // count spaces
				mask &= ~pattern; // zero lowest set bit in mask
			}while(mask != 0);
			asrc >>= ws_cnt*8; // little endian
		}

		*((uint64_t*)dst) = asrc;
		dst += (8 - ws_cnt);
	}
	dst += despace_branchless(dst, src, length);
	return (size_t)(dst - ((uint8_t*)dst_void));
}


// no lookup table
// don't know why I was despacing qwords and not owords...
size_t despace_ssse3_cumsum( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const __m128i is_3or7 = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	const __m128i lut_cntrl = _mm_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00);
	const __m128i id = _mm_setr_epi8(
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F);
	const __m128i mask_02 = _mm_set1_epi8(0x02);
	const __m128i mask_04 = _mm_add_epi8(mask_02, mask_02);
	const __m128i mask_20 = _mm_slli_epi64(mask_02, 4);
	const __m128i mask_70 = _mm_set1_epi8(0x70);

	for( uint8_t* end = &src[(length & ~15)]; src != end; src += 16){
		__m128i a,b,c,d,s,t,v;
		size_t cnt0, cnt1;

		// load
		v = _mm_loadu_si128((__m128i*)src);

		// detect spaces ( 0x01 == space, 0x00 == non-space )
		s = _mm_or_si128(_mm_abs_epi8(_mm_cmpeq_epi8(mask_20, v)),
			_mm_shuffle_epi8(lut_cntrl, _mm_adds_epu8(mask_70, v)));

		// create non-space mask ( 0x00 == space, 0xFF == non-space )
		b = _mm_cmpeq_epi8(_mm_setzero_si128(), s);

		// (qword) prefix sum of spaces
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 8));
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 16));
		s = _mm_add_epi8(s, _mm_slli_epi64(s, 32));

		// get non-space byte totals
		t = _mm_srli_epi64(s, 56); // hi-byte is total_spaces
		cnt0 = (uint32_t)_mm_cvtsi128_si32(t);
		t = _mm_unpackhi_epi64(t, t);
		cnt1 = (uint32_t)_mm_cvtsi128_si32(t);

		// compress
		b = _mm_andnot_si128(b, s); // zero non-spaces
		//
		c = _mm_srli_epi64(_mm_and_si128(mask_02, b), 9);
		d = _mm_srli_epi64(_mm_shuffle_epi8(is_3or7, b), 16);
		a = _mm_or_si128(_mm_cmpgt_epi8(mask_04, s), _mm_cmpeq_epi8(b, mask_04)); // match first 4 and below
		//
		s = _mm_add_epi8(s, c);
		s = _mm_add_epi8(s, d);
		//
		s = _mm_max_epu8(s, _mm_srli_epi64(_mm_andnot_si128(a, s), 32));
		v = _mm_shuffle_epi8(v, _mm_add_epi8(s, id));

		// store
		_mm_storel_epi64((__m128i*)dst, v);
		dst += 8 - cnt0;
		_mm_storel_epi64((__m128i*)dst, _mm_unpackhi_epi64(v, v));
		dst += 8 - cnt1;
	}
	dst += despace_branchless(dst, src, length & 15);
	return (size_t)(dst - ((uint8_t*)dst_void));
}


size_t despace_sse41_lut_216 (void* dst_void, void* src_void, size_t length)
{
    uint8_t* src = (uint8_t*)src_void;
    uint8_t* dst = (uint8_t*)dst_void;

    if (length >= 16) {
        // table of control characters (space, tab, newline, carriage return)
        const __m128i lut_cntrl = _mm_setr_epi8(' ', 0, 0, 0, 0, 0, 0, 0, 0, '\t', '\n', 0, 0, '\r', 0, 0);

        // bits[4:0] = index -> ((trit_d * 0) + (trit_c * 9) + (trit_b * 3) + (trit_a * 1))
        // bits[15:7] = popcnt
        const __m128i sadmask = _mm_set1_epi64x(0x8080898983838181);

        // adding 8 to each shuffle index is cheaper than extracting the high qword
        const __m128i offset = _mm_cvtsi64_si128(0x0808080808080808);

        // shuffle control indices
        static const uint64_t table[27] = {
            0x0000000000000706, 0x0000000000070600, 0x0000000007060100, 0x0000000000070602,
            0x0000000007060200, 0x0000000706020100, 0x0000000007060302, 0x0000000706030200,
            0x0000070603020100, 0x0000000000070604, 0x0000000007060400, 0x0000000706040100,
            0x0000000007060402, 0x0000000706040200, 0x0000070604020100, 0x0000000706040302,
            0x0000070604030200, 0x0007060403020100, 0x0000000007060504, 0x0000000706050400,
            0x0000070605040100, 0x0000000706050402, 0x0000070605040200, 0x0007060504020100,
            0x0000070605040302, 0x0007060504030200, 0x0706050403020100
        };

        const uint8_t* end = &src[length & ~15];
        do {
            __m128i v = _mm_loadu_si128((__m128i*)src);
            src += 16;

            // detect spaces
            __m128i mask = _mm_cmpeq_epi8(_mm_shuffle_epi8(lut_cntrl, v), v);

            // shift w/blend: each word now only has 3 states instead of 4
            // which takes us from 128 possiblities, per qword, down to 27
            v = _mm_blendv_epi8(v, _mm_srli_epi16(v, 8), mask);

            // extract bitfields describing each qword: index, popcnt
            __m128i desc = _mm_sad_epu8(_mm_and_si128(mask, sadmask), sadmask);
            size_t lo_desc = (size_t)_mm_cvtsi128_si32(desc);
            size_t hi_desc = (size_t)_mm_extract_epi16(desc, 4);

            // load shuffle control indices from pre-computed table
            __m128i lo_shuf = _mm_loadl_epi64((__m128i*)&table[lo_desc & 0x1F]);
            __m128i hi_shuf = _mm_or_si128(_mm_loadl_epi64((__m128i*)&table[hi_desc & 0x1F]), offset);

            // store an entire qword then advance the pointer by how ever
            // many of those bytes are actually wanted. Any trailing
            // garbage will be overwritten by the next store.
            // note: little endian byte memory order
            _mm_storel_epi64((__m128i*)dst, _mm_shuffle_epi8(v, lo_shuf));
            dst += (lo_desc >> 7);
            _mm_storel_epi64((__m128i*)dst, _mm_shuffle_epi8(v, hi_shuf));
            dst += (hi_desc >> 7);
        } while (src != end);
    }

    // tail loop
    length &= 15;
    if (length != 0) {
        const uint64_t bitmap = 0xFFFFFFFEFFFFC1FF;
        do {
            uint64_t c = *src++;
            *dst = (uint8_t)c;
            dst += ((bitmap >> c) & 1) | ((c + 0xC0) >> 8);
        } while (--length);
    }

    // return pointer to the location after the last element in dst
    return (size_t)(dst - ((uint8_t*)dst_void));
}


// no lookup tables
// probably needs improvment...
size_t despace_avx2_vpermd( void* dst_void, void* src_void, size_t length )
{
	uint8_t* src = (uint8_t*)src_void;
	uint8_t* dst = (uint8_t*)dst_void;

	const __m256i mask_20  = _mm256_set1_epi8( 0x20 );
	const __m256i mask_70  = _mm256_set1_epi8( 0x70 );
	const __m256i lut_cntrl = _mm256_setr_epi8(
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00,
		//
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00
	);

	const __m256i permutation_mask = _mm256_set1_epi64x( 0x0020100884828180 );
	const __m256i invert_mask = _mm256_set1_epi64x( 0x0020100880808080 ); 
	const __m256i zero = _mm256_setzero_si256();
	const __m256i fixup = _mm256_set_epi32(
		0x08080808, 0x0F0F0F0F, 0x00000000, 0x07070707,
		0x08080808, 0x0F0F0F0F, 0x00000000, 0x07070707
	);
	const __m256i lut = _mm256_set_epi32(
		0x04050607, // 0x03020100', 0x000000'07
		0x04050704, // 0x030200'00, 0x0000'0704
		0x04060705, // 0x030100'00, 0x0000'0705
		0x04070504, // 0x0300'0000, 0x00'070504
		0x05060706, // 0x020100'00, 0x0000'0706
		0x05070604, // 0x0200'0000, 0x00'070604
		0x06070605, // 0x0100'0000, 0x00'070605
		0x07060504  // 0x00'000000, 0x'07060504
	);

	// hi bits are ignored by pshufb, used to reject movement of low qword bytes
	const __m256i shuffle_a = _mm256_set_epi8(
		0x7F, 0x7E, 0x7D, 0x7C, 0x7B, 0x7A, 0x79, 0x78, 0x07, 0x16, 0x25, 0x34, 0x43, 0x52, 0x61, 0x70,
		0x7F, 0x7E, 0x7D, 0x7C, 0x7B, 0x7A, 0x79, 0x78, 0x07, 0x16, 0x25, 0x34, 0x43, 0x52, 0x61, 0x70
	);

	// broadcast 0x08 then blendd...
	const __m256i shuffle_b = _mm256_set_epi32(
		0x08080808, 0x08080808, 0x00000000, 0x00000000,
		0x08080808, 0x08080808, 0x00000000, 0x00000000
	);

	for( uint8_t* end = &src[(length & ~31)]; src != end; src += 32){
		__m256i r0,r1,r2,r3,r4;
		unsigned int s0,s1;

		r0 = _mm256_loadu_si256((__m256i *)src); // asrc

		r1 = _mm256_adds_epu8(mask_70, r0);
		r2 = _mm256_cmpeq_epi8(mask_20, r0);

		r1 = _mm256_shuffle_epi8(lut_cntrl, r1);

		r1 = _mm256_or_si256(r1, r2); // bytemask of spaces

		r2 = _mm256_sad_epu8(zero, r1);
		s0 = _mm256_movemask_epi8(r1);
		r1 = _mm256_andnot_si256(r1, permutation_mask);

		r1 = _mm256_sad_epu8(r1, invert_mask); // index_bitmap[0:5], low32_spaces_count[7:15]

		r2 = _mm256_shuffle_epi8(r2, zero);

		r2 = _mm256_sub_epi8(shuffle_a, r2); // add space cnt of low qword
		s0 = ~s0;

		r3 = _mm256_slli_epi64(r1, 29); // move top part of index_bitmap to high dword
		r4 = _mm256_srli_epi64(r1, 7); // number of spaces in low dword 

		r4 = _mm256_shuffle_epi8(r4, shuffle_b);
		r1 = _mm256_or_si256(r1, r3);

		r1 = _mm256_permutevar8x32_epi32(lut, r1);
		s1 = _mm_popcnt_u32(s0);
		r4 = _mm256_add_epi8(r4, shuffle_a);
		s0 = s0 & 0xFFFF; // isolate low oword

		r2 = _mm256_shuffle_epi8(r4, r2);
		s0 = _mm_popcnt_u32(s0);

		r2 = _mm256_max_epu8(r2, r4); // pin low qword bytes

		r1 = _mm256_xor_si256(r1, fixup);

		r1 = _mm256_shuffle_epi8(r1, r2); // complete shuffle mask

		r0 = _mm256_shuffle_epi8(r0, r1); // despace!

		_mm_storeu_si128((__m128i*)dst, _mm256_castsi256_si128(r0));
		_mm_storeu_si128((__m128i*)&dst[s0], _mm256_extracti128_si256(r0,1));
		dst += s1;
	}
	dst += despace_branchless(dst, src, length & 31);
	return (size_t)(dst - ((uint8_t*)dst_void));
}
