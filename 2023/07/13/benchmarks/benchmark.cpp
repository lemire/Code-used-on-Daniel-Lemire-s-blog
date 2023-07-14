
#include "performancecounters/benchmarker.h"
#include <algorithm>
#include <cctype>
#include <charconv>
#include <random>
#include <vector>

extern "C" {
#include "sse_type.h"
}

#define CONTIGUOUS (1 << 0)

static const uint8_t contiguous[256] = {
    // 0x00 = "\0"
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x00 - 0x07
    // 0x09 = "\t", 0x0a = "\n", 0x0d = "\r"
    0x01, 0x40, 0x04, 0x01, 0x01, 0x40, 0x01, 0x01, // 0x08 - 0x0f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x10 - 0x17
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x18 - 0x1f
    // 0x20 = " ", 0x22 = "\""
    0x40, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x20 - 0x27
    // 0x28 = "(", 0x29 = ")"
    0x10, 0x20, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x28 - 0x2f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x30 - 0x37
    // 0x3b = ";"
    0x01, 0x01, 0x01, 0x80, 0x01, 0x01, 0x01, 0x01, // 0x38 - 0x3f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x40 - 0x47
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x48 - 0x4f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x50 - 0x57
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x58 - 0x5f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x60 - 0x67
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x68 - 0x6f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x70 - 0x77
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x78 - 0x7f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x80 - 0x87
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x88 - 0x8f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x90 - 0x97
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0x98 - 0x9f
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xa0 - 0xa7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xa8 - 0xaf
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xb0 - 0xb7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xb8 - 0xbf
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xc0 - 0xc7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xc8 - 0xcf
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xd0 - 0xd7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xd8 - 0xdf
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xe0 - 0xe7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xe8 - 0xef
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 0xf8 - 0xf7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01  // 0xf8 - 0xff
};

static bool is_separator(char c) {
  return contiguous[uint8_t(c)] != CONTIGUOUS;
}

std::vector<std::string> strings = {
    "A",          "A6",    "AAAA",     "AFSDB",      "APL",    "CAA",
    "CDNSKEY",    "CDS",   "CERT",     "CH",         "CNAME",  "CS",
    "CSYNC",      "DHCID", "DLV",      "DNAME",      "DNSKEY", "DS",
    "EUI48",      "EUI64", "GPOS",     "HINFO",      "HIP",    "HS",
    "HTTPS",      "IN",    "IPSECKEY", "ISDN",       "KEY",    "KX",
    "L32",        "L64",   "LOC",      "LP",         "MB",     "MD",
    "MF",         "MG",    "MINFO",    "MR",         "MX",     "NAPTR",
    "NID",        "NS",    "NSAP",     "NSAP-PTR",   "NSEC",   "NSEC3",
    "NSEC3PARAM", "NULL",  "NXT",      "OPENPGPKEY", "PTR",    "PX",
    "RP",         "RRSIG", "RT",       "SIG",        "SMIMEA", "SOA",
    "SPF",        "SRV",   "SSHFP",    "SVCB",       "TLSA",   "TXT",
    "URI",        "WKS",   "X25",      "ZONEMD"};

std::vector<std::string> lowerstrings = {
    "a",          "a6",    "aaaa",     "afsdb",      "apl",    "caa",
    "cdnskey",    "cds",   "cert",     "ch",         "cname",  "cs",
    "csync",      "dhcid", "dlv",      "dname",      "dnskey", "ds",
    "eui48",      "eui64", "gpos",     "hinfo",      "hip",    "hs",
    "https",      "in",    "ipseckey", "isdn",       "key",    "kx",
    "l32",        "l64",   "loc",      "lp",         "mb",     "md",
    "mf",         "mg",    "minfo",    "mr",         "mx",     "naptr",
    "nid",        "ns",    "nsap",     "nsap-ptr",   "nsec",   "nsec3",
    "nsec3param", "null",  "nxt",      "openpgpkey", "ptr",    "px",
    "rp",         "rrsig", "rt",       "sig",        "smimea", "soa",
    "spf",        "srv",   "sshfp",    "svcb",       "tlsa",   "txt",
    "uri",        "wks",   "x25",      "zonemd"};

// compare and lookup_symbol and the initial approach in simdzone
static int compare(const void *p1, const void *p2) {
  const char *data(reinterpret_cast<const char *>(p1));
  const std::string &target(*reinterpret_cast<const std::string *>(p2));
  int r;
  if ((r = strncasecmp(data, target.data(), target.size())) != 0) {
    return r;
  }
  return !is_separator(data[target.size()]);
}

// essentially what simdzone had, a binary search.
static const std::string *lookup_symbol(const char *input) {
  return reinterpret_cast<const std::string *>(bsearch(
      input, strings.data(), strings.size(), sizeof(std::string), compare));
}

// Silly trie approach
__attribute__((noinline)) static int simple_trie(const std::string &s) {
  switch (s[0]) {
  case 'A':
  case 'a':
    switch (s[1]) {
    case '6':
      return is_separator(s[2]) ? 1 : -1;
    case 'A':
    case 'a':
      switch (s[2]) {
      case 'A':
      case 'a':
        switch (s[3]) {
        case 'A':
        case 'a':
          return is_separator(s[4]) ? 2 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'F':
    case 'f':
      switch (s[2]) {
      case 'S':
      case 's':
        switch (s[3]) {
        case 'D':
        case 'd':
          switch (s[4]) {
          case 'B':
          case 'b':
            return is_separator(s[5]) ? 3 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'L':
      case 'l':
        return is_separator(s[3]) ? 4 : -1;
      default:
        return -1;
      };
      break;
    default:
      return is_separator(s[2]) ? 0 : -1;
    };
    break;
  case 'C':
  case 'c':
    switch (s[1]) {
    case 'A':
    case 'a':
      switch (s[2]) {
      case 'A':
      case 'a':
        return is_separator(s[3]) ? 5 : -1;
      default:
        return -1;
      };
      break;
    case 'D':
    case 'd':
      switch (s[2]) {
      case 'S':
      case 's':
        return is_separator(s[3]) ? 6 : -1;
      case 'N':
      case 'n':
        switch (s[3]) {
        case 'S':
        case 's':
          switch (s[4]) {
          case 'K':
          case 'k':
            switch (s[5]) {
            case 'E':
            case 'e':
              switch (s[6]) {
              case 'Y':
              case 'y':
                return is_separator(s[7]) ? 7 : -1;
              default:
                return -1;
              };
              break;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'E':
    case 'e':
      switch (s[2]) {
      case 'R':
      case 'r':
        switch (s[3]) {
        case 'T':
        case 't':
          return is_separator(s[4]) ? 8 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'H':
    case 'h':
      return is_separator(s[2]) ? 9 : -1;
    case 'N':
    case 'n':
      switch (s[2]) {
      case 'A':
      case 'a':
        switch (s[3]) {
        case 'M':
        case 'm':
          switch (s[4]) {
          case 'E':
          case 'e':
            return is_separator(s[5]) ? 10 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      switch (s[2]) {
      case 'Y':
      case 'y':
        switch (s[3]) {
        case 'N':
        case 'n':
          switch (s[4]) {
          case 'C':
          case 'c':
            return is_separator(s[5]) ? 12 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return is_separator(s[3]) ? 11 : -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'D':
  case 'd':
    switch (s[1]) {
    case 'H':
    case 'h':
      switch (s[2]) {
      case 'C':
      case 'c':
        switch (s[3]) {
        case 'I':
        case 'i':
          switch (s[4]) {
          case 'D':
          case 'd':
            return is_separator(s[5]) ? 13 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'L':
    case 'l':
      switch (s[2]) {
      case 'V':
      case 'v':
        return is_separator(s[3]) ? 14 : -1;
      default:
        return -1;
      };
      break;
    case 'N':
    case 'n':
      switch (s[2]) {
      case 'A':
      case 'a':
        switch (s[3]) {
        case 'M':
        case 'm':
          switch (s[4]) {
          case 'E':
          case 'e':
            return is_separator(s[5]) ? 15 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      case 'S':
      case 's':
        switch (s[3]) {
        case 'K':
        case 'k':
          switch (s[4]) {
          case 'E':
          case 'e':
            switch (s[5]) {
            case 'Y':
            case 'y':
              return is_separator(s[6]) ? 16 : -1;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      return is_separator(s[2]) ? 17 : -1;
    default:
      return -1;
    };
    break;
  case 'E':
  case 'e':
    switch (s[1]) {
    case 'U':
    case 'u':
      switch (s[2]) {
      case 'I':
      case 'i':
        switch (s[3]) {
        case '4':
          switch (s[4]) {
          case '8':
            return is_separator(s[5]) ? 18 : -1;
          default:
            return -1;
          };
          break;
        case '6':
          switch (s[4]) {
          case '4':
            return is_separator(s[5]) ? 19 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'G':
  case 'g':
    switch (s[1]) {
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'O':
      case 'o':
        switch (s[3]) {
        case 'S':
        case 's':
          return is_separator(s[4]) ? 20 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'H':
  case 'h':
    switch (s[1]) {
    case 'I':
    case 'i':
      switch (s[2]) {
      case 'N':
      case 'n':
        switch (s[3]) {
        case 'F':
        case 'f':
          switch (s[4]) {
          case 'O':
          case 'o':
            return is_separator(s[5]) ? 21 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      case 'P':
      case 'p':
        return is_separator(s[3]) ? 22 : -1;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      return is_separator(s[2]) ? 23 : -1;
    case 'T':
    case 't':
      switch (s[2]) {
      case 'T':
      case 't':
        switch (s[3]) {
        case 'P':
        case 'p':
          switch (s[4]) {
          case 'S':
          case 's':
            return is_separator(s[5]) ? 24 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'I':
  case 'i':
    switch (s[1]) {
    case 'N':
    case 'n':
      return is_separator(s[2]) ? 25 : -1;
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'S':
      case 's':
        switch (s[3]) {
        case 'E':
        case 'e':
          switch (s[4]) {
          case 'C':
          case 'c':
            switch (s[5]) {
            case 'K':
            case 'k':
              switch (s[6]) {
              case 'E':
              case 'e':
                switch (s[7]) {
                case 'Y':
                case 'y':
                  return is_separator(s[8]) ? 26 : -1;
                default:
                  return -1;
                };
                break;
              default:
                return -1;
              };
              break;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      switch (s[2]) {
      case 'D':
      case 'd':
        switch (s[3]) {
        case 'N':
        case 'n':
          return is_separator(s[4]) ? 27 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'K':
  case 'k':
    switch (s[1]) {
    case 'E':
    case 'e':
      switch (s[2]) {
      case 'Y':
      case 'y':
        return is_separator(s[3]) ? 28 : -1;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      return is_separator(s[2]) ? 29 : -1;
    default:
      return -1;
    };
    break;
  case 'L':
  case 'l':
    switch (s[1]) {
    case '3':
      switch (s[2]) {
      case '2':
        return is_separator(s[3]) ? 30 : -1;
      default:
        return -1;
      };
      break;
    case '6':
      switch (s[2]) {
      case '4':
        return is_separator(s[3]) ? 31 : -1;
      default:
        return -1;
      };
      break;
    case 'O':
    case 'o':
      switch (s[2]) {
      case 'C':
      case 'c':
        return is_separator(s[3]) ? 32 : -1;
      default:
        return -1;
      };
      break;
    case 'P':
    case 'p':
      return is_separator(s[2]) ? 33 : -1;
    default:
      return -1;
    };
    break;
  case 'M':
  case 'm':
    switch (s[1]) {
    case 'B':
    case 'b':
      return is_separator(s[2]) ? 34 : -1;
    case 'D':
    case 'd':
      return is_separator(s[2]) ? 35 : -1;
    case 'F':
    case 'f':
      return is_separator(s[2]) ? 36 : -1;
    case 'G':
    case 'g':
      return is_separator(s[2]) ? 37 : -1;
    case 'I':
    case 'i':
      switch (s[2]) {
      case 'N':
      case 'n':
        switch (s[3]) {
        case 'F':
        case 'f':
          switch (s[4]) {
          case 'O':
          case 'o':
            return is_separator(s[5]) ? 38 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'R':
    case 'r':
      return is_separator(s[2]) ? 39 : -1;
    case 'X':
    case 'x':
      return is_separator(s[2]) ? 40 : -1;
    default:
      return -1;
    };
    break;
  case 'N':
  case 'n':
    switch (s[1]) {
    case 'A':
    case 'a':
      switch (s[2]) {
      case 'P':
      case 'p':
        switch (s[3]) {
        case 'T':
        case 't':
          switch (s[4]) {
          case 'R':
          case 'r':
            return is_separator(s[5]) ? 41 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'I':
    case 'i':
      switch (s[2]) {
      case 'D':
      case 'd':
        return is_separator(s[3]) ? 42 : -1;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      switch (s[2]) {
      case 'A':
      case 'a':
        switch (s[3]) {
        case 'P':
        case 'p':
          switch (s[4]) {
          case '-':
            switch (s[5]) {
            case 'P':
            case 'p':
              switch (s[6]) {
              case 'T':
              case 't':
                switch (s[7]) {
                case 'R':
                case 'r':
                  return is_separator(s[8]) ? 45 : -1;
                default:
                  return -1;
                };
                break;
              default:
                return -1;
              };
              break;
            default:
              return -1;
            };
            break;
          default:
            return is_separator(s[5]) ? 44 : -1;
          };
          break;
        default:
          return -1;
        };
        break;
      case 'E':
      case 'e':
        switch (s[3]) {
        case 'C':
        case 'c':
          switch (s[4]) {
          case '3':
            switch (s[5]) {
            case 'P':
            case 'p':
              switch (s[6]) {
              case 'A':
              case 'a':
                switch (s[7]) {
                case 'R':
                case 'r':
                  switch (s[8]) {
                  case 'A':
                  case 'a':
                    switch (s[9]) {
                    case 'M':
                    case 'm':
                      return is_separator(s[10]) ? 48 : -1;
                    default:
                      return -1;
                    };
                    break;
                  default:
                    return -1;
                  };
                  break;
                default:
                  return -1;
                };
                break;
              default:
                return -1;
              };
              break;
            default:
              return is_separator(s[6]) ? 47 : -1;
            };
            break;
          default:
            return is_separator(s[5]) ? 46 : -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return is_separator(s[3]) ? 43 : -1;
      };
      break;
    case 'U':
    case 'u':
      switch (s[2]) {
      case 'L':
      case 'l':
        switch (s[3]) {
        case 'L':
        case 'l':
          return is_separator(s[4]) ? 49 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      switch (s[2]) {
      case 'T':
      case 't':
        return is_separator(s[3]) ? 50 : -1;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'O':
  case 'o':
    switch (s[1]) {
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'E':
      case 'e':
        switch (s[3]) {
        case 'N':
        case 'n':
          switch (s[4]) {
          case 'P':
          case 'p':
            switch (s[5]) {
            case 'G':
            case 'g':
              switch (s[6]) {
              case 'P':
              case 'p':
                switch (s[7]) {
                case 'K':
                case 'k':
                  switch (s[8]) {
                  case 'E':
                  case 'e':
                    switch (s[9]) {
                    case 'Y':
                    case 'y':
                      return is_separator(s[10]) ? 51 : -1;
                    default:
                      return -1;
                    };
                    break;
                  default:
                    return -1;
                  };
                  break;
                default:
                  return -1;
                };
                break;
              default:
                return -1;
              };
              break;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'P':
  case 'p':
    switch (s[1]) {
    case 'T':
    case 't':
      switch (s[2]) {
      case 'R':
      case 'r':
        return is_separator(s[3]) ? 52 : -1;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      return is_separator(s[2]) ? 53 : -1;
    default:
      return -1;
    };
    break;
  case 'R':
  case 'r':
    switch (s[1]) {
    case 'P':
    case 'p':
      return is_separator(s[2]) ? 54 : -1;
    case 'R':
    case 'r':
      switch (s[2]) {
      case 'S':
      case 's':
        switch (s[3]) {
        case 'I':
        case 'i':
          switch (s[4]) {
          case 'G':
          case 'g':
            return is_separator(s[5]) ? 55 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'T':
    case 't':
      return is_separator(s[2]) ? 56 : -1;
    default:
      return -1;
    };
    break;
  case 'S':
  case 's':
    switch (s[1]) {
    case 'I':
    case 'i':
      switch (s[2]) {
      case 'G':
      case 'g':
        return is_separator(s[3]) ? 57 : -1;
      default:
        return -1;
      };
      break;
    case 'M':
    case 'm':
      switch (s[2]) {
      case 'I':
      case 'i':
        switch (s[3]) {
        case 'M':
        case 'm':
          switch (s[4]) {
          case 'E':
          case 'e':
            switch (s[5]) {
            case 'A':
            case 'a':
              return is_separator(s[6]) ? 58 : -1;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'O':
    case 'o':
      switch (s[2]) {
      case 'A':
      case 'a':
        return is_separator(s[3]) ? 59 : -1;
      default:
        return -1;
      };
      break;
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'F':
      case 'f':
        return is_separator(s[3]) ? 60 : -1;
      default:
        return -1;
      };
      break;
    case 'R':
    case 'r':
      switch (s[2]) {
      case 'V':
      case 'v':
        return is_separator(s[3]) ? 61 : -1;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      switch (s[2]) {
      case 'H':
      case 'h':
        switch (s[3]) {
        case 'F':
        case 'f':
          switch (s[4]) {
          case 'P':
          case 'p':
            return is_separator(s[5]) ? 62 : -1;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'V':
    case 'v':
      switch (s[2]) {
      case 'C':
      case 'c':
        switch (s[3]) {
        case 'B':
        case 'b':
          return is_separator(s[4]) ? 63 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'T':
  case 't':
    switch (s[1]) {
    case 'L':
    case 'l':
      switch (s[2]) {
      case 'S':
      case 's':
        switch (s[3]) {
        case 'A':
        case 'a':
          return is_separator(s[4]) ? 64 : -1;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      switch (s[2]) {
      case 'T':
      case 't':
        return is_separator(s[3]) ? 65 : -1;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'U':
  case 'u':
    switch (s[1]) {
    case 'R':
    case 'r':
      switch (s[2]) {
      case 'I':
      case 'i':
        return is_separator(s[3]) ? 66 : -1;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'W':
  case 'w':
    switch (s[1]) {
    case 'K':
    case 'k':
      switch (s[2]) {
      case 'S':
      case 's':
        return is_separator(s[3]) ? 67 : -1;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'X':
  case 'x':
    switch (s[1]) {
    case '2':
      switch (s[2]) {
      case '5':
        return is_separator(s[3]) ? 68 : -1;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  case 'Z':
  case 'z':
    switch (s[1]) {
    case 'O':
    case 'o':
      switch (s[2]) {
      case 'N':
      case 'n':
        switch (s[3]) {
        case 'E':
        case 'e':
          switch (s[4]) {
          case 'M':
          case 'm':
            switch (s[5]) {
            case 'D':
            case 'd':
              return is_separator(s[6]) ? 69 : -1;
            default:
              return -1;
            };
            break;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return -1;
      };
      break;
    default:
      return -1;
    };
    break;
  default:
    return -1;
  };
}

void pretty_print(size_t volume, size_t bytes, std::string name,
                  event_aggregate agg) {
  printf("%-30s : ", name.c_str());
  printf(" %5.2f GB/s ", bytes / agg.fastest_elapsed_ns());
  printf(" %5.1f Ma/s ", volume * 1000.0 / agg.fastest_elapsed_ns());
  printf(" %5.2f ns/d ", agg.fastest_elapsed_ns() / volume);
  if (collector.has_events()) {
    printf(" %5.2f GHz ", agg.fastest_cycles() / agg.fastest_elapsed_ns());
    printf(" %5.2f c/d ", agg.fastest_cycles() / volume);
    printf(" %5.2f i/d ", agg.fastest_instructions() / volume);
    printf(" %5.1f c/b ", agg.fastest_cycles() / bytes);
    printf(" %5.2f i/b ", agg.fastest_instructions() / bytes);
    printf(" %5.2f i/c ", agg.fastest_instructions() / agg.fastest_cycles());
  }
  printf("\n");
}

int main() {
  std::vector<std::string> source = {
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
  if (!collector.has_events()) {
    printf("Performance counters unavalaible. Consider running in privileged "
           "mode (e.g., sudo).\n");
  }
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0, source.size() - 1);
  std::vector<std::string> test_data;
  size_t N = 2000000;
  size_t bytes = 0;
  while (test_data.size() < N) {
    uint64_t t = distribution(generator);
    std::string s = source[t];
    bytes += s.size();
    s.resize(16);
    test_data.push_back(s);
  }
  volatile uint64_t sum{};
  pretty_print(N, bytes, "sse_type", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   uint8_t t;
                   sse_type(s.data(), &t); // should check error
                   sum += t;
                 }
               }));
  pretty_print(N, bytes, "simple_trie", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   int r = simple_trie(s);
                   sum += r; // should check error
                 }
               }));
  pretty_print(N, bytes, "bsearch", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                   const std::string *answer = lookup_symbol(s.data());
                   sum += answer == nullptr
                              ? 0
                              : answer->size(); // should check error
                 }
               }));
  pretty_print(N, bytes, "sse_length", bench([&test_data, &sum]() {
                 for (const std::string &s : test_data) {
                  sum += sse_length(s.data());
                 }
               }));
  pretty_print(
      N, bytes, "std::lower_bound", bench([&test_data, &sum]() {
        std::string s;
        s.resize(16);
        for (const std::string &sc : test_data) {
          for (size_t i = 0; i < 16; i++) {
            s[i] = std::tolower(sc[i]);
          }
          auto lb = std::lower_bound(
              lowerstrings.begin(), lowerstrings.end(), s,
              [](const std::string &source, const std::string &target) {
                std::string_view targetsv(target.data(), source.size());
                if (source == targetsv) {
                  return !is_separator(target.data()[source.size()]);
                }
                return source < targetsv;
              });
          if (lb != lowerstrings.end()) {
            // We have something to look at
            std::string &candidate = *lb;
            std::string_view targetsv(s.data(), candidate.size());

            if (targetsv == candidate &&
                is_separator(s.data()[candidate.size()])) {
              sum += targetsv.size(); // do something.
            }
          }

          // should check error
        }
      }));
}
