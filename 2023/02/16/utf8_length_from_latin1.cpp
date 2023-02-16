

size_t scalar_utf8_length(const char * c, size_t len) {
    size_t answer = 0;
    for(size_t i = 0; i<len; i++) {
        if((c[i]>>7)) { answer++;} 
    }
    return answer + len;
}



size_t avx2_utf8_length_basic(const uint8_t* str, size_t len) {
  size_t answer = len / sizeof(__m256i) * sizeof(__m256i);
  size_t i;
  for (i = 0; i + sizeof(__m256i) <= len; i += 32) {
    __m256i input = _mm256_loadu_si256((const __m256i*)(str + i));
    answer += __builtin_popcount(_mm256_movemask_epi8(input));
  }
  return answer + scalar_utf8_length(str + i, len - i);
}

/**
 * Similar to :
 * Wojciech Muła, Nathan Kurz, Daniel Lemire
 * Faster Population Counts Using AVX2 Instructions
 * Computer Journal 61 (1), 2018
 **/
size_t avx2_utf8_length(const uint8_t* str, size_t len) {
  size_t answer = len / sizeof(__m256i) * sizeof(__m256i);
  __m256i m80 = _mm256_set1_epi8(0x80);
  __m256i m7f = _mm256_set1_epi8(0x7f);

  size_t i = 0;
  __m256i four_64bits = _mm256_setzero_si256();
  
  while(i + sizeof(__m256i) <= len) {
    __m256i runner = _mm256_setzero_si256();
    // We can do up to 255 loops without overflow.
    size_t iterations = (len - i) / sizeof(__m256i);
    if(iterations > 255) { iterations = 255; }
    size_t max_i = i + iterations;
    for(; i <= maxi; i++ ) {
        __m256i input = _mm256_loadu_si256((const __m256i*)(str + i));
        __m256i msb = _mm256_subs_epu8(_mm256_and_si256(input, m80), m7f);
        runner = _mm256_add_epi8(runner, msb);
    }
    four_64bits = _mm256_add_epu64(four_64bits, _mm256_sad_epu8(runner, _mm256_setzero_si256()));
  }
  answer += _mm256_extract_epi64(acc, 1) + _mm256_extract_epi64(acc, 1) + _mm256_extract_epi64(acc, 2) + _mm256_extract_epi64(acc, 3);
  return answer + scalar_utf8_length(str + i, len - i);
}