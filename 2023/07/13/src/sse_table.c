#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <x86intrin.h> // update if we need to support Windows.

#include "sse_type.h"

typedef struct identifier identifier_t;
struct identifier {
  char name[16];
  uint16_t code;
};

static const identifier_t classes[5] = {
  { { 0 }, 0 }, // reserved
  { { 'i', 'n', 0 }, CLASS_IN },
  { { 'c', 's', 0 }, CLASS_CS },
  { { 'c', 'h', 0 }, CLASS_CH },
  { { 'h', 's', 0 }, CLASS_HS }
};

static const identifier_t types[260] = {
  { { 0 }, 0 }, // reserved
  { { 'a', 0 }, TYPE_A },
  { { 'n', 's', 0 }, TYPE_NS },
  { { 'm', 'd', 0 }, TYPE_MD },
  { { 'm', 'f', 0 }, TYPE_MF },
  { { 'c', 'n', 'a', 'm', 'e', 0 }, TYPE_CNAME },
  { { 's', 'o', 'a', 0 }, TYPE_SOA },
  { { 'm', 'b', 0 }, TYPE_MB },
  { { 'm', 'g', 0 }, TYPE_MG },
  { { 'm', 'r', 0 }, TYPE_MR },
  // 10
  { { 'n', 'u', 'l', 'l', 0 }, TYPE_NULL },
  { { 'w', 'k', 's', 0 }, TYPE_WKS },
  { { 'p', 't', 'r', 0 }, TYPE_PTR },
  { { 'h', 'i', 'n', 'f', 'o', 0 }, TYPE_HINFO },
  { { 'm', 'i', 'n', 'f', 'o', 0 }, TYPE_MINFO },
  { { 'm', 'x', 0 }, TYPE_MX },
  { { 't', 'x', 't', 0 }, TYPE_TXT },
  { { 'r', 'p', 0 }, TYPE_RP },
  { { 'a', 'f', 's', 'd', 'b', 0 }, TYPE_AFSDB },
  { { 'x', '2', '5', 0 }, TYPE_X25 },
  // 20
  { { 'i', 's', 'd', 'n', 0 }, TYPE_ISDN },
  { { 'r', 't', 0 }, TYPE_RT },
  { { 'n', 's', 'a', 'p', 0 }, TYPE_NSAP },
  { { 'n', 's', 'a', 'p', '-', 'p', 't', 'r', 0 }, TYPE_NSAP_PTR },
  { { 's', 'i', 'g', 0 }, TYPE_SIG },
  { { 'k', 'e', 'y', 0 }, TYPE_KEY },
  { { 'p', 'x', 0 }, TYPE_PX },
  { { 'g', 'p', 'o', 's', 0 }, TYPE_GPOS },
  { { 'a', 'a', 'a', 'a', 0 }, TYPE_AAAA },
  { { 'l', 'o', 'c', 0 }, TYPE_LOC },
  // 30
  { { 'n', 'x', 't', 0 }, TYPE_NXT },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 's', 'r', 'v', 0 }, TYPE_SRV },
  { { 0 }, 0 },
  { { 'n', 'a', 'p', 't', 'r', 0 }, TYPE_NAPTR },
  { { 'k', 'x', 0 }, TYPE_KX },
  { { 'c', 'e', 'r', 't', 0 }, TYPE_CERT },
  { { 'a', '6', 0 }, TYPE_A6 },
  { { 'd', 'n', 'a', 'm', 'e', 0 }, TYPE_DNAME },
  // 40
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { 'a', 'p', 'l', 0 }, TYPE_APL },
  { { 'd', 's', 0 }, TYPE_DS },
  { { 's', 's', 'h', 'f', 'p', 0 }, TYPE_SSHFP },
  { { 'i', 'p', 's', 'e', 'c', 'k', 'e', 'y', 0 }, TYPE_IPSECKEY },
  { { 'r', 'r', 's', 'i', 'g', 0 }, TYPE_RRSIG },
  { { 'n', 's', 'e', 'c', 0 }, TYPE_NSEC },
  { { 'd', 'n', 's', 'k', 'e', 'y', 0 }, TYPE_DNSKEY },
  { { 'd', 'h', 'c', 'i', 'd', 0 }, TYPE_DHCID },
  // 50
  { { 'n', 's', 'e', 'c', '3', 0 }, TYPE_NSEC3 },
  { { 'n', 's', 'e', 'c', '3', 'p', 'a', 'r', 'a', 'm', 0 }, TYPE_NSEC3PARAM },
  { { 't', 'l', 's', 'a', 0 }, TYPE_TLSA },
  { { 's', 'm', 'i', 'm', 'e', 'a', 0 }, TYPE_SMIMEA },
  { { '0' }, 0 },
  { { 'h', 'i', 'p', 0 }, TYPE_HIP },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 0 }, 0 },
  { { 'c', 'd', 's', 0 }, TYPE_CDS },
  // 60
  { { 'c', 'd', 'n', 's', 'k', 'e', 'y', 0 }, TYPE_CDNSKEY },
  { { 'o', 'p', 'e', 'n', 'p', 'g', 'p', 'k', 'e', 'y', 0 }, TYPE_OPENPGPKEY },
  { { 'c', 's', 'y', 'n', 'c', 0 }, TYPE_CSYNC },
  { { 'z', 'o', 'n', 'e', 'm', 'd', 0 }, TYPE_ZONEMD },
  { { 's', 'v', 'c', 'b', 0 }, TYPE_SVCB },
  { { 'h', 't', 't', 'p', 's', 0 }, TYPE_HTTPS },
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
  { { 's', 'p', 'f', 0 }, TYPE_SPF },
  // 100
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { '0' }, 0 },
  { { 'n', 'i', 'd', 0 }, TYPE_NID },
  { { 'l', '3', '2', 0 }, TYPE_L32 },
  { { 'l', '6', '4', 0 }, TYPE_L64 },
  { { 'l', 'p', 0 }, TYPE_LP },
  { { 'e', 'u', 'i', '4', '8', 0 }, TYPE_EUI48 },
  { { 'e', 'u', 'i', '6', '4', 0 }, TYPE_EUI64 },
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
  { { 'u', 'r', 'i', 0 }, TYPE_URI },
  { { 'c', 'a', 'a', 0 }, TYPE_CAA },
  { { 'a', 'v', 'c', 0 }, TYPE_AVC },
  { { 'd', 'l', 'v', 0 }, TYPE_DLV },
};

// Inspired by is-prefix-of-string-table
// https://trent.me/is-prefix-of-string-in-table/
typedef struct string_table string_table_t;
struct string_table {
  uint8_t unique_index[16];
  uint8_t unique_bytes[16];
  const identifier_t *identifiers[16];
};

#define C(code) (&classes[code])
#define T(code) (&types[code])

#define empty_ids \
  { { 0 }, { 0 }, { no_id, no_id, no_id, no_id, no_id, no_id, no_id, no_id, \
                    no_id, no_id, no_id, no_id, no_id, no_id, no_id, no_id }}

// every string is guarantueed to be followed by a delimiter
// all letters are guarantueed to be lower cased
static const string_table_t tables[26] = {
  { // A: A, A6, AAAA, AFSDB, APL, AVC
    { 1, 1, 1, 1, 1, 1, 0 },
    { '\0', '6', 'a', 'f', 'p', 'l', 'a', 0 },
    { T(1), T(38), T(28), T(18), T(42), T(258), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // B: -
    { 0, 0 },
    { 'b', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // C: CDNSKEY, CERT, CSYNC, CDS, CAA, CNAME, CS, CH
    { 2, 2, 2, 2, 1, 1, 1, 1, 0 },
    { 'n', 'r', 'y', 's', 'a', 'n', 's', 'h', 'c', 0 },
    { T(60), T(37), T(62), T(59), T(257), T(5), C(2), C(3),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // D: DNAME, DS, DNSKEY, DHCID, DLV
    { 2, 2, 2, 2, 2, 0 },
    { 'a', '\0', 's', 'c', 'v', 'd', 0 },
    { T(39), T(43), T(48), T(49), T(259), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // E: EUI48, EUI64
    { 3, 3, 0 },
    { '4', '6', 'e', 0 },
    { T(108), T(109), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // F: -
    { 0, 0 },
    { 'f', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0)  }
  },
  { // G: GPOS
    { 0, 0 },
    { 'g', 0 },
    { T(27), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // H: HINFO, HIP, HTTPS, HS
    { 2, 2, 2, 1, 0 },
    { 'n', 'p', 't', 's', 0 },
    { T(13), T(55), T(65), C(4), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // I: IPSECKEY, ISDN, IN
    { 1, 1, 1, 0 },
    { 'p', 's', 'n', 'i', 0 },
    { T(45), T(20), C(1), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // J: -
    { 0 },
    { 'j', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // K: KEY, KX
    { 1, 1, 0 },
    { 'e', 'x', 'k', 0 },
    { T(25), T(36), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // L: L32, L64, LOC, LP
    { 1, 1, 1, 1, 0 },
    { '3', '6', 'o', 'p', 'l', 0 },
    { T(105), T(106), T(29), T(107), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // M: MB, MD, MF, MG, MINFO, MR, MX
    { 1, 1, 1, 1, 1, 1, 1, 0 },
    { 'b', 'd', 'f', 'g', 'i', 'r', 'x', 'm', 0 },
    { T(7), T(3), T(4), T(8), T(14), T(9), T(15), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // N: NS, NID, NXT, NAPTR, NSAP-PTR, NSEC3PARAM, NSEC3, NSAP, NSEC, NULL
    { 2, 2, 2, 2, 4, 5, 4, 3, 3, 3, 0 },
    { '\0', 'd', 't', 'p', '-',  'p', '3', 'p', 'c', 'l', 'n', 0 },
    { T(2), T(104), T(30), T(35), T(23), T(51), T(50), T(22),
      T(47), T(10), 0, 0, 0, 0, 0, 0 }
  },
  { // O: OPENPGPKEY
    { 2, 0 },
    { 'e', 'o', 0 },
    { T(61), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // P: PTR, PX
    { 1, 1, 0 },
    { 't', 'x', 'p', 0 },
    { T(12), T(26), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // Q: -
    { 0 },
    { 'q', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // R: RP, RRSIG, RT
    { 1, 1, 1, 0 },
    { 'p', 'r', 't', 'r', 0 },
    { T(17), T(46), T(21), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // S: SIG, SMIMEA, SOA, SPF, SRV, SSHFP, SVCB
    { 1, 1, 1, 1, 1, 1, 1, 0 },
    { 'i', 'm', 'o', 'p', 'r', 's', 'v', 's', 0 },
    { T(24), T(53), T(6), T(99), T(33), T(44), T(64), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // T: TXT, TLSA
    { 1, 1, 0 },
    { 'x', 'l', 't', 0 },
    { T(16), T(52), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // U: URI
    { 1, 0 },
    { 'r', 'u', 0 },
    { T(256), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // V: -
    { 0 },
    { 'v', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // W: WKS
    { 1, 0 },
    { 'k', 'w', 0 },
    { T(11), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // X: X25
    { 1, 0 },
    { '2', 'x', 0 },
    { T(19), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // Y: -
    { 0 },
    { 'y', 0 },
    { T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  },
  { // Z: ZONEMD
    { 1, 0 },
    { 'o', 'z', 0 },
    { T(63), T(0), T(0), T(0), T(0), T(0), T(0), T(0),
      T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0) }
  }
};

static const uint8_t indexes[256] = {
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x00 - 0x07
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x08 - 0x0f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x10 - 0x17
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x18 - 0x1f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x20 - 0x27
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x28 - 0x2f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x30 - 0x37
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x38 - 0x3f
  // upper case
   0,  0,  1,  2,  3,  4,  5,  6,  //  0x40 - 0x47
   7,  8,  9, 10, 11, 12, 13, 14,  //  0x48 - 0x4f
  15, 16, 17, 18, 19, 20, 21, 22,  //  0x50 - 0x57
  23, 24, 25,  0,  0,  0,  0,  0,  //  0x58 - 0x5f
  // lower case
   0,  0,  1,  2,  3,  4,  5,  6,  //  0x60 - 0x67
   7,  8,  9, 10, 11, 12, 13, 14,  //  0x68 - 0x6f
  15, 16, 17, 18, 19, 20, 21, 22,  //  0x70 - 0x77
  23, 24, 25,  0,  0,  0,  0,  0,  //  0x78 - 0x7f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x80 - 0x87
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x88 - 0x8f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x90 - 0x97
   0,  0,  0,  0,  0,  0,  0,  0,  //  0x98 - 0x9f
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xa0 - 0xa7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xa8 - 0xaf
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xb0 - 0xb7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xb8 - 0xbf
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xc0 - 0xc7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xc8 - 0xcf
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xd0 - 0xd7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xd8 - 0xdf
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xe0 - 0xe7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xe8 - 0xef
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xf0 - 0xf7
   0,  0,  0,  0,  0,  0,  0,  0,  //  0xf8 - 0xff
};

static int8_t zero_masks[32] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                         0,  0,  0,  0,  0,  -1, -1, -1, -1, -1, -1,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

bool sse_table(const char *type_string, uint16_t *type)
{
  // FIXME: perhaps a computed goto (-fjump-table) is better?
  const uint8_t key = indexes[ (uint8_t)*type_string ];
  const __m128i sort = _mm_loadu_si128((const __m128i *)tables[key].unique_index);
  const __m128i keys = _mm_loadu_si128((const __m128i *)tables[key].unique_bytes);

  __m128i input = _mm_loadu_si128((const __m128i *)type_string);
  // RRTYPEs consist of [0-9a-zA-Z-] (unofficially, no other values are in use)
  // 0x2d        : hyphen : 0b0010_1101
  // 0x30 - 0x39 :  0 - 9 : 0b0011_0000 - 0b0011_1001
  // 0x41 - 0x4f :  A - O : 0b0100_0001 - 0b0100_1111
  // 0x50 - 0x5a :  P - Z : 0b0101_0000 - 0b0101_1010
  // 0x61 - 0x6f :  a - o : 0b0110_0001 - 0b0110_1111
  // 0x70 - 0x7a :  p - z : 0b0111_0000 - 0b0111_1010

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
  __m128i delimiters = _mm_setr_epi8(
    0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x28, 0x09, 0x0a, 0x3b, 0x00, 0x0d, 0x00, 0x00);
  __m128i mask = _mm_setr_epi8(
    -33, -1, -1, -1, -1, -1, -1, -1, -1, -33, -1, -1, -1, -1, -1, -1);

  // construct delimiter pattern for comparison
  __m128i pattern = _mm_shuffle_epi8(delimiters, input);
  // clear 5th bit (match 0x20 with 0x00, match 0x29 using 0x09)
  __m128i inputc = _mm_and_si128(input, _mm_shuffle_epi8(mask, input));
  int bitmask = _mm_movemask_epi8(_mm_cmpeq_epi8(inputc, pattern));
  uint16_t length = (uint16_t)__builtin_ctz((unsigned int)bitmask);
  __m128i zero_mask = _mm_loadu_si128((__m128i *)(zero_masks + 16 - length));
  // convert upper case to lower case (dash is 0x20, digits are 0x30 - 0x39)
  __m128i inputlc = _mm_or_si128(input, _mm_set1_epi8(0x20));
  input = _mm_andnot_si128(zero_mask, inputlc);
  // now we have a sanitized input. meaning, all characters letters are
  // lower cased and string is followed by zeros

  const __m128i sorted = _mm_shuffle_epi8(input, sort);
  const __m128i matches = _mm_cmpeq_epi8(sorted, keys);

  int index = __builtin_ctz((uint16_t)_mm_movemask_epi8(matches));
  const identifier_t *id = tables[key].identifiers[index];
  *type = id->code;

  const __m128i compar = _mm_loadu_si128((const __m128i *)id->name);
  const __m128i xorthem = _mm_xor_si128(compar, input);

  return _mm_test_all_zeros(xorthem, xorthem);
}
