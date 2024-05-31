#include <array>
#include <cstdint>
static constexpr std::array<uint8_t, 256> json_quotable_character =
    []() constexpr {
      std::array<uint8_t, 256> result{};
      for (int i = 0; i < 32; i++) {
        result[i] = 1;
      }
      for (int i : {'"', '\\'}) {
        result[i] = 1;
      }
      return result;
    }();

constexpr inline bool simple_needs_escaping(std::string_view v) {
  for (char c : v) {
    if ((uint8_t(c) < 32) | (c == '"') | (c == '\\')) {
      return true;
    }
  }
  return false;
}

constexpr inline bool branchless_needs_escaping(std::string_view v) {
  bool b = false;
  for (char c : v) {
    b |= ((uint8_t(c) < 32) | (c == '"') | (c == '\\'));
  }
  return b;
}

constexpr inline bool table_needs_escaping(std::string_view view) {
  uint8_t needs = 0;
  for (uint8_t c : view) {
    needs |= json_quotable_character[c];
  }
  return needs;
}

#if defined(__SSE2__) || defined(__x86_64__) || defined(__x86_64) ||           \
    (defined(_M_AMD64) || defined(_M_X64) ||                                   \
     (defined(_M_IX86_FP) && _M_IX86_FP == 2))
#define HAS_SSE2 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#define HAS_NEON 1
#endif

#if HAS_NEON
#include <arm_neon.h>

inline bool simd_needs_escaping(std::string_view view) {
  if (view.size() < 16) {
    return simple_needs_escaping(view);
  }
  size_t i = 0;
  static uint8_t rnt_array[16] = {1, 0, 34, 0, 0,  0, 0, 0,
                                  0, 0, 0,  0, 92, 0, 0, 0};
  const uint8x16_t rnt = vld1q_u8(rnt_array);
  uint8x16_t running{0};
  for (; i + 15 < view.size(); i += 16) {
    uint8x16_t word = vld1q_u8((const uint8_t *)view.data() + i);
    running = vorrq_u8(
        running,
        vceqq_u8(vqtbl1q_u8(rnt, vandq_u8(word, vdupq_n_u8(15))), word));
    running = vorrq_u8(running, vcltq_u8(word, vdupq_n_u8(32)));
  }
  if (i < view.size()) {
    uint8x16_t word =
        vld1q_u8((const uint8_t *)view.data() + view.length() - 16);
    running = vorrq_u8(
        running,
        vceqq_u8(vqtbl1q_u8(rnt, vandq_u8(word, vdupq_n_u8(15))), word));
    running = vorrq_u8(running, vcltq_u8(word, vdupq_n_u8(32)));
  }
  return vmaxvq_u32(vreinterpretq_u32_u8(running)) != 0;
}

#elif HAS_SSE2
#include <emmintrin.h>

inline bool simd_needs_escaping(std::string_view view) {
  if (view.size() < 16) {
    return simple_needs_escaping(view);
  }
  size_t i = 0;
  __m128i running = _mm_setzero_si128();
  for (; i + 15 < view.size(); i += 16) {
    __m128i word = _mm_loadu_si128((const __m128i *)(view.data() + i));
    running = _mm_or_si128(running, _mm_cmpeq_epi8(word, _mm_set1_epi8(34)));
    running = _mm_or_si128(running, _mm_cmpeq_epi8(word, _mm_set1_epi8(92)));
    running = _mm_or_si128(
        running, _mm_cmpeq_epi8(_mm_subs_epu8(word, _mm_set1_epi8(31)),
                                _mm_setzero_si128()));
  }
  if (i < view.size()) {
    __m128i word =
        _mm_loadu_si128((const __m128i *)(view.data() + view.length() - 16));
    running = _mm_or_si128(running, _mm_cmpeq_epi8(word, _mm_set1_epi8(34)));
    running = _mm_or_si128(running, _mm_cmpeq_epi8(word, _mm_set1_epi8(92)));
    running = _mm_or_si128(
        running, _mm_cmpeq_epi8(_mm_subs_epu8(word, _mm_set1_epi8(31)),
                                _mm_setzero_si128()));
  }
  return _mm_movemask_epi8(running) != 0;
}

#endif