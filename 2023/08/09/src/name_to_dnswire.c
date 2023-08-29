
#include "name_to_dnswire.h"

#include <stdint.h>
#include <x86intrin.h> // update if we need to support Windows.

size_t name_to_dnswire(const char *src, uint8_t *dst) {
  const char *srcinit = src;

  static const bool delimiter[256] = {
      1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t *counter = dst++;
  do {
    while (!delimiter[(uint8_t)*src]) {
      *dst++ = (uint8_t)*src;
      src++;
    }
    if (dst == counter || dst - counter - 1 > 63) {
      return 0;
    }
    *counter = (uint8_t)(dst - counter - 1);
    counter = dst++;
    if (*src != '.') {
      break;
    }
    src++;
  } while (true);
  *counter = 0;
  return (size_t)(src - srcinit);
}

#include <stdio.h>
void print8(const char *name, __m128i x) {
  printf("%.32s : ", name);
  uint8_t buffer[16];
  _mm_storeu_si128((__m128i *)buffer, x);
  for (size_t i = 0; i < 16; i++) {
    printf("%02x ", buffer[i]);
  }
  printf("\n");
}

// delimiters are marked as 0s, everything else as 1s
static inline uint16_t delimiter_mask(__m128i input) {
  // Delimiters for strings consisting of a contiguous set of characters
  // 0x00        :       end-of-file : 0b0000_0000
  // 0x20        :             space : 0b0010_0000
  // 0x22        :             quote : 0b0010_0010
  // 0x28        :  left parenthesis : 0b0010_1000
  // 0x29        : right parenthesis : 0b0010_1001
  // 0x09        :               tab : 0b0000_1001
  // 0x0a        :         line feed : 0b0000_1010
  // 0x3b        :         semicolon : 0b0011_1011
  // 0x0d        :   carriage return : 0b0000_1101
  const __m128i delimiters =
      _mm_setr_epi8(0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x09,
                    0x0a, 0x3b, 0x00, 0x0d, 0x00, 0x00);

  const __m128i mask = _mm_setr_epi8(-33, -1, -1, -1, -1, -1, -1, -1, -1, -33,
                                     -1, -1, -1, -1, -1, -1);
  // construct delimiter pattern for comparison
  __m128i pattern = _mm_shuffle_epi8(delimiters, input);
  // clear 5th bit (match 0x20 with 0x00, match 0x29 using 0x09)
  __m128i inputc = _mm_and_si128(input, _mm_shuffle_epi8(mask, input));
  __m128i classified = _mm_cmpeq_epi8(inputc, pattern);
  return (uint16_t)_mm_movemask_epi8(
      classified); // should be 1 wherever we have a match.
}

// the zero-trail of 'mask' indicates how many '.' we must use
// to padd input.
static inline __m128i pad_with_dots(__m128i input, uint16_t mask) {
  uint16_t length = (uint16_t)_tzcnt_u16(mask);

  static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                  0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  __m128i zero_mask = _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
  // masking with '.'
  return _mm_blendv_epi8(input, _mm_set1_epi8('.'), zero_mask);
}

static inline __m128i dot_to_counters(__m128i input,
                                      __m128i *previous_dotcounts) {
  __m128i dots = _mm_cmpeq_epi8(input, _mm_set1_epi8('.'));
  __m128i sequential =
      _mm_setr_epi8(-128, -127, -126, -125, -124, -123, -122, -121, -120, -119,
                    -118, -117, -116, -115, -114, -113);
  __m128i dotscounts = _mm_and_si128(dots, sequential);
  __m128i origdotscounts = dotscounts;
  __m128i shifted = _mm_alignr_epi8(*previous_dotcounts, dotscounts, 1);
  dotscounts = _mm_min_epi8(shifted, dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(*previous_dotcounts, dotscounts, 1),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(*previous_dotcounts, dotscounts, 2),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(*previous_dotcounts, dotscounts, 4),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(*previous_dotcounts, dotscounts, 8),
                            dotscounts);
  __m128i next = _mm_alignr_epi8(*previous_dotcounts, dotscounts, 1);
  *previous_dotcounts = dotscounts; // save it for later
  dotscounts = _mm_subs_epu8(next, origdotscounts);
  // need to subtract one to the counters.
  dotscounts = _mm_subs_epu8(dotscounts, _mm_set1_epi8(1));
  return _mm_blendv_epi8(input, dotscounts, dots);
}

static inline __m128i dot_to_counters_no_previous(__m128i input) {
  __m128i dots = _mm_cmpeq_epi8(input, _mm_set1_epi8('.'));
  __m128i sequential =
      _mm_setr_epi8(-128, -127, -126, -125, -124, -123, -122, -121, -120, -119,
                    -118, -117, -116, -115, -114, -113);
  __m128i dotscounts = _mm_and_si128(dots, sequential);
  __m128i origdotscounts = dotscounts;
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 1),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 2),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 4),
                            dotscounts);
  dotscounts = _mm_min_epi8(_mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 8),
                            dotscounts);
  __m128i next = _mm_alignr_epi8(_mm_setzero_si128(), dotscounts, 1);
  dotscounts = _mm_subs_epu8(next, origdotscounts);
  // need to subtract one to the counters.
  dotscounts = _mm_subs_epu8(dotscounts, _mm_set1_epi8(1));
  return _mm_blendv_epi8(input, dotscounts, dots);
}

size_t name_to_dnswire_simd(const char *src, uint8_t *dst) {
  const char *srcinit = src;
  // we always insert a fake '.' initially.
  __m128i input = _mm_loadu_si128((const __m128i *)src);
  input = _mm_alignr_epi8(input, _mm_set1_epi8('.'), 15);
  src -= 1; // we pretend that we are pointing at the virtual '.'.
  uint16_t m = delimiter_mask(input);
  while (m == 0) {
    src += 16;
    dst += 16;
    input = _mm_loadu_si128((const __m128i *)src);
    m = delimiter_mask(input);
  }
  // we have reached the end.
  input = pad_with_dots(input, m);
  size_t consumed_length = (size_t)(src + _tzcnt_u16(m) - srcinit);
  if (src > srcinit) { // we need to unwind
    __m128i previous = _mm_setzero_si128();
    do {
      _mm_storeu_si128((__m128i *)dst, dot_to_counters(input, &previous));
      previous = _mm_add_epi8(previous, _mm_set1_epi8((char)0x10));
      src -= 16;
      dst -= 16;
      if (src < srcinit) {
        break;
      }
      input = _mm_loadu_si128((const __m128i *)src);
    } while (true);
    input = _mm_loadu_si128((const __m128i *)(src + 1));
    input = _mm_alignr_epi8(input, _mm_set1_epi8('.'), 15);
    _mm_storeu_si128((__m128i *)dst, dot_to_counters(input, &previous));
  } else {
    _mm_storeu_si128((__m128i *)dst, dot_to_counters_no_previous(input));
  }
  return consumed_length;
}

void print8_avx(const char *name, __m256i x) {
  printf("%.32s : ", name);
  uint8_t buffer[32];
  _mm256_storeu_si256((__m256i *)buffer, x);
  for (size_t i = 0; i < 32; i++) {
    printf("%02x ", buffer[i]);
    if (i == 15) {
      printf("| ");
    }
  }
  printf("\n");
}
static inline __m256i pad_with_dots_avx(__m256i input, uint32_t mask) {
  uint32_t length = (uint32_t)_tzcnt_u32(mask);

  static int8_t zero_masks[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  __m256i zero_mask = _mm256_loadu_si256((__m256i *)(zero_masks + 32 - length));
  // masking with '.'
  return _mm256_blendv_epi8(input, _mm256_set1_epi8('.'), zero_mask);
}

// delimiters are marked as 0s, everything else as 1s
static inline uint32_t delimiter_mask_avx(__m256i input) {
  // Delimiters for strings consisting of a contiguous set of characters
  // 0x00        :       end-of-file : 0b0000_0000
  // 0x20        :             space : 0b0010_0000
  // 0x22        :             quote : 0b0010_0010
  // 0x28        :  left parenthesis : 0b0010_1000
  // 0x29        : right parenthesis : 0b0010_1001
  // 0x09        :               tab : 0b0000_1001
  // 0x0a        :         line feed : 0b0000_1010
  // 0x3b        :         semicolon : 0b0011_1011
  // 0x0d        :   carriage return : 0b0000_1101
  const __m256i delimiters = _mm256_setr_epi8(
      0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x09, 0x0a, 0x3b,
      0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x28, 0x09, 0x0a, 0x3b, 0x00, 0x0d, 0x00, 0x00);

  const __m256i mask = _mm256_setr_epi8(
      -33, -1, -1, -1, -1, -1, -1, -1, -1, -33, -1, -1, -1, -1, -1, -1, -33, -1,
      -1, -1, -1, -1, -1, -1, -1, -33, -1, -1, -1, -1, -1, -1);
  // construct delimiter pattern for comparison
  __m256i pattern = _mm256_shuffle_epi8(delimiters, input);
  // clear 5th bit (match 0x20 with 0x00, match 0x29 using 0x09)
  __m256i inputc = _mm256_and_si256(input, _mm256_shuffle_epi8(mask, input));
  __m256i classified = _mm256_cmpeq_epi8(inputc, pattern);
  return (uint32_t)_mm256_movemask_epi8(
      classified); // should be 1 wherever we have a match.
}

static inline __m256i dot_to_counters_avx(__m256i input,
                                          __m256i *previous_dotcounts) {
  __m256i dots = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('.'));
  __m256i sequential = _mm256_setr_epi8(
      -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117,
      -116, -115, -114, -113, -112, -111, -110, -109, -108, -107, -106, -105,
      -104, -103, -102, -101, -100, -99, -98, -97);
  __m256i dotscounts = _mm256_and_si256(dots, sequential);
  __m256i origdotscounts = dotscounts;
  __m256i pd = _mm256_permute2x128_si256(dotscounts, *previous_dotcounts, 0x21);
  __m256i shifted = _mm256_alignr_epi8(pd, dotscounts, 1);
  dotscounts = _mm256_min_epi8(shifted, dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 1), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 2), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 4), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 8), dotscounts);
  __m256i lower = _mm256_shuffle_epi8(dotscounts, _mm256_setzero_si256());
  lower = _mm256_permute2x128_si256(lower, *previous_dotcounts, 0x21);
  dotscounts = _mm256_min_epi8(lower, dotscounts);

  __m256i next = _mm256_alignr_epi8(lower, dotscounts, 1);

  *previous_dotcounts = dotscounts; // save it for later
  dotscounts = _mm256_subs_epu8(next, origdotscounts);
  // need to subtract one to the counters.
  dotscounts = _mm256_subs_epu8(dotscounts, _mm256_set1_epi8(1));

  return _mm256_blendv_epi8(input, dotscounts, dots);
}

static inline __m256i dot_to_counters_no_previous_avx(__m256i input) {
  __m256i dots = _mm256_cmpeq_epi8(input, _mm256_set1_epi8('.'));
  __m256i sequential = _mm256_setr_epi8(
      -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117,
      -116, -115, -114, -113, -112, -111, -110, -109, -108, -107, -106, -105,
      -104, -103, -102, -101, -100, -99, -98, -97);
  __m256i dotscounts = _mm256_and_si256(dots, sequential);
  __m256i origdotscounts = dotscounts;
  __m256i pd =
      _mm256_permute2x128_si256(dotscounts, _mm256_setzero_si256(), 0x21);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 1), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 2), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 4), dotscounts);
  dotscounts =
      _mm256_min_epi8(_mm256_alignr_epi8(pd, dotscounts, 8), dotscounts);
  __m256i lower = _mm256_shuffle_epi8(dotscounts, _mm256_setzero_si256());
  lower = _mm256_permute2x128_si256(lower, _mm256_setzero_si256(), 0x21);
  dotscounts = _mm256_min_epi8(lower, dotscounts);
  __m256i next = _mm256_alignr_epi8(lower, dotscounts, 1);
  dotscounts = _mm256_subs_epu8(next, origdotscounts);
  // need to subtract one to the counters.
  dotscounts = _mm256_subs_epu8(dotscounts, _mm256_set1_epi8(1));
  return _mm256_blendv_epi8(input, dotscounts, dots);
}

size_t name_to_dnswire_avx(const char *src, uint8_t *dst) {
  const char *srcinit = src;
  __m256i input = _mm256_loadu_si256((const __m256i *)src);
  __m256i pd = _mm256_permute2x128_si256(_mm256_set1_epi8('.'), input, 0x21);
  input = _mm256_alignr_epi8(input, pd, 15);
  src -= 1; // we pretend that we are pointing at the virtual '.'.
  uint32_t m = delimiter_mask_avx(input);

  if (m != 0) {
    if ((m & 0xffff) != 0) {
      __m128i input128 =
          pad_with_dots(_mm256_castsi256_si128(input), (uint16_t)m);
      size_t consumed_length = (size_t)(src + _tzcnt_u32(m) - srcinit);
      _mm_storeu_si128((__m128i *)dst, dot_to_counters_no_previous(input128));
      return consumed_length;
    }
    input = pad_with_dots_avx(input, m);
    size_t consumed_length = (size_t)(src + _tzcnt_u32(m) - srcinit);
    _mm256_storeu_si256((__m256i *)dst, dot_to_counters_no_previous_avx(input));
    return consumed_length;
  }
  do {
    src += 32;
    dst += 32;
    input = _mm256_loadu_si256((const __m256i *)src);
    m = delimiter_mask_avx(input);
  } while (m == 0);
  // we have reached the end.
  input = pad_with_dots_avx(input, m);
  size_t consumed_length = (size_t)(src + _tzcnt_u32(m) - srcinit);

  __m256i previous = _mm256_setzero_si256();
  do {
    _mm256_storeu_si256((__m256i *)dst, dot_to_counters_avx(input, &previous));
    previous = _mm256_add_epi8(previous, _mm256_set1_epi8((char)0x20));
    src -= 32;
    dst -= 32;
    if (src < srcinit) {
      break;
    }
    input = _mm256_loadu_si256((const __m256i *)src);
  } while (true);
  input = _mm256_loadu_si256((const __m256i *)(src + 1));
  pd = _mm256_permute2x128_si256(_mm256_set1_epi8('.'), input, 0x21);
  input = _mm256_alignr_epi8(input, pd, 15);
  _mm256_storeu_si256((__m256i *)dst, dot_to_counters_avx(input, &previous));

  return consumed_length;
}

typedef struct {
  uint64_t delimiter;
  uint64_t dots;
} del_dots_t;

static inline del_dots_t compute_index_avx(const char *src, uint8_t *dst) {
  __m256i input1 = _mm256_loadu_si256((const __m256i *)src);
  __m256i input2 = _mm256_loadu_si256((const __m256i *)(src + 32));
  _mm256_storeu_si256((__m256i *)(dst), input1);
  _mm256_storeu_si256((__m256i *)(dst + 32), input2);
  uint32_t m1 = delimiter_mask_avx(input1);
  uint32_t m2 = delimiter_mask_avx(input2);
  uint32_t dots1 = (uint32_t)_mm256_movemask_epi8(
      _mm256_cmpeq_epi8(input1, _mm256_set1_epi8('.')));
  uint32_t dots2 = (uint32_t)_mm256_movemask_epi8(
      _mm256_cmpeq_epi8(input2, _mm256_set1_epi8('.')));
  uint64_t M = (((uint64_t)m2) << 32) | m1;
  uint64_t D = (((uint64_t)dots2) << 32) | dots1;
  del_dots_t dd = {M, D};
  return dd;
}

#ifdef __AVX512F__

__attribute__((always_inline))
static inline del_dots_t compute_index_avx512(const char *src, uint8_t *dst) {
  __m512i input = _mm512_loadu_si512((const __m512i *)src);
  _mm512_storeu_si512((__m512i *)(dst), input);
  __m512i mask = _mm512_set_epi8(0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 40, 0, 0, 0, 0, 0, 34, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  uint64_t M = _mm512_cmpeq_epi8_mask(input, _mm512_permutexvar_epi8(input, mask));
  uint64_t D = _mm512_cmpeq_epi8_mask(input, _mm512_set1_epi8('.'));
  del_dots_t dd = {M, D};
  return dd;
}



size_t name_to_dnswire_idx_avx512(const char *src, uint8_t *dst) {
  // There is an obvious loop structure which could be used to shorten
  // the function.
  del_dots_t dd1 = compute_index_avx512(src, dst + 1);
  if ((dd1.dots & 1) == 1) {
    return 0;
  }
  //////////
  // Check if input first in 64 bytes.
  //////////
  if (dd1.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd1.delimiter);
    dd1.dots = (((uint64_t)1 << length) - 1) & dd1.dots;

    if (dd1.dots & (dd1.dots << 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;

    // We have a virtual '1' bit right before dd1.dots.
    // We have that t points at the index of the current dot, starting
    // with -1.
    // We mark the position of the next dot as nt.
    // We start at location t + 1 the difference nt - t - 1.
    // Then move t to nt, delete the bits in dots, and so forth.
    uint64_t t = (uint64_t)0 - 1;
    if (dd1.dots) {

      uint64_t nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << nt;
      while (dd1.dots) {
        nt = _tzcnt_u64(dd1.dots);
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd1.dots ^= (uint64_t)1 << nt;
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd2 = compute_index_avx512(src + 64, dst + 1 + 64);

  if (dd1.dots == 0) {
    return 0;
  }
  if ((dd1.dots & ((dd2.dots << 63) | dd1.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 128 bytes.
  //////////
  if (dd2.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd2.delimiter);

    dd2.dots = (((uint64_t)1 << length) - 1) & dd2.dots;
    length += 64;
    if (dd2.dots & (dd2.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = (uint64_t)0 - 1;
    uint64_t nt = _tzcnt_u64(dd1.dots);
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd1.dots ^= (uint64_t)1 << nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << nt;
    }
    if (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
      while (dd2.dots) {
        nt = _tzcnt_u64(dd2.dots) + 64;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd2.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd3 = compute_index_avx512(src + 64 * 2, dst + 1 + 64 * 2);
  if (dd2.dots == 0) {
    return 0;
  }
  if ((dd2.dots & ((dd3.dots << 63) | dd2.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 192 bytes.
  //////////
  if (dd3.delimiter != 0) {

    uint64_t length = _tzcnt_u64(dd3.delimiter);
    dd3.dots = (((uint64_t)1 << length) - 1) & dd3.dots;
    length += 128;
    if (dd3.dots & (dd3.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = _tzcnt_u64(dd1.dots);
    dstwriter[0] = (uint8_t)t;
    dd1.dots ^= (uint64_t)1 << t;
    uint64_t nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << t;
    }
    nt = _tzcnt_u64(dd2.dots) + 64;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd2.dots ^= (uint64_t)1 << (t & 63);
    while (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
    }
    if (dd3.dots) {

      nt = _tzcnt_u64(dd3.dots) + 128;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd3.dots ^= (uint64_t)1 << (t & 63);
      while (dd3.dots) {
        nt = _tzcnt_u64(dd3.dots) + 128;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd3.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd4 = compute_index_avx512(src + 64 * 3, dst + 1 + 64 * 3);
  if (dd3.dots == 0) {
    return 0;
  }
  if ((dd3.dots & ((dd4.dots << 63) | dd3.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 256 bytes.
  //////////
  if (dd4.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd4.delimiter);
    dd4.dots = (((uint64_t)1 << length) - 1) & dd4.dots;
    length += 192;
    if (dd4.dots & (dd4.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = _tzcnt_u64(dd1.dots);
    dstwriter[0] = (uint8_t)t;
    dd1.dots ^= (uint64_t)1 << t;
    uint64_t nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << t;
    }
    nt = _tzcnt_u64(dd2.dots) + 64;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd2.dots ^= (uint64_t)1 << (t & 63);
    while (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
    }
    nt = _tzcnt_u64(dd3.dots) + 128;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd3.dots ^= (uint64_t)1 << (t & 63);
    while (dd3.dots) {
      nt = _tzcnt_u64(dd3.dots) + 128;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd3.dots ^= (uint64_t)1 << (t & 63);
    }
    if (dd4.dots) {
      nt = _tzcnt_u64(dd4.dots) + 192;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd4.dots ^= (uint64_t)1 << (t & 63);
      while (dd4.dots) {
        nt = _tzcnt_u64(dd4.dots) + 192;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd4.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  //////////
  // The input is too long, we reject it.
  //////////
  return 0;
}
#endif
void printbinary(uint64_t n) {
  for (size_t i = 0; i < 64; i++) {
    if (n & 1)
      printf("1");
    else
      printf("0");

    n >>= 1;
  }
  printf("\n");
}

// simplified version of name_to_dnswire_idx_avx
size_t name_to_dnswire_loop(const char *src, uint8_t *dst)
{
  const char *text = src;
  uint8_t *label = dst, *wire = label + 1;
  // labels in domain names are limited to 63 octets. track length octets
  // (dots) in 64-bit wide bitmap. shift by length of block last copied to
  // detect null-labels and labels exceeding 63 octets (zero)
  uint64_t labels = 1llu << 63; // virtual length octet preceding first label

  *label = 0;

  for (uint64_t delimiter = 0; !delimiter; ) {
    // real world domain names quickly exceed 16 octets (www.example.com is
    // encoded as 3www7example3com0, or 18 octets), but rarely exceed 32
    // octets. encode in 32-byte blocks.
    const __m256i input = _mm256_loadu_si256((const __m256i *)text);
    _mm256_storeu_si256((__m256i *)wire, input);

    delimiter = delimiter_mask_avx(input);
    uint64_t dots = (uint32_t)_mm256_movemask_epi8(
      _mm256_cmpeq_epi8(input, _mm256_set1_epi8('.')));

    uint64_t limit = delimiter | (1llu << 32);
    uint64_t length = _tzcnt_u64(limit);

    // FIXME: account for escape sequences here in production
    dots &= (1llu << length) - 1;
    text += length;
    wire += length;
    labels = (dots << (64 - length)) | (labels >> length);
    if (!labels || labels & (labels >> 1))
      return 0;

    if (dots) {
      length = _tzcnt_u64(dots);
      dots >>= length + 1;
      *label += (uint8_t)length;

      while (dots) {
        length = _tzcnt_u64(dots);
        dots >>= length + 1;
        *label = (uint8_t)length;
        label += length + 1;
      }
    }
    *label = (uint8_t)(wire - label) - 1;
  }

  return (size_t)(wire - dst);
}

size_t name_to_dnswire_idx_avx(const char *src, uint8_t *dst) {
  // There is an obvious loop structure which could be used to shorten
  // the function.
  del_dots_t dd1 = compute_index_avx(src, dst + 1);
  if ((dd1.dots & 1) == 1) {
    return 0;
  }
  //////////
  // Check if input first in 64 bytes.
  //////////
  if (dd1.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd1.delimiter);
    dd1.dots = (((uint64_t)1 << length) - 1) & dd1.dots;

    if (dd1.dots & (dd1.dots << 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;

    // We have a virtual '1' bit right before dd1.dots.
    // We have that t points at the index of the current dot, starting
    // with -1.
    // We mark the position of the next dot as nt.
    // We start at location t + 1 the difference nt - t - 1.
    // Then move t to nt, delete the bits in dots, and so forth.
    uint64_t t = (uint64_t)0 - 1;
    if (dd1.dots) {

      uint64_t nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << nt;
      while (dd1.dots) {
        nt = _tzcnt_u64(dd1.dots);
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd1.dots ^= (uint64_t)1 << nt;
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd2 = compute_index_avx(src + 64, dst + 1 + 64);

  if (dd1.dots == 0) {
    return 0;
  }
  if ((dd1.dots & ((dd2.dots << 63) | dd1.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 128 bytes.
  //////////
  if (dd2.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd2.delimiter);

    dd2.dots = (((uint64_t)1 << length) - 1) & dd2.dots;
    length += 64;
    if (dd2.dots & (dd2.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = (uint64_t)0 - 1;
    uint64_t nt = _tzcnt_u64(dd1.dots);
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd1.dots ^= (uint64_t)1 << nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << nt;
    }
    if (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
      while (dd2.dots) {
        nt = _tzcnt_u64(dd2.dots) + 64;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd2.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd3 = compute_index_avx(src + 64 * 2, dst + 1 + 64 * 2);
  if (dd2.dots == 0) {
    return 0;
  }
  if ((dd2.dots & ((dd3.dots << 63) | dd2.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 192 bytes.
  //////////
  if (dd3.delimiter != 0) {

    uint64_t length = _tzcnt_u64(dd3.delimiter);
    dd3.dots = (((uint64_t)1 << length) - 1) & dd3.dots;
    length += 128;
    if (dd3.dots & (dd3.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = _tzcnt_u64(dd1.dots);
    dstwriter[0] = (uint8_t)t;
    dd1.dots ^= (uint64_t)1 << t;
    uint64_t nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << t;
    }
    nt = _tzcnt_u64(dd2.dots) + 64;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd2.dots ^= (uint64_t)1 << (t & 63);
    while (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
    }
    if (dd3.dots) {

      nt = _tzcnt_u64(dd3.dots) + 128;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd3.dots ^= (uint64_t)1 << (t & 63);
      while (dd3.dots) {
        nt = _tzcnt_u64(dd3.dots) + 128;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd3.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  del_dots_t dd4 = compute_index_avx(src + 64 * 3, dst + 1 + 64 * 3);
  if (dd3.dots == 0) {
    return 0;
  }
  if ((dd3.dots & ((dd4.dots << 63) | dd3.dots >> 1))) {
    return 0;
  }
  //////////
  // Check if input first in 256 bytes.
  //////////
  if (dd4.delimiter != 0) {
    uint64_t length = _tzcnt_u64(dd4.delimiter);
    dd4.dots = (((uint64_t)1 << length) - 1) & dd4.dots;
    length += 192;
    if (dd4.dots & (dd4.dots >> 1)) {
      return 0;
    }
    uint8_t *dstwriter = dst;
    uint64_t t = _tzcnt_u64(dd1.dots);
    dstwriter[0] = (uint8_t)t;
    dd1.dots ^= (uint64_t)1 << t;
    uint64_t nt;
    while (dd1.dots) {
      nt = _tzcnt_u64(dd1.dots);
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd1.dots ^= (uint64_t)1 << t;
    }
    nt = _tzcnt_u64(dd2.dots) + 64;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd2.dots ^= (uint64_t)1 << (t & 63);
    while (dd2.dots) {
      nt = _tzcnt_u64(dd2.dots) + 64;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd2.dots ^= (uint64_t)1 << (t & 63);
    }
    nt = _tzcnt_u64(dd3.dots) + 128;
    if (nt - t > 64) {
      return 0;
    }
    dstwriter[t + 1] = (uint8_t)(nt - t - 1);
    t = nt;
    dd3.dots ^= (uint64_t)1 << (t & 63);
    while (dd3.dots) {
      nt = _tzcnt_u64(dd3.dots) + 128;
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd3.dots ^= (uint64_t)1 << (t & 63);
    }
    if (dd4.dots) {
      nt = _tzcnt_u64(dd4.dots) + 192;
      if (nt - t > 64) {
        return 0;
      }
      dstwriter[t + 1] = (uint8_t)(nt - t - 1);
      t = nt;
      dd4.dots ^= (uint64_t)1 << (t & 63);
      while (dd4.dots) {
        nt = _tzcnt_u64(dd4.dots) + 192;
        dstwriter[t + 1] = (uint8_t)(nt - t - 1);
        t = nt;
        dd4.dots ^= (uint64_t)1 << (t & 63);
      }
    }
    dstwriter[t + 1] = (uint8_t)(length - t - 1);
    return length;
  }
  //////////
  // The input is too long, we reject it.
  //////////
  return 0;
}
