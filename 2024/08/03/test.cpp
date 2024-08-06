
#include "performancecounters/benchmarker.h"
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <immintrin.h>
#include <unistd.h> // assumes linux

void pretty_print(size_t, size_t bytes, std::string name, event_aggregate agg) {
  printf("%-40s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.elapsed_ns());
  printf(" %5.2f ns/value ", agg.elapsed_ns() / bytes);

  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/byte ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/byte ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}
static inline __m512i tolower64(__m512i c) {
  __m512i A = _mm512_set1_epi8('A');
  __m512i Z = _mm512_set1_epi8('Z');
  __m512i to_lower = _mm512_set1_epi8('a' - 'A');
  __mmask64 ge_A = _mm512_cmpge_epi8_mask(c, A);
  __mmask64 le_Z = _mm512_cmple_epi8_mask(c, Z);
  __mmask64 is_upper = _kand_mask64(ge_A, le_Z);
  return (_mm512_mask_add_epi8(c, is_upper, c, to_lower));
}

void print(__m512i c) {
  uint8_t buffer[64];
  _mm512_storeu_epi8(buffer, c);
  for(size_t i = 0; i<64;i++) printf(" %02x", buffer[i]);
  printf("\n");

}

void printchar(__m512i c) {
  uint8_t buffer[64];
  _mm512_storeu_epi8(buffer, c);
  for(size_t i = 0; i<64;i++)  {
    printf("%c", (char)buffer[i]);
  }
  printf("\n");

}
static inline __m512i tolower64_short(__m512i c) {
  __m512i ca = _mm512_sub_epi8(c, _mm512_set1_epi8('A'));
  __mmask64 is_upper = _mm512_cmple_epu8_mask(ca, _mm512_set1_epi8('Z' - 'A'));
  __m512i to_lower = _mm512_set1_epi8('a' - 'A');
  return (_mm512_mask_add_epi8(c, is_upper, c, to_lower));
}

static inline __m512i tolower64_vbmi_and_zen_2uops(__m512i c) {
  __mmask64 le_7t = _mm512_cmple_epu8_mask(c, _mm512_set1_epi8 (0x7f));
   __m512i byteconst_00_3f = _mm512_set_epi64 (0x3f3e3d3c3b3a3938, 0x3736353433323130, 0x2f2e2d2c2b2a2928, 0x2726252423222120, 0x1f1e1d1c1b1a1918, 0x1716151413121110, 0x0f0e0d0c0b0a0908, 0x0706050403020100);
   __m512i byteconst_40_7f = _mm512_set_epi64 (0x7f7e7d7c7b7a7978, 0x7776757473727170, 0x6f6e6d6c6b6a6968, 0x6766656463626160, 0x5f55d5c5b7a7978, 0x7776757473727170, 0x6f6e6d6c6b6a6968, 0x6766656463626140);
  return _mm512_mask2_permutex2var_epi8 (byteconst_00_3f, c, le_7t, byteconst_40_7f);
}

size_t lower(char *srcorig) {
  char *p = srcorig;
  for (; *p; ++p) {
    *p = *p > 0x40 && *p < 0x5b ? *p | 0x20 : *p;
  }
  return p - srcorig;
}

size_t lower(const char *srcorig, char * dst) {
  const char *p = srcorig;
  for (; *p; ++p, dst++) {
    *dst = *p > 0x40 && *p < 0x5b ? *p | 0x60 : *p;
  }
  return p - srcorig;
}

size_t lower64(char *srcorig) {
  uintptr_t address = reinterpret_cast<uintptr_t>(srcorig);
  uintptr_t aligned_address = address / 64 * 64;     // round down
  uintptr_t notincluded = address - aligned_address; // [0,64)
  char *src = reinterpret_cast<char *>(aligned_address);
  __mmask64 init_mask = _cvtu64_mask64((~UINT64_C(0)) << notincluded);
  __m512i src_v = _mm512_maskz_loadu_epi8(init_mask, src);
  __mmask64 is_zero =
      _mm512_mask_cmpeq_epu8_mask(init_mask, src_v, _mm512_setzero_si512());
  __m512i dst_v = tolower64(src_v);
  if (is_zero) {
    __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
    _mm512_mask_storeu_epi8(src, init_mask & zero_mask, dst_v);
    return __tzcnt_u64(is_zero) + (src - srcorig);
  }
  _mm512_mask_storeu_epi8(src, init_mask, dst_v);
  src += 64;
  while (true) {
    src_v = _mm512_loadu_epi8(src);
    dst_v = tolower64(src_v);
    is_zero = _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(src, zero_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_storeu_epi8(src, dst_v);
    src += 64;
  }
}

size_t lower64(const char *srcorig, char *dstorig) {
  uintptr_t address = reinterpret_cast<uintptr_t>(srcorig);
  uintptr_t aligned_address = address / 64 * 64;     // round down
  uintptr_t notincluded = address - aligned_address; // [0,64)
  const char *src;
  if(notincluded) {
    src = reinterpret_cast<const char *>(aligned_address);
    __mmask64 init_mask = _cvtu64_mask64((~UINT64_C(0)) << notincluded);
    __m512i src_v = _mm512_maskz_loadu_epi8(init_mask, src);
    __mmask64 is_zero =
        _mm512_mask_cmpeq_epu8_mask(init_mask, src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig - notincluded, zero_mask & init_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_mask_storeu_epi8(dstorig - notincluded, init_mask, dst_v);
    src += 64;
    dstorig += 64 - notincluded;
  } else { // fast path
    src = reinterpret_cast<const char *>(srcorig);
    __m512i src_v = _mm512_loadu_epi8(src);
    __mmask64 is_zero =
        _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig, zero_mask, dst_v);
      return __tzcnt_u64(is_zero);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }

  while (true) {
    __m512i src_v = _mm512_loadu_epi8(src);
    __m512i dst_v = tolower64(src_v);
    __mmask64 is_zero = _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig, zero_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }
}


size_t lower64_short(const char *srcorig, char *dstorig) {
  uintptr_t address = reinterpret_cast<uintptr_t>(srcorig);
  uintptr_t aligned_address = address / 64 * 64;     // round down
  uintptr_t notincluded = address - aligned_address; // [0,64)
  const char *src;
  if(notincluded) {
    src = reinterpret_cast<const char *>(aligned_address);
    __mmask64 init_mask = _cvtu64_mask64((~UINT64_C(0)) << notincluded);
    __m512i src_v = _mm512_maskz_loadu_epi8(init_mask, src);
    __mmask64 is_zero =
        _mm512_mask_cmpeq_epu8_mask(init_mask, src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64_short(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig - notincluded, zero_mask & init_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_mask_storeu_epi8(dstorig - notincluded, init_mask, dst_v);
    src += 64;
    dstorig += 64 - notincluded;
  } else { // fast path
    src = reinterpret_cast<const char *>(srcorig);
    __m512i src_v = _mm512_loadu_epi8(src);
    __mmask64 is_zero =
        _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64_short(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig - notincluded, zero_mask, dst_v);
      return __tzcnt_u64(is_zero);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }

  while (true) {
    __m512i src_v = _mm512_loadu_epi8(src);
    __m512i dst_v = tolower64_short(src_v);
    __mmask64 is_zero = _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig, zero_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }
}

size_t lower64_alt(const char *srcorig, char *dstorig) {
  uintptr_t address = reinterpret_cast<uintptr_t>(srcorig);
  uintptr_t aligned_address = address / 64 * 64;     // round down
  uintptr_t notincluded = address - aligned_address; // [0,64)
  const char *src;
  if(notincluded) {
    src = reinterpret_cast<const char *>(aligned_address);
    __mmask64 init_mask = _cvtu64_mask64((~UINT64_C(0)) << notincluded);
    __m512i src_v = _mm512_maskz_loadu_epi8(init_mask, src);
    __mmask64 is_zero =
        _mm512_mask_cmpeq_epu8_mask(init_mask, src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64_vbmi_and_zen_2uops(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig - notincluded, zero_mask & init_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_mask_storeu_epi8(dstorig - notincluded, init_mask, dst_v);
    src += 64;
    dstorig += 64 - notincluded;
  } else { // fast path
    src = reinterpret_cast<const char *>(srcorig);
    __m512i src_v = _mm512_loadu_epi8(src);
    __mmask64 is_zero =
        _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    __m512i dst_v = tolower64_vbmi_and_zen_2uops(src_v);
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig - notincluded, zero_mask, dst_v);
      return __tzcnt_u64(is_zero);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }

  while (true) {
    __m512i src_v = _mm512_loadu_epi8(src);
    __m512i dst_v = tolower64_vbmi_and_zen_2uops(src_v);
    __mmask64 is_zero = _mm512_cmpeq_epu8_mask(src_v, _mm512_setzero_si512());
    if (is_zero) {
      __mmask64 zero_mask = (is_zero - 1) ^ is_zero;
      _mm512_mask_storeu_epi8(dstorig, zero_mask, dst_v);
      return __tzcnt_u64(is_zero) + (src - srcorig);
    }
    _mm512_storeu_epi8(dstorig, dst_v);
    src += 64;
    dstorig += 64;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <string>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *src = argv[1];
  size_t volume = strlen(src);
  size_t repeat = 1;
  while (volume * repeat < 1000000) {
    repeat *= 10;
  }
  char *c = (char *)malloc(volume+1);
  c[volume] = '\0';
  lower64(src, c);
  printf("%s\n", c);
  printf("short:\n");
  lower64_short(src, c);
  printf("%s\n", c);
  lower(src, c);
  printf("%s\n", c);

  pretty_print(repeat, volume * repeat, "memcpy",
               bench([src, volume, c, repeat]() {
                 for (size_t k = 0; k < repeat; k++)
                   memcpy(c, src, volume);
               }));
  pretty_print(repeat, volume * repeat, "lower",
               bench([src, volume, c, repeat]() {
                 for (size_t k = 0; k < repeat; k++) {
                   lower(src, c);
                 }
               }));
  pretty_print(repeat, volume * repeat, "lower64",
               bench([src, volume, c, repeat]() {
                 for (size_t k = 0; k < repeat; k++) {
                   lower64(src, c);
                 }
               }));

  pretty_print(repeat, volume * repeat, "lower64_short",
               bench([src, volume, c, repeat]() {
                 for (size_t k = 0; k < repeat; k++) {
                   lower64_short(src, c);
                 }
               }));
  pretty_print(repeat, volume * repeat, "lower64_alt",
               bench([src, volume, c, repeat]() {
                 for (size_t k = 0; k < repeat; k++) {
                   lower64_alt(src, c);
                 }
               }));
  free(c);
  return EXIT_SUCCESS;
}
