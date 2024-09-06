#ifndef AVX512COMPRESS_H
#define AVX512COMPRESS_H

#include <immintrin.h>
#include <cstddef>
#include <cstdint>

void to_float16(uint16_t *dst, const double *src, size_t length) {
    size_t i = 0;
    __mmask8 mask;

    // Process 8 elements at a time
    for (; i + 7 < length; i += 8) {
        // Load 8 double-precision floats
        __m512d src_vec = _mm512_loadu_pd(&src[i]);

        // Convert to 16-bit floats with rounding
        __m128bh dst_vec = _mm256_cvtneps_pbh(_mm512_cvt_roundpd_ps(src_vec, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC));

        // Store the result
        _mm_storeu_si128((__m128i*)&dst[i], *(__m128i*)&dst_vec);
    }

    // Handle remaining elements
    if (i < length) {
        // Create a mask for the remaining elements
        mask = (1 << (length - i)) - 1;

        // Load remaining double-precision floats
        __m512d src_vec = _mm512_maskz_loadu_pd(mask, &src[i]);

        // Convert to 16-bit floats with rounding
        __m128bh dst_vec = _mm256_cvtneps_pbh(_mm512_cvt_roundpd_ps(src_vec, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC));

        // Store the result with masking
        _mm_mask_storeu_epi16(&dst[i], mask, *(__m128i*)&dst_vec);
    }
}


void from_float16(double *dst, const uint16_t *src, size_t length) {
    size_t i = 0;
    __mmask8 mask;

    // Process 8 elements at a time
    for (; i + 7 < length; i += 8) {
        // Load 8 half-precision floats
        __m128i src_vec = _mm_loadu_si128((__m128i*)&src[i]);

        // Convert to double-precision floats
        __m512d dst_vec = _mm512_cvtps_pd(_mm256_cvtpbh_ps(*(__m128bh*)&src_vec));

        // Store the result
        _mm512_storeu_pd(&dst[i], dst_vec);
    }

    // Handle remaining elements
    if (i < length) {
        // Create a mask for the remaining elements
        mask = (1 << (length - i)) - 1;

        // Load remaining half-precision floats
        __m128i src_vec = _mm_maskz_loadu_epi16(mask, &src[i]);

        // Convert to double-precision floats
        __m512d dst_vec = _mm512_cvtps_pd(_mm256_cvtpbh_ps( *(__m128bh*)&src_vec));

        // Store the result with masking
        _mm512_mask_storeu_pd(&dst[i], mask, dst_vec);
    }
}

#endif
