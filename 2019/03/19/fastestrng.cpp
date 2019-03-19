#include <cstdint>
#include <iostream>
#include <sys/time.h>

static unsigned long long wyseed;

inline unsigned long long wyrng(void) {
  wyseed += 0x60bee2bee120fc15ull;
  __uint128_t tmp = (__uint128_t)(wyseed)*0xa3b195354a39b70dull;
  unsigned long long m1 = (tmp >> 64) ^ tmp;
  tmp = (__uint128_t)m1 * 0x1b03738712fad5c9ull;
  return (tmp >> 64) ^ tmp;
}

static __uint128_t g_lehmer64_state;
inline unsigned long long lehmer64() {
  g_lehmer64_state *= 0xda942042e4dd58b5ull;
  return g_lehmer64_state >> 64;
}
static __uint128_t g_lehmer64_state2;
inline unsigned long long lehmer64_2() {
  g_lehmer64_state2 *= 0xda942042e4dd58b5ull;
  return g_lehmer64_state2 >> 64;
}

using namespace std;
int main(void) {
#define N 524288
  uint64_t recipient[N];
  g_lehmer64_state = (__uint128_t)1232451235;
  g_lehmer64_state2 = (__uint128_t)135432;
  wyseed = 3214215;
  timeval beg, end;
  for (size_t ti = 0; ti < 4; ti++) {
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N; i++)
      recipient[i] = wyrng();
    gettimeofday(&end, NULL);
    cerr << "wyrng   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N; i++)
      recipient[i] = lehmer64();
    gettimeofday(&end, NULL);
    cerr << "lehmer64\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N; i += 2) {
      recipient[i] = lehmer64();
      recipient[i + 1] = lehmer64_2();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (2)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    cout << endl
         << "Next we do random number computations only, doing no work."
         << endl;
    uint64_t s = 0;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N; i++)
      s += wyrng();
    gettimeofday(&end, NULL);
    cerr << "wyrng   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N; i++)
      s += lehmer64();
    gettimeofday(&end, NULL);
    cerr << "lehmer64\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N / 2; i++)
      s += lehmer64() + lehmer64_2();
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (2)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;
    cout << endl << endl;
  }
  return 0;
}
