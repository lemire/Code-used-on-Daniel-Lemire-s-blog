#ifndef AVX512IP_H
#define AVX512IP_H

#include <x86intrin.h>

/**
 * AVX-512VL and AVX-512BW based parsing of IPv4 and IPv6 addresses respectively.
 * credit to Shreesh Adiga.
 *
 * Daniel Lemire, "Parsing IP addresses crazily fast," in Daniel Lemire's blog,
 * June 8, 2023, https://lemire.me/blog/2023/06/08/parsing-ip-addresses-crazily-fast/.
 */


/**
 * Parses an IPv4 address in dotted-decimal form (e.g., "192.168.0.1")
 * using an AVX-512VL SIMD path.
 *
 * @param input Pointer to the input string (NUL termination is not required).
 * @param len Length of the input string.
 * @param ptr Output pointer to a uint32_t receiving the IPv4 value in host order.
 * @return 1 on success, 0 if the format is invalid.
 */
static int parse_ipv4_avx512vl(const char *input, size_t len, uint32_t *ptr) {
    if (len > 15) [[unlikely]] { return 0; }
    int error = 0;
    __mmask16 len_mask = (1 << len) - 1;
    __m128i dot = _mm_set1_epi8('.');
    __m128i str = _mm_mask_loadu_epi8(dot, len_mask, (const __m128i *)input);
    __mmask16 dots_bitvector = _mm_cmpeq_epu8_mask(str, dot);
    __m128i digits_vec = _mm_sub_epi8(str, _mm_set1_epi8('0'));
    // string should have only 3 dots
    error |= (_mm_popcnt_u32(dots_bitvector & len_mask) != 3);
    __mmask16 copy_mask = ~dots_bitvector;
    __m128i index_reg = _mm_setr_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    // obtain the indices corresponding to location of the dots
    __m128i compressed_index = _mm_maskz_compress_epi8(dots_bitvector, index_reg);
    // 4 32-bit integers (zero-extended) a0, a1, a2, a3 where a_i indicates the location (1-16) of the i_th dot in input string
    __m128i dot_location_epi32 = _mm_cvtepu8_epi32(compressed_index);
    // 4 32-bit integers 0, a0, a1, a2
    __m128i dot_location_shifted = _mm_bslli_si128(dot_location_epi32, 4);
    // compute a0 - 0, a1 - a0, a2 - a1, a3 - a2 which gives number of digits between dots (including dot)
    __m128i difference = _mm_sub_epi32(dot_location_epi32, dot_location_shifted);
    // need to subtract 1 to exclude dot and get in-between digits count
    __m128i num_digits_between_dots = _mm_sub_epi32(difference, _mm_set1_epi32(1));
    // error if num_digits is not between 1 and 3
    // using unsigned ((x - 1) > 2) for error expression instead of two compares
    error |= _mm_cmpgt_epu8_mask(_mm_sub_epi32(num_digits_between_dots, _mm_set1_epi32(0x1)), _mm_set1_epi32(0x2));
    __m128i expand_mask_creation_register = _mm_setr_epi8(
            0,    0,    0,    0,
            0,    0,    0, 0xff,
            0,    0, 0xff, 0xff,
            0, 0xff, 0xff, 0xff
    );

    // _mm_permutevar_ps is required to index into the expand_mask_creation_register
    // _mm_movepi8_mask for each 32 bit integer will have the bitmask for zero padding necessary per octet.
    __mmask16 expand_mask = _mm_movepi8_mask(
            _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(expand_mask_creation_register), num_digits_between_dots)));

    // get rid of '.' and compress digits
    __m128i compressed_vec = _mm_maskz_compress_epi8(copy_mask, digits_vec);
    // all entries must be less than 10
    error |= _mm_cmpgt_epu8_mask(compressed_vec, _mm_set1_epi8(0x9));
    // expand the compressed_vec to have all octets with 4 digits (zero padding)
    __m128i padded_digits_vec = _mm_maskz_expand_epi8(expand_mask, compressed_vec);

    // Error if the leading digit is 0 e.g. 02, 012 etc will be treated as error.
    // Extract the most significant digit from num_digits_between_dots and check if it is 0.
    // If there is only 1 digit then skip the check as zero is valid number
    __m128i shifted_vec = _mm_shuffle_epi8(padded_digits_vec,
            _mm_sub_epi32(
                _mm_setr_epi8(4, 0x0, 0x0, 0x0, 8, 0x0, 0x0, 0x0, 12, 0x0, 0x0, 0x0, 16, 0x0, 0x0, 0x0),
                num_digits_between_dots));
    __mmask8 more_than_1digit = _mm_cmpgt_epu32_mask(_mm_sub_epi32(num_digits_between_dots, _mm_set1_epi32(0x1)), _mm_setzero_si128());
    error |= _mm_mask_cmpeq_epu32_mask(more_than_1digit, _mm_setzero_si128(), shifted_vec);
    // multiply digits by 100, 10, 1 respectively and sum them to a 32 bit num
    __m128i res = _mm_dpbusd_epi32(_mm_setzero_si128(), padded_digits_vec, _mm_set1_epi32(0x010a6400));
    // error if any of the 4 octet is bigger than 255
    error |= _mm_cmpgt_epu32_mask(res, _mm_set1_epi32(0xff));

    if (!error) [[likely]] {
        // assemble the bytes and convert from network order to host order and write to memory
        _mm_storeu_si32(ptr, _mm_shuffle_epi8(res, _mm_setr_epi8(0, 4, 8, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)));
    }

    return !error;
}

/**
 * Parses an IPv6 address (full, compressed, or IPv4-suffixed form) using AVX-512.
 *
 * The validated address is written as 16 bytes to the output buffer.
 *
 * @param input Pointer to the input string (NUL termination is not required).
 * @param len Length of the input string.
 * @param ptr Output pointer to 16 bytes receiving the parsed address.
 * @return 1 on success, 0 if the format is invalid.
 */
static int parse_ipv6_avx512(const char *input, size_t len, uint8_t *ptr) {
    if (len > 45 || len < 2) [[unlikely]] { return 0; }
    uint64_t error = 0;
    __mmask64 len_mask = (1ULL << len) - 1;
    __m512i colon = _mm512_set1_epi8(':');
    __m512i str = _mm512_mask_loadu_epi8(colon, len_mask, (const __m512i *)input);
    __mmask64 colons_bitvector = _mm512_cmpeq_epu8_mask(str, colon);
    __m512i index_reg = _mm512_set_epi8(
            64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
            48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,
            32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,
            16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
    __m512i compressed_index = _mm512_maskz_compress_epi8(colons_bitvector, index_reg);
    colons_bitvector &= len_mask;
    // 16 32-bit integers (zero-extended) a0, a1, a2, a3 ... where a_i indicates the location of the i_th colon in input string
    __m512i colon_location_epi32 = _mm512_cvtepu8_epi32(_mm512_castsi512_si128(compressed_index));
    // 16 32-bit integers 0, a0, a1, a2 ...
    __m512i colon_location_shifted = _mm512_alignr_epi32(colon_location_epi32, _mm512_setzero_si512(), 15);
    // compute a0 - 0, a1 - a0, a2 - a1, a3 - a2 which gives number of digits between colon (including colon)
    __m512i difference = _mm512_sub_epi32(colon_location_epi32, colon_location_shifted);
    // need to subtract 1 to exclude colons and get in-between digits count
    __m512i num_digits_between_colons = _mm512_sub_epi32(difference, _mm512_set1_epi32(1));
    __mmask16 is_zero = _mm512_cmpeq_epi32_mask(num_digits_between_colons, _mm512_setzero_si512());

    uint64_t doublecolon = ((colons_bitvector) >> 1) & colons_bitvector;
    uint8_t num_colons = _mm_popcnt_u64(colons_bitvector);

    // error if more than 1 consecutive colon in the input
    // blsr is 0 if only one bit is set
    error |= _blsr_u64(doublecolon);

    __m512i dot = _mm512_set1_epi8('.');
    __mmask64 dots_bitvector = _mm512_cmpeq_epu8_mask(str, dot);
    int dots_present = (dots_bitvector > 0);

    // input_mask will have the mask upto ipv6 part and exclude the ipv4 part for validation and computation
    __mmask64 input_mask;
    if (dots_present) {
        input_mask = (((__mmask64)1) << (64 - _lzcnt_u64(colons_bitvector))) - 1;
    } else {
        input_mask = len_mask;
    }

    __m512i lookup_table_lo = _mm512_set_epi8(
            -1, -1, -1, -1, -1, -1, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
            );
    __m512i lookup_table_hi = _mm512_set_epi8(
-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, 14, 13, 12, 11, 10, -1
            );
    error |= _mm512_movepi8_mask(str);

    __mmask64 copy_mask = input_mask & ~colons_bitvector;
    // translate hex even for ipv4 part so that we can skip subtracting '0' in ipv4 validation later
    str = _mm512_permutex2var_epi8(lookup_table_lo, str, lookup_table_hi);
    error |= (_mm512_movepi8_mask(str) & copy_mask);
    // get rid of ':' and compress digits
    __m512i compressed_vec = _mm512_maskz_compress_epi8(copy_mask, str);
    // then optionally before expanding create the expand mask for double colon
    __m256i expand_mask_creation_register = _mm256_setr_epi8(
            0,       0,    0,    0,
            0,       0,    0, 0xff,
            0,       0, 0xff, 0xff,
            0,    0xff, 0xff, 0xff,
            0xff, 0xff, 0xff, 0xff,
            0,       0,    0,    0,
            0,       0,    0,    0,
            0,       0,    0,    0
    );

    if (doublecolon) [[likely]] {
        // for doublecolon case, we need to have the num_digits_between_colons expanded so that it has 8 octets (or 6 if ipv4 present)
        int idx = _tzcnt_u32(is_zero);
        int num_padding_octets = dots_present ? 7 : 8;
        num_padding_octets -= (num_colons == num_padding_octets ? num_padding_octets : num_colons + 1);
        int mask_ = (1 << num_padding_octets) - 1;
        mask_ <<= (idx + 1);
        mask_ = ~mask_;
        num_digits_between_colons = _mm512_maskz_expand_epi32(mask_, num_digits_between_colons);
        int num_colons_mask = (1 << (num_colons + 1)) - 1;
        is_zero &= num_colons_mask;
        // validate is_zero to be contiguous 1s and 0s
        error |= (_blsr_u32(is_zero + _blsi_u32(is_zero)) != 0);
        __m512i num_digits_between_colons_shifted = _mm512_alignr_epi32(_mm512_setzero_si512(), num_digits_between_colons, 1);
        __mmask16 vec_select = ((doublecolon & 1) && num_colons == 7 + !dots_present) ? 0xffff : 0;
        num_digits_between_colons = _mm512_mask_mov_epi32(num_digits_between_colons, vec_select, num_digits_between_colons_shifted);
        int double_colon_start_or_end = (doublecolon | (doublecolon >> (len - 2))) & 1;
        error |= (num_colons > 6 + !dots_present + double_colon_start_or_end); // number of colons can be maximum 8 if double colon at start or end, otherwise max 7
    } else {
        // simple case without double colon
        // need to have 7 colons and 8 octets. Or have 6 colons and ipv4 at the end
        error |= (num_colons + dots_present != 7);

        // num colons will be 6 or 7, check those many bits of is_zero to ensure non-zero number of digits in each octet
        error |= (is_zero & ((1 << (num_colons + 1)) - 1));
    }

    // error if more than 4 digits between colons in the first 6 octets excluding ipv4
    int octet_mask = dots_present ? 0x3f : 0xff;
    error |= (_mm512_cmpgt_epu32_mask(num_digits_between_colons, _mm512_set1_epi32(4)) & octet_mask);

    __mmask32 expand_mask = _mm256_movepi8_mask(_mm256_permutevar8x32_epi32(expand_mask_creation_register, _mm512_castsi512_si256(num_digits_between_colons)));
    __m256i padded_digits_vec = _mm256_maskz_expand_epi8(expand_mask, _mm512_castsi512_si256(compressed_vec));
    __m256i res = _mm256_maddubs_epi16(padded_digits_vec, _mm256_set1_epi16(0x0110));
    __m128i output = _mm256_cvtepi16_epi8(res);

    if (dots_present) {
        // this follows the ipv4 validation logic from above function.
        error |= (_mm_popcnt_u64(dots_bitvector) != 3);
        dots_bitvector |= ~len_mask;
        copy_mask = ~(dots_bitvector | input_mask);
        __m512i ipv4_compressed_index = _mm512_maskz_compress_epi8(dots_bitvector, index_reg);
        __m128i dot_location_epi32 = _mm_cvtepu8_epi32(_mm512_castsi512_si128(ipv4_compressed_index));
        __m128i dot_location_shifted = _mm_bslli_si128(dot_location_epi32, 4);
        dot_location_shifted = _mm_insert_epi32(dot_location_shifted, _mm_popcnt_u64(input_mask), 0);
        __m128i difference = _mm_sub_epi32(dot_location_epi32, dot_location_shifted);
        __m128i num_digits_between_dots = _mm_sub_epi32(difference, _mm_set1_epi32(1));
        error |= _mm_cmpgt_epu8_mask(_mm_sub_epi32(num_digits_between_dots, _mm_set1_epi32(0x1)), _mm_set1_epi32(0x2));
        __mmask16 expand_mask = _mm_movepi8_mask(
                _mm_castps_si128(_mm_permutevar_ps(_mm_castsi128_ps(_mm256_castsi256_si128(expand_mask_creation_register)), num_digits_between_dots)));

        __m512i compressed_vec = _mm512_maskz_compress_epi8(copy_mask, str);
        error |= _mm_cmpgt_epu8_mask(_mm512_castsi512_si128(compressed_vec), _mm_set1_epi8(0x9));
        __m128i padded_digits_vec = _mm_maskz_expand_epi8(expand_mask, _mm512_castsi512_si128(compressed_vec));

        __m128i shifted_vec = _mm_shuffle_epi8(padded_digits_vec,
                _mm_sub_epi32(_mm_setr_epi8(4, 0x0, 0x0, 0x0,
                        8, 0x0, 0x0, 0x0,
                        12, 0x0, 0x0, 0x0,
                        16, 0x0, 0x0, 0x0), num_digits_between_dots));
        __mmask8 more_than_1digit = _mm_cmpgt_epu32_mask(_mm_sub_epi32(num_digits_between_dots, _mm_set1_epi32(0x1)),
                _mm_setzero_si128());
        error |= _mm_mask_cmpeq_epu32_mask(more_than_1digit, _mm_setzero_si128(), shifted_vec);

        __m128i res = _mm_dpbusd_epi32(_mm_setzero_si128(), padded_digits_vec, _mm_set1_epi32(0x010a6400));
        error |= _mm_cmpgt_epu32_mask(res, _mm_set1_epi32(0xff));
        // move the 32 bit ipv4 value to upper word of xmm register to merge with output
        __m128i ipv4_output = _mm_shuffle_epi8(res, _mm_setr_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 12));
        output = _mm_mask_mov_epi32(ipv4_output, 0x7, output);
    }

    if (!error) [[likely]] {
        _mm_storeu_si128((__m128i *)ptr, output);
    }

    return !error;
}

#endif