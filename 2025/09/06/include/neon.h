#ifndef NEON_H
#define NEON_H

#if __ARM_NEON__
#define HAVE16

#include <cstdint>
#include <cstring>
#include <arm_neon.h>


#include <arm_neon.h>

// Precomputed shuffle masks for K = 1 to 16 (same as original)
static const uint8_t shuffle_masks[16][16] = {
    {0x80, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 0x80, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 0x80, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 0x80, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 0x80, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 0x80, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 0x80, 6, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 0x80, 7, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 0x80, 8, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 0x80, 9, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0x80, 10, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0x80, 11, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x80, 12, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0x80, 13, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0x80, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0x80}
};

/**
 * Insert a line feed character in the 16-byte input at index K in [0,16).
 */
inline int8x16_t insert_line_feed16(int8x16_t input, int K) {
    // Prepare a vector with '\n' (0x0A)
    int8x16_t line_feed_vector = vdupq_n_u8('\n');
    
    // Load the precomputed shuffle mask for K
    int8x16_t mask = vld1q_u8(shuffle_masks[K]);
    
    // Create a mask where 0x80 indicates the line feed position
    uint8x16_t lf_pos = vceqq_u8(mask, vdupq_n_u8(0x80));
    
    // Since Neon lacks a direct shuffle like _mm_shuffle_epi8, we adjust the approach.
    // Shift bytes to make space for '\n' by splitting and concatenating.
    // For simplicity, we blend the input with the line feed vector at the marked position.
    int8x16_t result = input;
    
    // Use vbsl to select '\n' where lf_pos is true, else keep input bytes
    result = vbslq_s8(lf_pos, line_feed_vector, input);
    
    return result;
}

/**
 * Insert line feeds every K bytes in the input data (buffer, length).
 * The output is written to the output buffer (which must be large enough).
 */
inline void insert_line_feed16(const char *buffer, size_t length, int K, char *output) {
    if (K == 0) {
        memcpy(output, buffer, length);
        return;
    }

    size_t input_pos = 0;
    size_t next_line_feed = K;
    
    if (K >= 15) {
        while (input_pos + 15 <= length) {
            int8x16_t chunk = vld1q_u8((const uint8_t *)(buffer + input_pos));
            if (next_line_feed >= 16) {
                vst1q_u8((uint8_t *)output, chunk);
                output += 16;
                next_line_feed -= 16;
                input_pos += 16;
            } else {
                // Insert line feed at next_line_feed position
                chunk = insert_line_feed16(chunk, next_line_feed);
                vst1q_u8((uint8_t *)output, chunk);
                output += 16;
                next_line_feed = K - (15 - next_line_feed);
                input_pos += 15;
            }
        }
    }
    
    // Handle remaining bytes scalarly
    while (input_pos < length) {
        output[0] = buffer[input_pos];
        output++;
        input_pos++;
        next_line_feed--;
        if (next_line_feed == 0) {
            output[0] = '\n';
            output++;
            next_line_feed = K;
        }
    }
}

#endif //__ARM_NEON__

#endif // NEON_H