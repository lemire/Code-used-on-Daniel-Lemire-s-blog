#include <cstring>
#include <x86intrin.h>

struct Original {
  static size_t escape(const char *in, size_t len, char *out) {
    const char *const finalin = in + len;
    const char *const initout = out;
    for (; in < finalin; in++) {
      if ((*in == '\\') || (*in == '"')) {
        *out = '\\';
        out++;
      }
      *out = *in;
      out++;
    }
    return out - initout;
  }
};

struct Optimized {
static size_t escape(const char *in, size_t len, char *out) {
  const char *const finalin = in + len;
  const char *const initout = out;
  __m512i solidus = _mm512_set1_epi8('\\');
  __m512i quote = _mm512_set1_epi8('"');
  for (; in + 32 <= finalin; in += 32) {
    __m256i input = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(in));
    __m512i input1 = _mm512_cvtepu8_epi16(input);
    __m512i shifted_input1 = _mm512_bslli_epi128(input1, 1);

    __mmask64 is_solidus = _mm512_cmpeq_epi8_mask(input1, solidus);
    __mmask64 is_quote = _mm512_cmpeq_epi8_mask(input1, quote);
    __mmask64 is_quote_or_solidus = _kor_mask64(is_solidus, is_quote);
    __mmask64 to_keep = _kor_mask64(is_quote_or_solidus, 0xaaaaaaaaaaaaaaaa);

    __m512i escaped =
        _mm512_mask_blend_epi8(is_quote_or_solidus, shifted_input1, solidus);
    _mm512_mask_compressstoreu_epi8(out, to_keep, escaped);
    out += _mm_popcnt_u64(_cvtmask64_u64(to_keep));
  }
  for (; in < finalin; in++) {
    if ((*in == '\\') || (*in == '"')) {
      *out = '\\';
      out++;
    }
    *out = *in;
    out++;
  }
  return out - initout;
}
};

