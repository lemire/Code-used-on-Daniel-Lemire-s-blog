#include <cstddef>
#include <cstdio>
#include <x86intrin.h>
#include <cstdint>
#include <cstring>
#include <cassert>

// Precomputed shuffle masks for K = 1 to 16
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
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0x80} };


__m128i insert_line_feed(__m128i input, int K) {
    // Prepare a vector with '\n' (0x0A) 
    __m128i line_feed_vector = _mm_set1_epi8('\n');

    // Load the precomputed shuffle mask for K (index K-1)
    __m128i mask = _mm_loadu_si128((__m128i*)shuffle_masks[K]);
    __m128i lf_pos = _mm_cmpeq_epi8(mask, _mm_set1_epi8(0x80));

    // Perform the shuffle to reposition the K bytes
    __m128i shuffled = _mm_shuffle_epi8(input, mask);

    // Blend with line_feed_vector to insert '\n' at the appropriate positions
    __m128i result = _mm_blendv_epi8(shuffled, line_feed_vector, lf_pos);

    return result;
}


void print(__m128i x) {
  alignas(16) char buffer[16];
  _mm_storeu_si128((__m128i *)buffer, x);
  for (int i = 0; i < 16; ++i) {
    printf("%c", buffer[i]);
  }
}
void print32(__m256i x) {
  alignas(32) char buffer[32];
  _mm256_storeu_si256((__m256i *)buffer, x);
  for (int i = 0; i < 32; ++i) {
    if(buffer[i] ==0) {printf("0"); continue;}
    printf("%c", buffer[i]);
    if(i == 15) printf(" | ");
  }
}
__m256i insert_line_feed32(__m256i input, int K) {
  __m256i line_feed_vector = _mm256_set1_epi8('\n');
  __m128i identity = _mm_setr_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  if(K >= 16)  {
    __m128i maskhi = _mm_loadu_si128((__m128i*)shuffle_masks[K - 16]);

    __m256i mask = _mm256_set_m128i(maskhi, identity);
    __m256i lf_pos = _mm256_cmpeq_epi8(mask, _mm256_set1_epi8(0x80));
    __m256i shuffled = _mm256_shuffle_epi8(input, mask);
    __m256i result = _mm256_blendv_epi8(shuffled, line_feed_vector, lf_pos);
    return result;
  }
  // Shift input right by 1 byte
  __m256i shift =  _mm256_alignr_epi8(input, _mm256_permute2x128_si256(input, input, 0x21), 15);
  input = _mm256_blend_epi32(input, shift, 0xF0);
  __m128i masklo = _mm_loadu_si128((__m128i*)shuffle_masks[K]);
  __m256i mask = _mm256_set_m128i(identity, masklo);
  __m256i lf_pos = _mm256_cmpeq_epi8(mask, _mm256_set1_epi8(0x80));
  __m256i shuffled = _mm256_shuffle_epi8(input, mask);
  __m256i result = _mm256_blendv_epi8(shuffled, line_feed_vector, lf_pos);
  return result;
}

void insert_line_feed(const char *buffer, size_t length, int K, char *output) {
  if(K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  size_t next_line_feed = K;
  if(K >= 31){
    while(input_pos + 15 <= length) {
      __m128i chunk = _mm_loadu_si128((__m128i *)(buffer + input_pos));
      if(next_line_feed >= 32) {
        _mm_storeu_si128((__m128i *)(output), chunk);
        output += 16;
        next_line_feed -= 16;
        input_pos += 16;
      } else {
        // we write next_line_feed bytes, then a line feed, then the rest (31 - next_line_feed bytes)
        chunk = insert_line_feed(chunk, next_line_feed);
        _mm_storeu_si128((__m128i *)(output), chunk);
        output += 16;
        next_line_feed =  K - (15 - next_line_feed);
        input_pos += 15;
      }
    }
  }
}

void insert_line_feed32(const char *buffer, size_t length, int K, char *output) {
  if(K == 0) {
    memcpy(output, buffer, length);
    return;
  }
  size_t input_pos = 0;
  size_t next_line_feed = K;
  if(K >= 31){
    while(input_pos + 32 <= length) {
      __m256i chunk = _mm256_loadu_si256((__m256i *)(buffer + input_pos));
      if(next_line_feed >= 32) {
        _mm256_storeu_si256((__m256i *)(output), chunk);
        output += 32;
        next_line_feed -= 32;
        input_pos += 32;
      } else {
        // we write next_line_feed bytes, then a line feed, then the rest (31 - next_line_feed bytes)
        chunk = insert_line_feed32(chunk, next_line_feed);
        _mm256_storeu_si256((__m256i *)(output), chunk);
        output += 32;
        next_line_feed =  K - (31 - next_line_feed);
        input_pos += 31;
      }
    }
  }
  while(input_pos < length) {
    output[0] = buffer[input_pos];
    output++;
    input_pos++;
    next_line_feed--;
    if(next_line_feed == 0) {
      output[0] = '\n';
      output++;
      next_line_feed = K;
    }
  }
}


int main(const int argc, const char **argv) {
  (void)argc;
  (void)argv;
  size_t N = 1000;
    char buffer[N];
  for(size_t i = 0; i < N; ++i) {
    buffer[i] = 'A'+(i%26);
  }
  char obuffer [2*N];
  for(size_t k = 1; k < 100; ++k) {
    for (int i = 0; i < k; i++) {
        printf("-");
    }
    printf("\n");
    insert_line_feed(buffer, N, k, obuffer);
      printf("%.*s", (int)(N), obuffer);
    printf("\n");
  }
 return 0;
}