#include <charconv>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <sstream>

#include <string_view>
#include <tuple>
#include <vector>

uint64_t nano() {
  return std::chrono::duration_cast<::std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

#include <string>

std::string ipv40(const uint64_t address) noexcept {
  std::string output = std::to_string(address % 256);
  auto n = address >> 8;
  for (size_t i = 1; i < 4; i++) {
    output.insert(0, std::to_string(n % 256) + ".");
    n >>= 8;
  }
  return output;
}

std::string ipvss(const uint32_t address) noexcept {
  std::stringstream s;
  s << (address >> 24);
  for (int i = 2; i >= 0; i--) {
    s << "." << ((address >> (i * 8)) & 0xFF);
  }
  return s.str();
}

std::string ipv41(const uint32_t address) noexcept {
  std::string output = std::to_string(address >> 24);
  for (int i = 2; i >= 0; i--) {
    output.append("." + std::to_string((address >> (i * 8)) % 256));
  }
  return output;
}

std::string ipv42(const uint32_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, address >> 24).ptr;
  for (int i = 2; i >= 0; i--) {
    *point++ = '.';
    point = std::to_chars(point, point_end, (address >> (i * 8)) % 256).ptr;
  }
  output.resize(point - output.data());
  return output;
}

std::string ipv43(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *point = output.data();
  char *point_end = output.data() + output.size();
  point = std::to_chars(point, point_end, uint8_t(address >> 24)).ptr;
  for (int i = 2; i >= 0; i--) {
    *point++ = '.';
    point = std::to_chars(point, point_end, uint8_t(address >> (i * 8))).ptr;
  }
  output.resize(point - output.data());
  return output;
}

char *to_chars_51(char *p, unsigned char x) {
  constexpr char const *table = "00010203040506070809"
                                "10111213141516171819"
                                "20212223242526272829"
                                "30313233343536373839"
                                "40414243444546474849"
                                "50515253545556575859"
                                "60616263646566676869"
                                "70717273747576777879"
                                "80818283848586878889"
                                "90919293949596979899";

  if (x < 10) {
    *p++ = x + '0';
    return p;
  }

  if (x < 100) {
    *p++ = table[2 * x + 0];
    *p++ = table[2 * x + 1];
    return p;
  }

  if (x < 200) {
    x -= 100;
    *p++ = '1';
    *p++ = table[2 * x + 0];
    *p++ = table[2 * x + 1];
    return p;
  }

  // if( x < 300 )
  {
    x -= 200;
    *p++ = '2';
    *p++ = table[2 * x + 0];
    *p++ = table[2 * x + 1];
    return p;
  }
}

// credit: Peter Dimov
std::string ipv51(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *p = output.data();

  p = to_chars_51(p, uint8_t(address >> 24));
  *p++ = '.';
  p = to_chars_51(p, uint8_t(address >> 16));
  *p++ = '.';
  p = to_chars_51(p, uint8_t(address >> 8));
  *p++ = '.';
  p = to_chars_51(p, uint8_t(address >> 0));

  output.resize(p - output.data());
  return output;
}

char *to_chars_52(char *p, unsigned char x) {
  constexpr std::string_view table[256] = {
      "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",   "8",   "9",
      "10",  "11",  "12",  "13",  "14",  "15",  "16",  "17",  "18",  "19",
      "20",  "21",  "22",  "23",  "24",  "25",  "26",  "27",  "28",  "29",
      "30",  "31",  "32",  "33",  "34",  "35",  "36",  "37",  "38",  "39",
      "40",  "41",  "42",  "43",  "44",  "45",  "46",  "47",  "48",  "49",
      "50",  "51",  "52",  "53",  "54",  "55",  "56",  "57",  "58",  "59",
      "60",  "61",  "62",  "63",  "64",  "65",  "66",  "67",  "68",  "69",
      "70",  "71",  "72",  "73",  "74",  "75",  "76",  "77",  "78",  "79",
      "80",  "81",  "82",  "83",  "84",  "85",  "86",  "87",  "88",  "89",
      "90",  "91",  "92",  "93",  "94",  "95",  "96",  "97",  "98",  "99",
      "100", "101", "102", "103", "104", "105", "106", "107", "108", "109",
      "110", "111", "112", "113", "114", "115", "116", "117", "118", "119",
      "120", "121", "122", "123", "124", "125", "126", "127", "128", "129",
      "130", "131", "132", "133", "134", "135", "136", "137", "138", "139",
      "140", "141", "142", "143", "144", "145", "146", "147", "148", "149",
      "150", "151", "152", "153", "154", "155", "156", "157", "158", "159",
      "160", "161", "162", "163", "164", "165", "166", "167", "168", "169",
      "170", "171", "172", "173", "174", "175", "176", "177", "178", "179",
      "180", "181", "182", "183", "184", "185", "186", "187", "188", "189",
      "190", "191", "192", "193", "194", "195", "196", "197", "198", "199",
      "200", "201", "202", "203", "204", "205", "206", "207", "208", "209",
      "210", "211", "212", "213", "214", "215", "216", "217", "218", "219",
      "220", "221", "222", "223", "224", "225", "226", "227", "228", "229",
      "230", "231", "232", "233", "234", "235", "236", "237", "238", "239",
      "240", "241", "242", "243", "244", "245", "246", "247", "248", "249",
      "250", "251", "252", "253", "254", "255",
  };

  std::string_view sv = table[x];

  std::memcpy(p, sv.data(), sv.size());
  return p + sv.size();
}
// credit: Peter Dimov
std::string ipv52(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');
  char *p = output.data();

  p = to_chars_52(p, uint8_t(address >> 24));
  *p++ = '.';
  p = to_chars_52(p, uint8_t(address >> 16));
  *p++ = '.';
  p = to_chars_52(p, uint8_t(address >> 8));
  *p++ = '.';
  p = to_chars_52(p, uint8_t(address >> 0));

  output.resize(p - output.data());
  return output;
}

// This data structure uses at least 4kB.
class Foo {
public:
  Foo() {
    for (int i = 0; i < 256; i++) {
      std::string s0 = std::to_string(i);
      std::string s1 = std::string(".") + s0;
      std::memcpy(v0 + i, s0.data(), sizeof(uint32_t));
      std::memcpy(v1 + i, s1.data(), sizeof(uint32_t));
      l0[i] = (i < 10) ? 1 : (i < 100 ? 2 : 3);
      l1[i] = l0[i] + 1;
    }
  }
  uint32_t v0[256]; // value for the 0th byte
  uint32_t v1[256]; // value for the 1+ byte
  uint32_t l0[256]; // length for the 0th byte
  uint32_t l1[256]; // length for the 1+ byte
};

Foo foo;

std::string ipv61(const uint64_t address) noexcept {
  std::string output(4 * 3 + 3, '\0');

  char *point = output.data();
  uint8_t by;

  by = address >> 24;
  std::memcpy(point, foo.v0 + by, sizeof(uint32_t));
  point += foo.l0[by];

  by = address >> 16;
  std::memcpy(point, foo.v1 + by, sizeof(uint32_t));
  point += foo.l1[by];

  by = address >> 8;
  std::memcpy(point, foo.v1 + by, sizeof(uint32_t));
  point += foo.l1[by];

  by = address >> 0;
  std::memcpy(point, foo.v1 + by, sizeof(uint32_t));
  point += foo.l1[by];
  output.resize(point - output.data());
  return output;
}

////////////////////////////////////////////////////////////////////////////////
// almost same as ipv61 but with constexpr table generators
// using only 1024 + 256 = 1280 bytes instead of 4kB
// and append . at the end instead prepending it

#include <array>

template <class Function, std::size_t... Indices>
constexpr auto make_array_helper(Function &&f, std::index_sequence<Indices...>)
    -> std::array<typename std::invoke_result<Function, std::size_t>::type,
                  sizeof...(Indices)> {
  return {{f(Indices)...}};
}

template <int N, class Function>
constexpr auto make_array(Function &&f)
    -> std::array<typename std::invoke_result<Function, std::size_t>::type, N> {
  return make_array_helper(f, std::make_index_sequence<N>{});
}

static std::string ipv62(const uint32_t address) noexcept {

  // This data structure uses 1024 bytes = 256 x sizeof(uint32_t).
  static constexpr auto table_u8_to_u32_str =
      make_array<256>([](uint8_t index) -> uint32_t {
        // equivalent to this but constexpr
        //
        // std::string s = std::to_string(index) + "."
        // uint32_t dst;
        // std::memcpy(&dst, s.data(), sizeof(uint32_t));
        // return dst;

        unsigned int shift = 0;
        uint8_t src = index;
        uint32_t dst = 0;

        if (index >= 100) {
          dst += ('0' + (src / 100)) << shift;
          shift += 8;
          src %= 100;
        }

        if (index >= 10) {
          dst += ('0' + (src / 10)) << shift;
          shift += 8;
          src %= 10;
        }

        dst += ('0' + src) << shift;
        shift += 8;

        dst += '.' << shift;

        return dst;
      });

  // This data structure uses 256 bytes = 256 x sizeof(uint8_t).
  static constexpr auto table_length =
      make_array<256>([](uint8_t index) -> uint8_t {
        return (index < 10) ? 2 : (index < 100 ? 3 : 4);
      });

  std::string output(16, '\0');

  char *point = output.data();
  uint8_t by;

  by = address >> 24;
  std::memcpy(point, table_u8_to_u32_str.data() + by, sizeof(uint32_t));
  point += table_length[by];

  by = address >> 16;
  std::memcpy(point, table_u8_to_u32_str.data() + by, sizeof(uint32_t));
  point += table_length[by];

  by = address >> 8;
  std::memcpy(point, table_u8_to_u32_str.data() + by, sizeof(uint32_t));
  point += table_length[by];

  by = address;
  std::memcpy(point, table_u8_to_u32_str.data() + by, sizeof(uint32_t));
  point += table_length[by];

  --point; // remove last appended .
  output.resize(point - output.data());
  return output;
}

////////////////////////////////////////////////////////////////////////////////
// We can do nearly as well using only 1kB.
std::string ipv71(const uint64_t address) noexcept {
  // uses 1025 bytes
  constexpr static const char *lookup =
      ".0  .1  .2  .3  .4  .5  .6  .7  .8  .9  .10 .11 .12 .13 .14 .15 .16 .17 "
      ".18 .19 .20 .21 .22 .23 .24 .25 .26 .27 .28 .29 .30 .31 .32 .33 .34 .35 "
      ".36 .37 .38 .39 .40 .41 .42 .43 .44 .45 .46 .47 .48 .49 .50 .51 .52 .53 "
      ".54 .55 .56 .57 .58 .59 .60 .61 .62 .63 .64 .65 .66 .67 .68 .69 .70 .71 "
      ".72 .73 .74 .75 .76 .77 .78 .79 .80 .81 .82 .83 .84 .85 .86 .87 .88 .89 "
      ".90 .91 .92 .93 .94 .95 .96 .97 .98 .99 "
      ".100.101.102.103.104.105.106.107.108.109.110.111.112.113.114.115.116."
      "117.118.119.120.121.122.123.124.125.126.127.128.129.130.131.132.133.134."
      "135.136.137.138.139.140.141.142.143.144.145.146.147.148.149.150.151.152."
      "153.154.155.156.157.158.159.160.161.162.163.164.165.166.167.168.169.170."
      "171.172.173.174.175.176.177.178.179.180.181.182.183.184.185.186.187.188."
      "189.190.191.192.193.194.195.196.197.198.199.200.201.202.203.204.205.206."
      "207.208.209.210.211.212.213.214.215.216.217.218.219.220.221.222.223.224."
      "225.226.227.228.229.230.231.232.233.234.235.236.237.238.239.240.241.242."
      "243.244.245.246.247.248.249.250.251.252.253.254.255";
  std::string output(4 * 3 + 3, '\0');

  char *point = output.data();
  uint8_t by;

  by = address >> 24;
  std::memcpy(point, lookup + by * 4 + 1, 4);
  point += (by < 10) ? 1 : (by < 100 ? 2 : 3);

  by = address >> 16;
  std::memcpy(point, lookup + by * 4, 4);
  point += (by < 10) ? 2 : (by < 100 ? 3 : 4);

  by = address >> 8;
  std::memcpy(point, lookup + by * 4, 4);
  point += (by < 10) ? 2 : (by < 100 ? 3 : 4);

  by = address >> 0;
  std::memcpy(point, lookup + by * 4, 4);
  point += (by < 10) ? 2 : (by < 100 ? 3 : 4);
  output.resize(point - output.data());
  return output;
}

std::string ipv81(const uint32_t address) noexcept {
  // uses 1024 bytes
  // little-endian version
  constexpr static const uint32_t lookup[] = {
      0x40002e30, 0x40002e31, 0x40002e32, 0x40002e33, 0x40002e34, 0x40002e35,
      0x40002e36, 0x40002e37, 0x40002e38, 0x40002e39, 0x802e3031, 0x802e3131,
      0x802e3231, 0x802e3331, 0x802e3431, 0x802e3531, 0x802e3631, 0x802e3731,
      0x802e3831, 0x802e3931, 0x802e3032, 0x802e3132, 0x802e3232, 0x802e3332,
      0x802e3432, 0x802e3532, 0x802e3632, 0x802e3732, 0x802e3832, 0x802e3932,
      0x802e3033, 0x802e3133, 0x802e3233, 0x802e3333, 0x802e3433, 0x802e3533,
      0x802e3633, 0x802e3733, 0x802e3833, 0x802e3933, 0x802e3034, 0x802e3134,
      0x802e3234, 0x802e3334, 0x802e3434, 0x802e3534, 0x802e3634, 0x802e3734,
      0x802e3834, 0x802e3934, 0x802e3035, 0x802e3135, 0x802e3235, 0x802e3335,
      0x802e3435, 0x802e3535, 0x802e3635, 0x802e3735, 0x802e3835, 0x802e3935,
      0x802e3036, 0x802e3136, 0x802e3236, 0x802e3336, 0x802e3436, 0x802e3536,
      0x802e3636, 0x802e3736, 0x802e3836, 0x802e3936, 0x802e3037, 0x802e3137,
      0x802e3237, 0x802e3337, 0x802e3437, 0x802e3537, 0x802e3637, 0x802e3737,
      0x802e3837, 0x802e3937, 0x802e3038, 0x802e3138, 0x802e3238, 0x802e3338,
      0x802e3438, 0x802e3538, 0x802e3638, 0x802e3738, 0x802e3838, 0x802e3938,
      0x802e3039, 0x802e3139, 0x802e3239, 0x802e3339, 0x802e3439, 0x802e3539,
      0x802e3639, 0x802e3739, 0x802e3839, 0x802e3939, 0xee303031, 0xee313031,
      0xee323031, 0xee333031, 0xee343031, 0xee353031, 0xee363031, 0xee373031,
      0xee383031, 0xee393031, 0xee303131, 0xee313131, 0xee323131, 0xee333131,
      0xee343131, 0xee353131, 0xee363131, 0xee373131, 0xee383131, 0xee393131,
      0xee303231, 0xee313231, 0xee323231, 0xee333231, 0xee343231, 0xee353231,
      0xee363231, 0xee373231, 0xee383231, 0xee393231, 0xee303331, 0xee313331,
      0xee323331, 0xee333331, 0xee343331, 0xee353331, 0xee363331, 0xee373331,
      0xee383331, 0xee393331, 0xee303431, 0xee313431, 0xee323431, 0xee333431,
      0xee343431, 0xee353431, 0xee363431, 0xee373431, 0xee383431, 0xee393431,
      0xee303531, 0xee313531, 0xee323531, 0xee333531, 0xee343531, 0xee353531,
      0xee363531, 0xee373531, 0xee383531, 0xee393531, 0xee303631, 0xee313631,
      0xee323631, 0xee333631, 0xee343631, 0xee353631, 0xee363631, 0xee373631,
      0xee383631, 0xee393631, 0xee303731, 0xee313731, 0xee323731, 0xee333731,
      0xee343731, 0xee353731, 0xee363731, 0xee373731, 0xee383731, 0xee393731,
      0xee303831, 0xee313831, 0xee323831, 0xee333831, 0xee343831, 0xee353831,
      0xee363831, 0xee373831, 0xee383831, 0xee393831, 0xee303931, 0xee313931,
      0xee323931, 0xee333931, 0xee343931, 0xee353931, 0xee363931, 0xee373931,
      0xee383931, 0xee393931, 0xee303032, 0xee313032, 0xee323032, 0xee333032,
      0xee343032, 0xee353032, 0xee363032, 0xee373032, 0xee383032, 0xee393032,
      0xee303132, 0xee313132, 0xee323132, 0xee333132, 0xee343132, 0xee353132,
      0xee363132, 0xee373132, 0xee383132, 0xee393132, 0xee303232, 0xee313232,
      0xee323232, 0xee333232, 0xee343232, 0xee353232, 0xee363232, 0xee373232,
      0xee383232, 0xee393232, 0xee303332, 0xee313332, 0xee323332, 0xee333332,
      0xee343332, 0xee353332, 0xee363332, 0xee373332, 0xee383332, 0xee393332,
      0xee303432, 0xee313432, 0xee323432, 0xee333432, 0xee343432, 0xee353432,
      0xee363432, 0xee373432, 0xee383432, 0xee393432, 0xee303532, 0xee313532,
      0xee323532, 0xee333532, 0xee343532, 0xee353532,
  };

  // This is a hack. Thanks to C++11 semantics, the buffer includes
  // null termination, which is not included in the allocation size.
  // That byte may be overwritten by code below but resizing guarantees
  // that the string is valid in the end of the function anyway.
  // This string wouldn't need initialization at all, but this way
  // in practice one byte less gets initialized, removing one store.
  std::string output(4 * 4 - 1, '\0');

  char *buf = output.data();

  uint32_t val0, val1, val2, val3, str;
  size_t digits;

  val0 = lookup[(uint8_t)(address >> 24)];
  val1 = lookup[(uint8_t)(address >> 16)];
  val2 = lookup[(uint8_t)(address >> 8)];
  val3 = lookup[(uint8_t)address];

  str = val0 & 0x3fffffff;
  std::memcpy(buf, &str, 4);
  digits = val0 >> 30;

  str = val1 & 0x3fffffff;
  std::memcpy(buf + digits + 1, &str, 4);
  digits += val1 >> 30;

  str = val2 & 0x3fffffff;
  std::memcpy(buf + digits + 2, &str, 4);
  digits += val2 >> 30;

  str = val3; // no masking needed, last byte never included in output
  std::memcpy(buf + digits + 3, &str, 4);
  digits += val3 >> 30;

  output.resize(digits + 3);
  return output;
}

std::vector<std::string> data;

std::tuple<double, double, double, double, double, double, double, double,
           double, double, double>
simulation(const size_t N) {
  double t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
  data.reserve(N);

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv40(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t1 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv41(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t2 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv42(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t3 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv43(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t4 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv51(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t5 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv52(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t6 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv61(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t7 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv62(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t8 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv71(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t9 = double(finish - start) / N;
  }

  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipv81(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t10 = double(finish - start) / N;
  }
  {
    data.clear();
    uint64_t start = nano();
    for (size_t i = 0; i < N; i++) {
      data.emplace_back(ipvss(uint32_t(1271132211 * i)));
    }
    uint64_t finish = nano();
    t11 = double(finish - start) / N;
  }

  return {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11};
}

void demo() {
  double avg1 = 0;
  double avg2 = 0;
  double avg3 = 0;
  double avg4 = 0;
  double avg5 = 0;
  double avg6 = 0;
  double avg7 = 0;
  double avg8 = 0;
  double avg9 = 0;
  double avg10 = 0;
  double avg11 = 0;

  size_t times = 10;

  for (size_t i = 0; i < times; i++) {
    auto [t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11] = simulation(131072);
    avg1 += t1;
    avg2 += t2;
    avg3 += t3;
    avg4 += t4;
    avg5 += t5;
    avg6 += t6;
    avg7 += t7;
    avg8 += t8;
    avg9 += t9;
    avg10 += t10;
    avg11 += t11;
  }
  avg1 /= times;
  avg2 /= times;
  avg3 /= times;
  avg4 /= times;
  avg5 /= times;
  avg6 /= times;
  avg7 /= times;
  avg8 /= times;
  avg9 /= times;
  avg10 /= times;
  avg11 /= times;

  std::cout << "Time per string in ns.\n";
  std::cout << "stringstream        " << avg11 << std::endl;
  std::cout << "std::to_string (1)  " << avg1 << std::endl;
  std::cout << "std::to_string (2)  " << avg2 << std::endl;
  std::cout << "blog post (similar) " << avg3 << std::endl;
  std::cout << "blog post           " << avg4 << std::endl;
  std::cout << "Dimov 1             " << avg5 << std::endl;
  std::cout << "Dimov 2             " << avg6 << std::endl;
  std::cout << "Zukowski            " << avg7 << std::endl;
  std::cout << "Zukowski by oc      " << avg8 << std::endl;
  std::cout << "thin table          " << avg9 << std::endl;
  std::cout << "combined table      " << avg10 << std::endl;
}

void test() {
  puts("Running tests.");
  for (uint32_t i = 0; i < 100000; i++) {
    uint32_t ip = 1271132211 * i;
    std::string s40 = ipv40(ip);
    std::string s41 = ipv41(ip);
    std::string s42 = ipv42(ip);
    std::string s43 = ipv43(ip);
    std::string s51 = ipv51(ip);
    std::string s52 = ipv52(ip);
    std::string s61 = ipv61(ip);
    std::string s62 = ipv62(ip);
    std::string s71 = ipv71(ip);
    std::string s81 = ipv81(ip);
    std::string sss = ipvss(ip);
    if (s40 != sss) {
      printf("ip=%08x s40: '%s' sss: '%s'\n", ip, s40.c_str(), sss.c_str());
      exit(1);
    }
    if (s40 != s41) {
      printf("ip=%08x s40: '%s' s41: '%s'\n", ip, s40.c_str(), s41.c_str());
      exit(1);
    }
    if (s40 != s42) {
      printf("ip=%08x s40: '%s' s42: '%s'\n", ip, s40.c_str(), s42.c_str());
      exit(1);
    }
    if (s40 != s43) {
      printf("ip=%08x s40: '%s' s43: '%s'\n", ip, s40.c_str(), s43.c_str());
      exit(1);
    }
    if (s40 != s51) {
      printf("ip=%08x s40: '%s' s51: '%s'\n", ip, s40.c_str(), s51.c_str());
      exit(1);
    }
    if (s40 != s52) {
      printf("ip=%08x s40: '%s' s52: '%s'\n", ip, s40.c_str(), s52.c_str());
      exit(1);
    }
    if (s40 != s61) {
      printf("ip=%08x s40: '%s' s61: '%s'\n", ip, s40.c_str(), s61.c_str());
      exit(1);
    }
    if (s40 != s62) {
      printf("ip=%08x s40: '%s' s62: '%s'\n", ip, s40.c_str(), s62.c_str());
      exit(1);
    }
    if (s40 != s71) {
      printf("ip=%08x s40: '%s' s71: '%s'\n", ip, s40.c_str(), s71.c_str());
      exit(1);
    }
    if (s40 != s81) {
      printf("ip=%08x s40: '%s' s81: '%s'\n", ip, s40.c_str(), s81.c_str());
      exit(1);
    }
  }
  puts("Functions are ok.");
}

int main() {
  test();
  demo();
}
