// g++  -O3 -o gcd gcd.cpp

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utility>

using namespace std;
class WallClockTimer {
public:
  struct timeval t1, t2;

public:
  WallClockTimer() : t1(), t2() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  void reset() {
    gettimeofday(&t1, 0);
    t2 = t1;
  }
  int elapsed() {
    return (t2.tv_sec * 1000 + t2.tv_usec / 1000) -
           (t1.tv_sec * 1000 + t1.tv_usec / 1000);
  }
  int split() {
    gettimeofday(&t2, 0);
    return elapsed();
  }
};

// from http://en.wikipedia.org/wiki/Binary_GCD_algorithm
unsigned int gcdwikipedia2(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  for (shift = 0; ((u | v) & 1) == 0; ++shift) {
    u >>= 1;
    v >>= 1;
  }

  while ((u & 1) == 0)
    u >>= 1;

  do {
    while ((v & 1) == 0)
      v >>= 1;
    if (u > v) {
      unsigned int t = v;
      v = u;
      u = t;
    }
    v = v - u;
  } while (v != 0);
  return u << shift;
}

// based on wikipedia's article,
// fixed by D. Lemire and R. Corderoy
unsigned int gcdwikipedia2fast(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u > v) {
      unsigned int t = v;
      v = u;
      u = t;
    }
    v = v - u;
  } while (v != 0);
  return u << shift;
}

// based on wikipedia's article,
// fixed by D. Lemire and R. Corderoy
unsigned int gcdwikipedia2fastswap(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u > v)
      std::swap(u, v);
    v = v - u;
  } while (v != 0);
  return u << shift;
}

#ifdef USE_ASSEMBLY
// based on wikipedia's article,
// fixed by D. Lemire and R. Corderoy
unsigned int gcdwikipedia2fastxchg(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u > v)
      asm volatile("xchg %0, %1\n" : "+r"(u), "+r"(v) :);
    v = v - u;
  } while (v != 0);
  return u << shift;
}
#endif
// based on wikipedia's article,
// fixed by D. Lemire, R. Corderoy, K. Willets
unsigned int gcdwikipedia3fast(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    v >>= __builtin_ctz(v);
    if (u == v)
      break;
    else if (u > v) {
      unsigned int t = v;
      v = u;
      u = t;
    }
    v = v - u;
  } while (true);
  return u << shift;
}
// based on wikipedia's article,
// fixed by D. Lemire and R. Corderoy (twice)
unsigned int gcdwikipedia4fast(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    unsigned m;
    v >>= __builtin_ctz(v);
    m = (v ^ u) & -(v < u);
    u ^= m;
    v ^= m;
    v -= u;
  } while (v != 0);
  return u << shift;
}

// based on wikipedia's article,
// fixed by D. Lemire and R. Corderoy (twice) and by Spelvin
unsigned int gcdwikipedia8Spelvin(unsigned int u, unsigned int v) {
  int shift;
  assert(v <= INT_MAX);
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    unsigned m;
    v >>= __builtin_ctz(v);
    v -= u;
    m = (int)v >> 31;
    u += v & m; /* u + (v - u) = v */
    v = (v + m) ^ m;
  } while (v != 0);
  return u << shift;
}

// based on wikipedia's article,
// fixed by D. Lemire,  K. Willets
unsigned int gcdwikipedia5fast(unsigned int u, unsigned int v) {
  int shift, uz, vz;
  uz = __builtin_ctz(u);
  if (u == 0)
    return v;
  vz = __builtin_ctz(v);
  if (v == 0)
    return u;
  shift = uz > vz ? vz : uz;
  u >>= uz;
  do {
    v >>= vz;
    if (u > v) {
      unsigned int t = v;
      v = u;
      u = t;
    }
    v = v - u;
    vz = __builtin_ctz(v);
  } while (v != 0);
  return u << shift;
}
#ifdef USE_ASSEMBLY
// based on wikipedia's article,
// fixed by D. Lemire,  K. Willets
unsigned int gcdwikipedia6fastxchg(unsigned int u, unsigned int v) {
  int shift, uz, vz;
  uz = __builtin_ctz(u);
  if (u == 0)
    return v;
  vz = __builtin_ctz(v);
  if (v == 0)
    return u;
  shift = uz > vz ? vz : uz;
  u >>= uz;
  do {
    v >>= vz;
    if (u > v)
      asm volatile("xchg %0, %1\n" : "+r"(u), "+r"(v) :);
    v = v - u;
    vz = __builtin_ctz(v);
  } while (v != 0);
  return u << shift;
}
#endif
// based on wikipedia's article,
// fixed by D. Lemire,  K. Willets
unsigned int gcdwikipedia7fast(unsigned int u, unsigned int v) {
  int shift, uz, vz;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  uz = __builtin_ctz(u);
  vz = __builtin_ctz(v);
  shift = uz > vz ? vz : uz;
  u >>= uz;

  do {
    v >>= vz;
    long long int diff = v;
    diff -= u;
    vz = __builtin_ctz(diff);
    if (diff == 0)
      break;
    if (diff < 0)
      u = v;
    v = abs(diff);

  } while (1);
  return u << shift;
}

// based on wikipedia's article,
// fixed by D. Lemire,  K. Willets
unsigned int gcdwikipedia7fast32(unsigned int u, unsigned int v) {
  int shift, uz, vz;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  uz = __builtin_ctz(u);
  vz = __builtin_ctz(v);
  shift = uz > vz ? vz : uz;
  u >>= uz;
  do {
    v >>= vz;
    int diff = v;
    diff -= u;
    vz = __builtin_ctz(diff);
    if (diff == 0)
      break;
    if (v < u)
      u = v;
    v = abs(diff);

  } while (1);
  return u << shift;
}

// best from http://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/
unsigned gcd_recursive(unsigned a, unsigned b) {
  if (b)
    return gcd_recursive(b, a % b);
  else
    return a;
}
// from http://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/
unsigned gcd_iterative_mod(unsigned a, unsigned b) {
  while (b) {
    unsigned t = b;
    b = a % b;
    a = t;
  }
  return a;
}

unsigned basicgcd(unsigned x, unsigned y) {
  return (x % y) == 0 ? y : basicgcd(y, x % y);
}
// attributed to  Jens
// Franke by Samuel Neves
unsigned gcdFranke(unsigned x, unsigned y) {
  unsigned f;
  unsigned s0, s1;

  if (0 == x)
    return y;
  if (0 == y)
    return x;

  s0 = __builtin_ctz(x);
  s1 = __builtin_ctz(y);
  f = s0 < s1 ? s0 : s1;
  x >>= s0;
  y >>= s1;

  while (x != y) {
    if (x < y) {
      y -= x;
      y >>= __builtin_ctz(y);
    } else {
      x -= y;
      x >>= __builtin_ctz(x);
    }
  }
  return x << f;
}

unsigned int test(unsigned int offset) {
  WallClockTimer timer;
  unsigned int N = 2000;
  cout << "gcd between numbers in [" << offset + 1 << " and " << offset + N
       << "]" << endl;
  int ti1 = 0;
  int ti2 = 0;
  int ti3 = 0;
  int ti4 = 0;
  int ti5 = 0;
  int ti6 = 0;
  int ti7 = 0;
  int ti8 = 0;
  int ti9 = 0;
  int ti10 = 0;

  int ti13 = 0;
  int ti14 = 0;
  int ti15 = 0;

  int bogus = 0;
  cout << "Running tests... ";

  for (unsigned int x = 1 + offset; x <= N + offset; ++x)
    for (unsigned int y = 1 + offset; y <= N + offset; ++y) {
      assert(gcdwikipedia2(x, y) == gcdwikipedia2fast(x, y));
      assert(gcdwikipedia2(x, y) == gcdwikipedia2fastswap(x, y));
#ifdef USE_ASSEMBLY
      assert(gcdwikipedia2(x, y) == gcdwikipedia2fastxchg(x, y));
#endif
      assert(gcdwikipedia2(x, y) == gcdwikipedia3fast(x, y));
      assert(gcdwikipedia2(x, y) == gcdwikipedia4fast(x, y));
      assert(gcdwikipedia2(x, y) == gcdwikipedia5fast(x, y));
#ifdef USE_ASSEMBLY
      assert(gcdwikipedia2(x, y) == gcdwikipedia6fastxchg(x, y));
#endif
      assert(gcdwikipedia2(x, y) == gcdwikipedia7fast(x, y));
      assert(gcdwikipedia2(x, y) == gcdwikipedia7fast32(x, y));
      assert(gcdwikipedia2(x, y) == gcd_recursive(x, y));
      assert(gcdwikipedia2(x, y) == gcd_iterative_mod(x, y));
      assert(gcdwikipedia2(x, y) == basicgcd(x, y));
      assert(gcdwikipedia2(x, y) == gcdFranke(x, y));
      assert(gcdwikipedia2(x, y) == gcdwikipedia8Spelvin(x, y));
    }

  cout << "Ok! " << endl;
  cout << "We proceed to report timings (smaller values are better)." << endl;

  double q = N * N;
  cout << "basicgcd                    ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += basicgcd(x, y);
  ti1 += timer.split();
  cout << q * 0.001 / ti1 << endl;
  cout << "gcdwikipedia2               ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia2(x, y);
  ti2 += timer.split();
  cout << q * 0.001 / ti2 << endl;
  cout << "gcdwikipedia2fast           ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia2fast(x, y);
  ti3 += timer.split();

  cout << q * 0.001 / ti3 << endl;
  cout << "gcd_recursive               ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcd_recursive(x, y);
  ti4 += timer.split();
  cout << q * 0.001 / ti4 << endl;
  cout << "gcd_iterative_mod           ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcd_iterative_mod(x, y);
  ti5 += timer.split();
  cout << q * 0.001 / ti5 << endl;
  cout << "gcdFranke                   ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdFranke(x, y);
  ti6 += timer.split();
  cout << q * 0.001 / ti6 << endl;
  cout << "gcdwikipedia3fast           ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia3fast(x, y);
  ti7 += timer.split();
  cout << q * 0.001 / ti7 << endl;
  cout << "gcdwikipedia4fast           ";
  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia4fast(x, y);
  ti8 += timer.split();
  cout << q * 0.001 / ti8 << endl;
  cout << "gcdwikipedia5fast           ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia5fast(x, y);
  ti9 += timer.split();
  cout << q * 0.001 / ti9 << endl;
  cout << "gcdwikipedia2fastswap       ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia2fastswap(x, y);
  ti10 += timer.split();
  cout << q * 0.001 / ti10 << endl;
#ifdef USE_ASSEMBLY
  int ti11 = 0;
  int ti12 = 0;
  cout << "gcdwikipedia6fastxchg       ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia6fastxchg(x, y);
  ti11 += timer.split();
  cout << q * 0.001 / ti11 << endl;
  cout << "gcdwikipedia2fastxchg       ";

  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia2fastxchg(x, y);
  ti12 += timer.split();
  cout << q * 0.001 / ti12 << endl;
#endif
  timer.reset();
  cout << "gcdwikipedia7fast           ";

  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia7fast(x, y);
  ti13 += timer.split();
  cout << q * 0.001 / ti13 << endl;
  cout << "gcdwikipedia7fast32         ";
  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia7fast32(x, y);
  ti14 += timer.split();
  cout << q * 0.001 / ti14 << endl;
  cout << "gcdwikipedia8Spelvin        ";
  timer.reset();
  for (unsigned int x = 1; x <= N; ++x)
    for (unsigned int y = 1; y <= N; ++y)
      bogus += gcdwikipedia8Spelvin(x, y);
  ti15 += timer.split();
  cout << q * 0.001 / ti15 << endl;
  return bogus;
}

int main() {
  assert(sizeof(long) == 8);
  assert(sizeof(int) == 4);
  unsigned int bogus = 0;
  bogus += test(0);
  cout << endl;
  bogus += test(1000 * 1000 * 1000);
  if (bogus == 0)
    abort();
}
