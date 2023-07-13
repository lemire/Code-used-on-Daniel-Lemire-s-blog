
#include "performancecounters/benchmarker.h"
#include <charconv>
#include <random>
#include <vector>

extern "C" {
#include "sse_type.h"
}

// This only checks that the prefix is correct, we would need to
// check that the next character is a separator, to be more precise.
__attribute__((noinline)) int simple_trie(const std::string &s) {
  switch (s[0]) {
  case 'A':
  case 'a':
    switch (s[1]) {
    case '6':
      return 1;
    case 'A':
    case 'a':
      switch (s[2]) {
      case 'A':
      case 'a':
        switch (s[3]) {
        case 'A':
        case 'a':
          return 2;
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
            return 3;
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
        return 4;
      default:
        return -1;
      };
      break;
    default:
      return 0;
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
        return 5;
      default:
        return -1;
      };
      break;
    case 'D':
    case 'd':
      switch (s[2]) {
      case 'S':
      case 's':
        return 6;
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
                return 7;
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
          return 8;
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
      return 9;
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
            return 10;
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
            return 12;
          default:
            return -1;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return 11;
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
            return 13;
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
        return 14;
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
            return 15;
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
              return 16;
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
      return 17;
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
            return 18;
          default:
            return -1;
          };
          break;
        case '6':
          switch (s[4]) {
          case '4':
            return 19;
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
          return 20;
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
            return 21;
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
        return 22;
      default:
        return -1;
      };
      break;
    case 'S':
    case 's':
      return 23;
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
            return 24;
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
      return 25;
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
                  return 26;
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
          return 27;
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
        return 28;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      return 29;
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
        return 30;
      default:
        return -1;
      };
      break;
    case '6':
      switch (s[2]) {
      case '4':
        return 31;
      default:
        return -1;
      };
      break;
    case 'O':
    case 'o':
      switch (s[2]) {
      case 'C':
      case 'c':
        return 32;
      default:
        return -1;
      };
      break;
    case 'P':
    case 'p':
      return 33;
    default:
      return -1;
    };
    break;
  case 'M':
  case 'm':
    switch (s[1]) {
    case 'B':
    case 'b':
      return 34;
    case 'D':
    case 'd':
      return 35;
    case 'F':
    case 'f':
      return 36;
    case 'G':
    case 'g':
      return 37;
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
            return 38;
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
      return 39;
    case 'X':
    case 'x':
      return 40;
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
            return 41;
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
        return 42;
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
                  return 45;
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
            return 44;
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
                      return 48;
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
              return 47;
            };
            break;
          default:
            return 46;
          };
          break;
        default:
          return -1;
        };
        break;
      default:
        return 43;
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
          return 49;
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
        return 50;
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
                      return 51;
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
        return 52;
      default:
        return -1;
      };
      break;
    case 'X':
    case 'x':
      return 53;
    default:
      return -1;
    };
    break;
  case 'R':
  case 'r':
    switch (s[1]) {
    case 'P':
    case 'p':
      return 54;
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
            return 55;
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
      return 56;
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
        return 57;
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
              return 58;
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
        return 59;
      default:
        return -1;
      };
      break;
    case 'P':
    case 'p':
      switch (s[2]) {
      case 'F':
      case 'f':
        return 60;
      default:
        return -1;
      };
      break;
    case 'R':
    case 'r':
      switch (s[2]) {
      case 'V':
      case 'v':
        return 61;
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
            return 62;
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
          return 63;
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
          return 64;
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
        return 65;
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
        return 66;
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
        return 67;
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
        return 68;
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
              return 69;
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
                   sum += simple_trie(s); // should check error
                 }
               }));
}