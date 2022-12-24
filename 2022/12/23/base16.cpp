#include <x86intrin.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

void encode_scalar(const uint8_t *source, size_t len, char *target) {
  const uint16_t table[] = {
      0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830,
      0x3930, 0x6130, 0x6230, 0x6330, 0x6430, 0x6530, 0x6630, 0x3031, 0x3131,
      0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731, 0x3831, 0x3931, 0x6131,
      0x6231, 0x6331, 0x6431, 0x6531, 0x6631, 0x3032, 0x3132, 0x3232, 0x3332,
      0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932, 0x6132, 0x6232, 0x6332,
      0x6432, 0x6532, 0x6632, 0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533,
      0x3633, 0x3733, 0x3833, 0x3933, 0x6133, 0x6233, 0x6333, 0x6433, 0x6533,
      0x6633, 0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734,
      0x3834, 0x3934, 0x6134, 0x6234, 0x6334, 0x6434, 0x6534, 0x6634, 0x3035,
      0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935,
      0x6135, 0x6235, 0x6335, 0x6435, 0x6535, 0x6635, 0x3036, 0x3136, 0x3236,
      0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936, 0x6136, 0x6236,
      0x6336, 0x6436, 0x6536, 0x6636, 0x3037, 0x3137, 0x3237, 0x3337, 0x3437,
      0x3537, 0x3637, 0x3737, 0x3837, 0x3937, 0x6137, 0x6237, 0x6337, 0x6437,
      0x6537, 0x6637, 0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638,
      0x3738, 0x3838, 0x3938, 0x6138, 0x6238, 0x6338, 0x6438, 0x6538, 0x6638,
      0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839,
      0x3939, 0x6139, 0x6239, 0x6339, 0x6439, 0x6539, 0x6639, 0x3061, 0x3161,
      0x3261, 0x3361, 0x3461, 0x3561, 0x3661, 0x3761, 0x3861, 0x3961, 0x6161,
      0x6261, 0x6361, 0x6461, 0x6561, 0x6661, 0x3062, 0x3162, 0x3262, 0x3362,
      0x3462, 0x3562, 0x3662, 0x3762, 0x3862, 0x3962, 0x6162, 0x6262, 0x6362,
      0x6462, 0x6562, 0x6662, 0x3063, 0x3163, 0x3263, 0x3363, 0x3463, 0x3563,
      0x3663, 0x3763, 0x3863, 0x3963, 0x6163, 0x6263, 0x6363, 0x6463, 0x6563,
      0x6663, 0x3064, 0x3164, 0x3264, 0x3364, 0x3464, 0x3564, 0x3664, 0x3764,
      0x3864, 0x3964, 0x6164, 0x6264, 0x6364, 0x6464, 0x6564, 0x6664, 0x3065,
      0x3165, 0x3265, 0x3365, 0x3465, 0x3565, 0x3665, 0x3765, 0x3865, 0x3965,
      0x6165, 0x6265, 0x6365, 0x6465, 0x6565, 0x6665, 0x3066, 0x3166, 0x3266,
      0x3366, 0x3466, 0x3566, 0x3666, 0x3766, 0x3866, 0x3966, 0x6166, 0x6266,
      0x6366, 0x6466, 0x6566, 0x6666};
  for (size_t i = 0; i < len; i++) {
    uint16_t code = table[source[i]];
    ::memcpy(target, &code, 2);
    target += 2;
  }
}

void encode_ssse3(const uint8_t *source, size_t len, char *target) {
  __m128i shuf = _mm_set_epi8('f', 'e', 'd', 'c', 'b', 'a', '9', '8', '7', '6',
                              '5', '4', '3', '2', '1', '0');
  size_t i = 0;
  __m128i maskf = _mm_set1_epi8(0xf);
  for (; i + 16 <= len; i += 16) {
    __m128i input = _mm_loadu_si128((const __m128i *)(source + i));
    __m128i inputbase = _mm_and_si128(maskf, input);
    __m128i inputs4 = _mm_and_si128(maskf, _mm_srli_epi16(input, 4));
    __m128i firstpart = _mm_unpacklo_epi8(inputs4, inputbase);
    __m128i output1 = _mm_shuffle_epi8(shuf, firstpart);
    __m128i secondpart = _mm_unpackhi_epi8(inputs4, inputbase);
    __m128i output2 = _mm_shuffle_epi8(shuf, secondpart);
    _mm_storeu_si128((__m128i *)(target), output1);
    target += 16;
    _mm_storeu_si128((__m128i *)(target), output2);
    target += 16;
  }
  encode_scalar(source + i, len - i, target);
}

void encode_avx2(const uint8_t *source, size_t len, char *target) {
  __m256i shuf =
      _mm256_set_epi8('f', 'e', 'd', 'c', 'b', 'a', '9', '8', '7', '6', '5',
                      '4', '3', '2', '1', '0', 'f', 'e', 'd', 'c', 'b', 'a',
                      '9', '8', '7', '6', '5', '4', '3', '2', '1', '0');
  size_t i = 0;
  __m256i maskf = _mm256_set1_epi8(0xf);

  for (; i + 32 <= len; i += 32) {
    __m256i input = _mm256_loadu_si256((const __m256i *)(source + i));
    input = _mm256_permute4x64_epi64(input, 0b11011000);
    __m256i inputbase = _mm256_and_si256(maskf, input);
    __m256i inputs4 = _mm256_and_si256(maskf, _mm256_srli_epi16(input, 4));
    __m256i firstpart = _mm256_unpacklo_epi8(inputs4, inputbase);
    __m256i output1 = _mm256_shuffle_epi8(shuf, firstpart);
    __m256i secondpart = _mm256_unpackhi_epi8(inputs4, inputbase);
    __m256i output2 = _mm256_shuffle_epi8(shuf, secondpart);
    _mm256_storeu_si256((__m256i *)(target), output1);
    target += 32;
    _mm256_storeu_si256((__m256i *)(target), output2);
    target += 32;
  }
  encode_scalar(source + i, len - i, target);
}

void encode_scalar_to_utf16(const uint8_t *source, size_t len,
                            char16_t *target) {
  const uint32_t table[] = {
      0x00300030, 0x00300030, 0x00300030, 0x00300030, 0x00300030, 0x00300030,
      0x00300030, 0x00300030, 0x00300030, 0x00300030, 0x00600030, 0x00600030,
      0x00600030, 0x00600030, 0x00600030, 0x00600030, 0x00300031, 0x00300031,
      0x00300031, 0x00300031, 0x00300031, 0x00300031, 0x00300031, 0x00300031,
      0x00300031, 0x00300031, 0x00600031, 0x00600031, 0x00600031, 0x00600031,
      0x00600031, 0x00600031, 0x00300032, 0x00300032, 0x00300032, 0x00300032,
      0x00300032, 0x00300032, 0x00300032, 0x00300032, 0x00300032, 0x00300032,
      0x00600032, 0x00600032, 0x00600032, 0x00600032, 0x00600032, 0x00600032,
      0x00300033, 0x00300033, 0x00300033, 0x00300033, 0x00300033, 0x00300033,
      0x00300033, 0x00300033, 0x00300033, 0x00300033, 0x00600033, 0x00600033,
      0x00600033, 0x00600033, 0x00600033, 0x00600033, 0x00300034, 0x00300034,
      0x00300034, 0x00300034, 0x00300034, 0x00300034, 0x00300034, 0x00300034,
      0x00300034, 0x00300034, 0x00600034, 0x00600034, 0x00600034, 0x00600034,
      0x00600034, 0x00600034, 0x00300035, 0x00300035, 0x00300035, 0x00300035,
      0x00300035, 0x00300035, 0x00300035, 0x00300035, 0x00300035, 0x00300035,
      0x00600035, 0x00600035, 0x00600035, 0x00600035, 0x00600035, 0x00600035,
      0x00300036, 0x00300036, 0x00300036, 0x00300036, 0x00300036, 0x00300036,
      0x00300036, 0x00300036, 0x00300036, 0x00300036, 0x00600036, 0x00600036,
      0x00600036, 0x00600036, 0x00600036, 0x00600036, 0x00300037, 0x00300037,
      0x00300037, 0x00300037, 0x00300037, 0x00300037, 0x00300037, 0x00300037,
      0x00300037, 0x00300037, 0x00600037, 0x00600037, 0x00600037, 0x00600037,
      0x00600037, 0x00600037, 0x00300038, 0x00300038, 0x00300038, 0x00300038,
      0x00300038, 0x00300038, 0x00300038, 0x00300038, 0x00300038, 0x00300038,
      0x00600038, 0x00600038, 0x00600038, 0x00600038, 0x00600038, 0x00600038,
      0x00300039, 0x00300039, 0x00300039, 0x00300039, 0x00300039, 0x00300039,
      0x00300039, 0x00300039, 0x00300039, 0x00300039, 0x00600039, 0x00600039,
      0x00600039, 0x00600039, 0x00600039, 0x00600039, 0x00300061, 0x00300061,
      0x00300061, 0x00300061, 0x00300061, 0x00300061, 0x00300061, 0x00300061,
      0x00300061, 0x00300061, 0x00600061, 0x00600061, 0x00600061, 0x00600061,
      0x00600061, 0x00600061, 0x00300062, 0x00300062, 0x00300062, 0x00300062,
      0x00300062, 0x00300062, 0x00300062, 0x00300062, 0x00300062, 0x00300062,
      0x00600062, 0x00600062, 0x00600062, 0x00600062, 0x00600062, 0x00600062,
      0x00300063, 0x00300063, 0x00300063, 0x00300063, 0x00300063, 0x00300063,
      0x00300063, 0x00300063, 0x00300063, 0x00300063, 0x00600063, 0x00600063,
      0x00600063, 0x00600063, 0x00600063, 0x00600063, 0x00300064, 0x00300064,
      0x00300064, 0x00300064, 0x00300064, 0x00300064, 0x00300064, 0x00300064,
      0x00300064, 0x00300064, 0x00600064, 0x00600064, 0x00600064, 0x00600064,
      0x00600064, 0x00600064, 0x00300065, 0x00300065, 0x00300065, 0x00300065,
      0x00300065, 0x00300065, 0x00300065, 0x00300065, 0x00300065, 0x00300065,
      0x00600065, 0x00600065, 0x00600065, 0x00600065, 0x00600065, 0x00600065,
      0x00300066, 0x00300066, 0x00300066, 0x00300066, 0x00300066, 0x00300066,
      0x00300066, 0x00300066, 0x00300066, 0x00300066, 0x00600066, 0x00600066,
      0x00600066, 0x00600066, 0x00600066, 0x00600066};
  for (size_t i = 0; i < len; i++) {
    uint32_t code = table[source[i]];
    ::memcpy(target, &code, 4);
    target += 2;
  }
}

void encode_sse41_to_utf16(const uint8_t *source, size_t len,
                           char16_t *target) {
  __m128i shuf = _mm_set_epi8('f', 'e', 'd', 'c', 'b', 'a', '9', '8', '7', '6',
                              '5', '4', '3', '2', '1', '0');
  size_t i = 0;
  __m128i maskf = _mm_set1_epi8(0xf);
  for (; i + 16 <= len; i += 16) {
    __m128i input = _mm_loadu_si128((const __m128i *)(source + i));
    __m128i inputbase = _mm_and_si128(maskf, input);
    __m128i inputs4 = _mm_and_si128(maskf, _mm_srli_epi16(input, 4));
    __m128i firstpart = _mm_unpacklo_epi8(inputs4, inputbase);
    __m128i output1 = _mm_shuffle_epi8(shuf, firstpart);
    __m128i secondpart = _mm_unpackhi_epi8(inputs4, inputbase);
    __m128i output2 = _mm_shuffle_epi8(shuf, secondpart);
    _mm_storeu_si128((__m128i *)(target), _mm_cvtepu8_epi16(output1));
    _mm_storeu_si128((__m128i *)(target + 8),
                     _mm_cvtepu8_epi16(_mm_shuffle_epi32(output1, 0b01001110)));
    target += 16;
    _mm_storeu_si128((__m128i *)(target), _mm_cvtepu8_epi16(output2));
    _mm_storeu_si128((__m128i *)(target + 8),
                     _mm_cvtepu8_epi16(_mm_shuffle_epi32(output2, 0b01001110)));
    target += 16;
  }
  encode_scalar_to_utf16(source + i, len - i, target);
}

void encode_avx2_to_utf16(const uint8_t *source, size_t len, char16_t *target) {
  __m256i shuf =
      _mm256_set_epi8('f', 'e', 'd', 'c', 'b', 'a', '9', '8', '7', '6', '5',
                      '4', '3', '2', '1', '0', 'f', 'e', 'd', 'c', 'b', 'a',
                      '9', '8', '7', '6', '5', '4', '3', '2', '1', '0');
  size_t i = 0;
  __m256i maskf = _mm256_set1_epi8(0xf);

  for (; i + 32 <= len; i += 32) {
    __m256i input = _mm256_loadu_si256((const __m256i *)(source + i));
    input = _mm256_permute4x64_epi64(input, 0b11011000);
    __m256i inputbase = _mm256_and_si256(maskf, input);
    __m256i inputs4 = _mm256_and_si256(maskf, _mm256_srli_epi16(input, 4));
    __m256i firstpart = _mm256_unpacklo_epi8(inputs4, inputbase);
    __m256i output1 = _mm256_shuffle_epi8(shuf, firstpart);
    __m256i secondpart = _mm256_unpackhi_epi8(inputs4, inputbase);
    __m256i output2 = _mm256_shuffle_epi8(shuf, secondpart);
    _mm256_storeu_si256((__m256i *)(target),
                        _mm256_cvtepu8_epi16(_mm256_castsi256_si128(output1)));
    _mm256_storeu_si256(
        (__m256i *)(target + 16),
        _mm256_cvtepu8_epi16(_mm256_extracti128_si256(output1, 1)));
    target += 32;
    _mm256_storeu_si256((__m256i *)(target),
                        _mm256_cvtepu8_epi16(_mm256_castsi256_si128(output2)));
    _mm256_storeu_si256(
        (__m256i *)(target + 16),
        _mm256_cvtepu8_epi16(_mm256_extracti128_si256(output2, 1)));
    target += 32;
  }
  encode_scalar_to_utf16(source + i, len - i, target);
}
void to_utf8(size_t N) {

  uint8_t *input = new uint8_t[N];
  for (size_t i = 0; i < N; i++) {
    input[i] = rand();
  }

  char *buffer = new char[2 * N];

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_scalar(input, N, buffer);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("scalar %f GB/s\n", t);
  }
  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_ssse3(input, N, buffer);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("ssse3  %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_avx2(input, N, buffer);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("avx2   %f GB/s\n", t);
  }
  delete[] input;

  delete[] buffer;
}

void to_utf16(size_t N) {
  uint8_t *input = new uint8_t[N];

  for (size_t i = 0; i < N; i++) {
    input[i] = rand();
  }

  char16_t *buffer16 = new char16_t[2 * N];

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_scalar_to_utf16(input, N, buffer16);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("to:utf16 scalar %f GB/s\n", t);
  }
  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_sse41_to_utf16(input, N, buffer16);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("to:utf16 sse4   %f GB/s\n", t);
  }

  {
    uint64_t start = nano();
    uint64_t finish = start;
    size_t count{0};
    uint64_t threshold = 500000000;
    for (; finish - start < threshold;) {
      count++;
      encode_avx2_to_utf16(input, N, buffer16);
      finish = nano();
    }
    double t = (N * count) / double(finish - start);

    printf("to:utf16 avx2   %f GB/s\n", t);
  }
  delete[] input;

  delete[] buffer16;
}
int main() {
  for (size_t N = 1024; N <= 8192; N *= 2) {
    printf("===N = %zu \n", N);
    to_utf8(N);
    to_utf16(N);
  }
}