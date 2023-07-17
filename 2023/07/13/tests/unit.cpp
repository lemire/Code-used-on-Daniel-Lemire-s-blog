#include <arpa/inet.h>
#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <omp.h>
#include <string>
#include <string_view>
#include <vector>
#include <x86intrin.h>

extern "C" {
#include "sse_type.h"
}

std::vector<std::string> strings = {
    "A",          "A6",      "AAAA",     "AFSDB",      "APL",    "CAA",
    "CDS",        "CDNSKEY", "CERT",     "CH",         "CNAME",  "CS",
    "CSYNC",      "DHCID",   "DLV",      "DNAME",      "DNSKEY", "DS",
    "EUI48",      "EUI64",   "GPOS",     "HINFO",      "HIP",    "HS",
    "HTTPS",      "IN",      "IPSECKEY", "ISDN",       "KEY",    "KX",
    "L32",        "L64",     "LOC",      "LP",         "MB",     "MD",
    "MF",         "MG",      "MINFO",    "MR",         "MX",     "NAPTR",
    "NID",        "NS",      "NSAP",     "NSAP-PTR",   "NSEC",   "NSEC3",
    "NSEC3PARAM", "NULL",    "NXT",      "OPENPGPKEY", "PTR",    "PX",
    "RP",         "RRSIG",   "RT",       "SIG",        "SMIMEA", "SOA",
    "SPF",        "SRV",     "SSHFP",    "SVCB",       "TLSA",   "TXT",
    "URI",        "WKS",     "X25",      "ZONEMD"};
std::vector<uint16_t> expected = {
    TYPE_A,        TYPE_A6,         TYPE_AAAA,    TYPE_AFSDB,      TYPE_APL,
    TYPE_CAA,      TYPE_CDS,        TYPE_CDNSKEY, TYPE_CERT,       CLASS_CH,
    TYPE_CNAME,    CLASS_CS,        TYPE_CSYNC,   TYPE_DHCID,      TYPE_DLV,
    TYPE_DNAME,    TYPE_DNSKEY,     TYPE_DS,      TYPE_EUI48,      TYPE_EUI64,
    TYPE_GPOS,     TYPE_HINFO,      TYPE_HIP,     CLASS_HS,        TYPE_HTTPS,
    CLASS_IN,      TYPE_IPSECKEY,   TYPE_ISDN,    TYPE_KEY,        TYPE_KX,
    TYPE_L32,      TYPE_L64,        TYPE_LOC,     TYPE_LP,         TYPE_MB,
    TYPE_MD,       TYPE_MF,         TYPE_MG,      TYPE_MINFO,      TYPE_MR,
    TYPE_MX,       TYPE_NAPTR,      TYPE_NID,     TYPE_NS,         TYPE_NSAP,
    TYPE_NSAP_PTR, TYPE_NSEC,       TYPE_NSEC3,   TYPE_NSEC3PARAM, TYPE_NULL,
    TYPE_NXT,      TYPE_OPENPGPKEY, TYPE_PTR,     TYPE_PX,         TYPE_RP,
    TYPE_RRSIG,    TYPE_RT,         TYPE_SIG,     TYPE_SMIMEA,     TYPE_SOA,
    TYPE_SPF,      TYPE_SRV,        TYPE_SSHFP,   TYPE_SVCB,       TYPE_TLSA,
    TYPE_TXT,      TYPE_URI,        TYPE_WKS,     TYPE_X25,        TYPE_ZONEMD};
bool simple_test() {
  for (size_t i = 0; i < strings.size(); i++) {
    std::cout << " =----= " << std::endl;
    std::string main = strings[i];
    std::cout << "testing: " << main << std::endl;

    main.resize(16);
    uint16_t type;

    if (!sse_type(main.data(), &type)) {
      printf("expected success\n");
      return false;
    }
    if (type != expected[i]) {
      printf("bad type\n");
      return false;
    }
    if (!sse_table(main.data(), &type)) {
      printf("expected success\n");
      return false;
    }
    if (type != expected[i]) {
      printf("bad type\n");
      return false;
    }
  }
  printf("SUCCESS\n");
  return true;
}

int main() { return (simple_test()) ? EXIT_SUCCESS : EXIT_FAILURE; }
