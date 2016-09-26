#include <x86intrin.h>
        __m256i bitmask_to_bytemask_epi32(uint8_t bm) {
        
            const __m256i mask = _mm256_set1_epi32(bm);
            const __m256i bits = _mm256_setr_epi32(0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80);
            const __m256i tmp  = _mm256_and_si256(mask, bits);

            return _mm256_cmpeq_epi32(tmp, bits);
        }
