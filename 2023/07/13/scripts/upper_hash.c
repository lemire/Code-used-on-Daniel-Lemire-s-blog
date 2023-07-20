/*
 * hash.c -- Calculate perfect hash for TYPEs and CLASSes
 *
 * Copyright (c) 2023, NLnet Labs. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

typedef struct tuple tuple_t;
struct tuple {
  char name[16];
  uint16_t code;
  bool type;
};

static const tuple_t types_and_classes[] = {
  // classes
  { "IN", 1, false },
  { "CS", 2, false },
  { "CH", 3, false },
  { "HS", 4, false },
  // types
  { "A", 1, true },
  { "NS", 2, true },
  { "MD", 3, true },
  { "MF", 4, true },
  { "CNAME", 5, true },
  { "SOA", 6, true },
  { "MB", 7, true },
  { "MG", 8, true },
  { "MR", 9, true },
  { "NULL", 10, true },
  { "WKS", 11, true },
  { "PTR", 12, true },
  { "HINFO", 13, true },
  { "MINFO", 14, true },
  { "MX", 15, true },
  { "TXT", 16, true },
  { "RP", 17, true },
  { "AFSDB", 18, true },
  { "X25", 19, true },
  { "ISDN", 20, true },
  { "RT", 21, true },
  { "NSAP", 22, true },
  { "NSAP-PTR", 23, true },
  { "SIG", 24, true },
  { "KEY", 25, true },
  { "PX", 26, true },
  { "GPOS", 27, true },
  { "AAAA", 28, true },
  { "LOC", 29, true },
  { "NXT", 30, true },
  { "SRV", 33, true },
  { "NAPTR", 35, true },
  { "KX", 36, true },
  { "CERT", 37, true },
  { "A6", 38, true },
  { "DNAME", 39, true },
  { "APL", 42, true },
  { "DS", 43, true },
  { "SSHFP", 44, true },
  { "IPSECKEY", 45, true },
  { "RRSIG", 46, true },
  { "NSEC", 47, true },
  { "DNSKEY", 48, true },
  { "DHCID", 49, true },
  { "NSEC3", 50, true },
  { "NSEC3PARAM", 51, true },
  { "TLSA", 52, true },
  { "SMIMEA", 53, true },
  { "HIP", 55, true },
  { "CDS", 59, true },
  { "CDNSKEY", 60, true },
  { "OPENPGPKEY", 61, true },
  { "CSYNC", 62, true },
  { "ZONEMD", 63, true },
  { "SVCB", 64, true },
  { "HTTPS", 65, true },
  { "SPF", 99, true },
  { "NID", 104, true },
  { "L32", 105, true },
  { "L64", 106, true },
  { "LP", 107, true },
  { "EUI48", 108, true },
  { "EUI64", 109, true },
  { "URI", 256, true },
  { "CAA", 257, true },
  { "AVC", 258, true },
  { "DLV", 32769, true }
};

const uint64_t original_magic = 3523216699ull; // original hash from hash.cpp

static uint8_t hash(uint64_t magic, uint64_t value)
{
  uint32_t value32 = ((value >> 32) ^ value);
  return (value32 * magic) >> 32;
}

static void print_table(uint64_t magic)
{
  struct { uint16_t code; bool type; } keys[256];
  memset(keys, 0, sizeof(keys));
  const size_t n = sizeof(types_and_classes)/sizeof(types_and_classes[0]);
  for (size_t i=0; i < n; i++) {
    uint64_t value;
    memcpy(&value, types_and_classes[i].name, 8);
    uint8_t key = hash(magic, value);
    keys[key].code = types_and_classes[i].code;
    keys[key].type = types_and_classes[i].type;
  }

  printf("static const symbol_t *hash_to_symbol[256] = {\n");
  for (size_t i=0; i < 256; ) {
    for (size_t j=i+8; i < j; i++) {
      uint16_t code = keys[i].code;
      char macro = !code || keys[i].type ? 'T' : 'C';
      printf("%c(%u), ", macro, code);
    }
    printf("\n");
  }
  printf("};\n");
}

int main(int argc, char *argv[])
{
  const size_t n = sizeof(types_and_classes)/sizeof(types_and_classes[0]);
  for (uint64_t magic = original_magic; magic < UINT64_MAX; magic++) {
    size_t i;
    uint16_t keys[256] = { 0 };
    for (i=0; i < n; i++) {
      uint64_t value;
      memcpy(&value, types_and_classes[i].name, 8);

      uint8_t key = hash(magic, value);
      if (keys[key])
        break;
      keys[key] = 1;
    }

    if (i == n) {
      printf("i: %zu, magic: %" PRIu64 "\n", i, magic);
      for (i=0; i < n; i++) {
        uint64_t value;
        memcpy(&value, types_and_classes[i].name, 8);
        uint8_t key = hash(magic, value);
        printf("TYPE_%s: %" PRIu8 " (%" PRIu16 ")\n", types_and_classes[i].name, key, types_and_classes[i].code);
      }
      print_table(magic);
      return 0;
    }
  }

  printf("no magic value\n");
  return 1;
}
