#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <x86intrin.h>

#include <string.h>
uint8_t hash(uint64_t val) {

  uint32_t val32 = ((val >> 32) ^ val);
  return (val32 * (uint64_t)3523216699) >> 32;
}

std::string str_tolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); } // correct
  );
  return s;
}
char to_lower(char x) {
  if (x == 0) {
    return 0;
  }
  return x | 0x20;
}
int main() {
  std::vector<std::string> t = {
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
  std::vector<std::string> allstring;
  for (size_t i = 0; i < 256; i++) {
    allstring.push_back("{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}");
  }

  for (const std::string s : t) {
    std::string g = str_tolower(s);
    uint64_t val = 0;
    memcpy(&val, g.data(), (g.size() > 8 ? 8 : g.size()));
    std::cout << "TYPE_" << s << " = " << int(hash(val)) << "," << std::endl;


    char buffer[16]{};
    std::ostringstream os;

    memcpy(buffer, s.data(), s.size());
    os << "{";
    os << int(to_lower(buffer[0]));
    for (size_t i = 1; i < 16; i++) {
      os << ", ";
      os << int(to_lower(buffer[i]));
    }
    os << "}";

    allstring[hash(val)] = os.str();
  }
  printf("final\n");

  for (size_t i = 0; i < 256; i++) {
    std::cout << allstring[i] << ",\n";
  }
}