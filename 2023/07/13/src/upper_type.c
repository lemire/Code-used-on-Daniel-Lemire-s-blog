#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <x86intrin.h> // update if we need to support Windows.

#include "sse_type.h"

typedef struct symbol symbol_t;
struct symbol {
  char name[16];
  uint16_t code;
};

static const symbol_t classes[5] = {
  { { 0 }, 0 }, // reserved
  { { 'I', 'N', 0 }, CLASS_IN },
  { { 'C', 'S', 0 }, CLASS_CS },
  { { 'C', 'H', 0 }, CLASS_CH },
  { { 'H', 'S', 0 }, CLASS_HS }
};

static const symbol_t types[260] = {
  { { 0 }, 0 }, // reserved
  { { 'A', 0 }, TYPE_A },
  { { 'N', 'S', 0 }, TYPE_NS },
  { { 'M', 'D', 0 }, TYPE_MD },
  { { 'M', 'F', 0 }, TYPE_MF },
  { { 'C', 'N', 'A', 'M', 'E', 0 }, TYPE_CNAME },
  { { 'S', 'O', 'A', 0 }, TYPE_SOA },
  { { 'M', 'B', 0 }, TYPE_MB },
  { { 'M', 'G', 0 }, TYPE_MG },
  { { 'M', 'R', 0 }, TYPE_MR },
  // 10
  { { 'N', 'U', 'L', 'L', 0 }, TYPE_NULL },
  { { 'W', 'K', 'S', 0 }, TYPE_WKS },
  { { 'P', 'T', 'R', 0 }, TYPE_PTR },
  { { 'H', 'I', 'N', 'F', 'O', 0 }, TYPE_HINFO },
  { { 'M', 'I', 'N', 'F', 'O', 0 }, TYPE_MINFO },
  { { 'M', 'X', 0 }, TYPE_MX },
  { { 'T', 'X', 'T', 0 }, TYPE_TXT },
  { { 'R', 'P', 0 }, TYPE_RP },
  { { 'A', 'F', 'S', 'D', 'B', 0 }, TYPE_AFSDB },
  { { 'X', '2', '5', 0 }, TYPE_X25 },
  // 20
  { { 'I', 'S', 'D', 'N', 0 }, TYPE_ISDN },
  { { 'R', 'T', 0 }, TYPE_RT },
  { { 'N', 'S', 'A', 'P', 0 }, TYPE_NSAP },
  { { 'N', 'S', 'A', 'P', '-', 'P', 'T', 'R', 0 }, TYPE_NSAP_PTR },
  { { 'S', 'I', 'G', 0 }, TYPE_SIG },
  { { 'K', 'E', 'Y', 0 }, TYPE_KEY },
  { { 'P', 'X', 0 }, TYPE_PX },
  { { 'G', 'P', 'O', 'S', 0 }, TYPE_GPOS },
  { { 'A', 'A', 'A', 'A', 0 }, TYPE_AAAA },
  { { 'L', 'O', 'C', 0 }, TYPE_LOC },
  // 30
  { { 'N', 'X', 'T', 0 }, TYPE_NXT },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 'S', 'R', 'V', 0 }, TYPE_SRV },
  { { 0 }, 0 },
  { { 'N', 'A', 'P', 'T', 'R', 0 }, TYPE_NAPTR },
  { { 'K', 'X', 0 }, TYPE_KX },
  { { 'C', 'E', 'R', 'T', 0 }, TYPE_CERT },
  { { 'A', '6', 0 }, TYPE_A6 },
  { { 'D', 'N', 'A', 'M', 'E', 0 }, TYPE_DNAME },
  // 40
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { 'A', 'P', 'L', 0 }, TYPE_APL },
  { { 'D', 'S', 0 }, TYPE_DS },
  { { 'S', 'S', 'H', 'F', 'P', 0 }, TYPE_SSHFP },
  { { 'I', 'P', 'S', 'E', 'C', 'K', 'E', 'Y', 0 }, TYPE_IPSECKEY },
  { { 'R', 'R', 'S', 'I', 'G', 0 }, TYPE_RRSIG },
  { { 'N', 'S', 'E', 'C', 0 }, TYPE_NSEC },
  { { 'D', 'N', 'S', 'K', 'E', 'Y', 0 }, TYPE_DNSKEY },
  { { 'D', 'H', 'C', 'I', 'D', 0 }, TYPE_DHCID },
  // 50
  { { 'N', 'S', 'E', 'C', '3', 0 }, TYPE_NSEC3 },
  { { 'N', 'S', 'E', 'C', '3', 'P', 'A', 'R', 'A', 'M', 0 }, TYPE_NSEC3PARAM },
  { { 'T', 'L', 'S', 'A', 0 }, TYPE_TLSA },
  { { 'S', 'M', 'I', 'M', 'E', 'A', 0 }, TYPE_SMIMEA },
  { { '0' }, 0 },
  { { 'H', 'I', 'P', 0 }, TYPE_HIP },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 'C', 'D', 'S', 0 }, TYPE_CDS },
  // 60
  { { 'C', 'D', 'N', 'S', 'K', 'E', 'Y', 0 }, TYPE_CDNSKEY },
  { { 'O', 'P', 'E', 'N', 'P', 'G', 'P', 'K', 'E', 'Y', 0 }, TYPE_OPENPGPKEY },
  { { 'C', 'S', 'Y', 'N', 'C', 0 }, TYPE_CSYNC },
  { { 'Z', 'O', 'N', 'E', 'M', 'D', 0 }, TYPE_ZONEMD },
  { { 'S', 'V', 'C', 'B', 0 }, TYPE_SVCB },
  { { 'H', 'T', 'T', 'P', 'S', 0 }, TYPE_HTTPS },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 70
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 80
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 90
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 'S', 'P', 'F', 0 }, TYPE_SPF },
  // 100
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { 'N', 'I', 'D', 0 }, TYPE_NID },
  { { 'L', '3', '2', 0 }, TYPE_L32 },
  { { 'L', '6', '4', 0 }, TYPE_L64 },
  { { 'L', 'P', 0 }, TYPE_LP },
  { { 'E', 'U', 'I', '4', '8', 0 }, TYPE_EUI48 },
  { { 'E', 'U', 'I', '6', '4', 0 }, TYPE_EUI64 },
  // 110
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 120
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 130
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 140
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 150
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 160
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 170
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 180
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 190
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 200
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 210
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 220
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 230
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 240
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  // 250
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 'U', 'R', 'I', 0 }, TYPE_URI },
  { { 'C', 'A', 'A', 0 }, TYPE_CAA },
  { { 'A', 'V', 'C', 0 }, TYPE_AVC },
  { { 'D', 'L', 'V', 0 }, TYPE_DLV },
};


#define C(n) &classes[n]
#define T(n) &types[n]

// maps to type code, except for dlv
static const symbol_t *hash_to_symbol[256] = {
    T(0),   T(0),   T(0),   T(0),   T(0),  T(44),   T(0),   T(3),
    T(0),   T(0),   T(0),   T(0),  T(11),   T(0),  T(42),   T(0),
    T(0),   T(0),   T(0),   T(0),   T(0),  T(62),   T(0),   T(0),
    T(0),  T(99),  T(25),   T(0),  T(53),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0),  T(50),   T(0),   T(0),   T(0),
    T(0),  T(39),   T(0),  T(21),   T(0),   T(5),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(1),   T(0),   T(0),
    C(1),   T(0), T(105),  T(49),   T(0),  T(59),   T(0),   T(29),
    T(0),  T(20),   T(0),   T(6),   T(0),   T(0),   T(0),   C(3),
    T(0),  T(63),   T(0),   T(0),   T(0),   C(2),  T(43),  T(37),
    T(0),   C(4),   T(0),   T(0),  T(45), T(104),   T(2),   T(0),
   T(23),  T(55),   T(0),  T(24),   T(0),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(7),   T(0),   T(0),   T(0),  T(12),
    T(0),   T(0),  T(60),   T(0),   T(0),  T(36),  T(10),  T(15),
    T(0),  T(26),   T(0),   T(0),  T(19),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),  T(65),   T(0),   T(8),   T(0), T(108),
    T(0),  T(38),   T(0),   T(9),   T(0),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0),  T(46),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(0),  T(27),  T(48),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),
    T(0),   T(0),  T(28),   T(4),  T(51),   T(0),   T(0),  T(30),
    T(0), T(106),   T(0),   T(0),  T(16),  T(64),   T(0),   T(0),
    T(0),   T(0), T(257),   T(0),   T(0),   T(0),   T(0),   T(0),
  T(256),   T(0),   T(0),   T(0),   T(0),  T(22),   T(0),   T(0),
    T(0),  T(33),   T(0),  T(61),   T(0),  T(52),   T(0),   T(0),
  T(259),   T(0),   T(0),   T(0),  T(14),   T(0),   T(0),   T(0),
   T(13),   T(0),   T(0),   T(0),   T(0),   T(0), T(107),   T(0),
    T(0),  T(18),   T(0),  T(17),   T(0),   T(0),  T(35),   T(0),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),   T(0),
    T(0),   T(0),   T(0),   T(0), T(258),   T(0),   T(0), T(109),
    T(0),   T(0),   T(0),   T(0),   T(0),   T(0),  T(47),   T(0)
};

static const int8_t zero_masks__[48] = {
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0
};

static const int8_t *zero_masks = &zero_masks__[31];

static inline uint8_t hash(uint64_t val)
{
  uint32_t val32 = (uint32_t)((val >> 32) ^ val);
  // magic value generated using upper_hash.c, rerun when adding types
  return (uint8_t)((val32 * (uint64_t)3523264710) >> 32);
}

bool swar_upper_len(const char *str, size_t len, uint16_t *type)
{
  uint64_t input0, input1;
  const uint64_t letter_mask = 0x4040404040404040ull;

  memcpy(&input0, str,   8);
  memcpy(&input1, str+8, 8);

  // convert to upper case
  input0 = input0 & ~((input0 & letter_mask) >> 1);
  input1 = input1 & ~((input1 & letter_mask) >> 1);

  // sanitize input
  len &= 0x1f;
  const uint8_t *zero_mask = zero_masks - len;
  uint64_t zero_mask0, zero_mask1;

  memcpy(&zero_mask0, zero_mask,     8);
  memcpy(&zero_mask1, zero_mask + 8, 8);

  input0 &= zero_mask0;
  input1 &= zero_mask1;

  // input sanitized and upper case
  const uint8_t index = hash(input0);
  const symbol_t *symbol = hash_to_symbol[index];

  *type = symbol->code;
  uint64_t compar0, compar1;
  memcpy(&compar0, symbol->name,     8);
  memcpy(&compar1, symbol->name + 8, 8);

  return ((input0 ^ compar0) | (input1 ^ compar1)) == 0;
}

bool sse_upper_len(const char *str, size_t len, uint16_t *type)
{
  __m128i input = _mm_loadu_si128((const __m128i *)str);
  const __m128i letter_mask =
    _mm_srli_epi32(_mm_and_si128(input, _mm_set1_epi8(0x40)), 1);

  // convert to upper case
  input = _mm_andnot_si128(letter_mask, input);

  // sanitize input
  len &= 0x1f;
  const __m128i zero_mask =
    _mm_loadu_si128((const __m128i *)(zero_masks - len));
  input = _mm_and_si128(input, zero_mask);

  const uint8_t index = hash((uint64_t)_mm_cvtsi128_si64(input));
  const symbol_t *symbol = hash_to_symbol[index];

  *type = symbol->code;
  const __m128i compar = _mm_loadu_si128((const __m128i *)symbol->name);
  const __m128i xorthem = _mm_xor_si128(compar, input);

  return _mm_test_all_zeros(xorthem, xorthem);
}
