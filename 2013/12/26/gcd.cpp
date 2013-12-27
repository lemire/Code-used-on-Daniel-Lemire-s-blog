// g++  -O3 -o gcd gcd.cpp

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>

using namespace std;
class WallClockTimer
{
public:
    struct timeval t1, t2;
public:
    WallClockTimer() : t1(), t2() {
        gettimeofday(&t1,0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1,0);
        t2 = t1;
    }
    int elapsed() {
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2,0);
        return elapsed();
    }
};

// from http://en.wikipedia.org/wiki/Binary_GCD_algorithm
unsigned int gcdwikipedia2(unsigned int u, unsigned int v)
{
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
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


// from http://en.wikipedia.org/wiki/Binary_GCD_algorithm
unsigned int gcdwikipedia2fast(unsigned int u, unsigned int v)
{
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
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

// from http://en.wikipedia.org/wiki/Binary_GCD_algorithm
unsigned int doesCTZhelp(unsigned int u, unsigned int v)
{
    int shift;
    if (u == 0) return v;
    if (v == 0) return u;
    shift = __builtin_ctz(u | v);
    u >>= shift;
    v >>= shift;
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

// best from http://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/
unsigned gcd_recursive(unsigned a, unsigned b)
{
    if (b)
        return gcd_recursive(b, a % b);
    else
        return a;
}
// from http://hbfs.wordpress.com/2013/12/10/the-speed-of-gcd/
unsigned gcd_iterative_mod(unsigned a, unsigned b)
{
    while (b)
    {
        unsigned t=b;
        b=a % b;
        a=t;
    }
    return a;
}


unsigned basicgcd(unsigned x, unsigned y) {
    return (x % y) == 0 ? y :  basicgcd(y,x % y);
}
// attributed to  Jens
// Franke by Samuel Neves 
unsigned gcdFranke (unsigned x, unsigned y)
{
  unsigned f;
  unsigned s0, s1;

  if(0 == x) return y;
  if(0 == y) return x;

  s0 = __builtin_ctz(x);
  s1 = __builtin_ctz(y);
  f = s0 < s1 ? s0 : s1; 
  x >>= s0;
  y >>= s1;

  while(x!=y)
  {
    if(x<y)
    {
      y-=x;
      y >>= __builtin_ctz(y);
    }
    else
    {
      x-=y;
      x >>= __builtin_ctz(x);
    }
  }
  return x<<f;
}

bool doesCTZhelps (unsigned x, unsigned y)
{
  unsigned f;
  unsigned s0, s1;

  if(0 == x) return y;
  if(0 == y) return x;

  s0 = __builtin_ctz(x);
  s1 = __builtin_ctz(y);
  f = s0 < s1 ? s0 : s1; 
  x >>= s0;
  y >>= s1;
  if(s0>0) return true;
  if(s1>0) return true;
  while(x!=y)
  {
    if(x<y)
    {
      y-=x;
  if(__builtin_ctz(y)>0) return true;

      y >>= __builtin_ctz(y);
    }
    else
    {
      x-=y;
      x >>= __builtin_ctz(x);
    }
  }
  return x<<f;
}


int main() {
    assert(sizeof(long)==8);
    assert(sizeof(int)==4);
    WallClockTimer timer;
    int N = 2000;
    int ti1 = 0;
    int ti2 = 0;
    int ti3 = 0;
    int ti4 = 0;
    int ti5 = 0;
    int ti6 = 0;
    int bogus = 0;
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y) {
            assert(gcdwikipedia2(x,y)==gcdwikipedia2fast(x,y));
            assert(gcdwikipedia2(x,y)==gcd_recursive(x,y));
            assert(gcdwikipedia2(x,y)==gcd_iterative_mod(x,y));
            assert(gcdwikipedia2(x,y)==basicgcd(x,y));
            assert(gcdwikipedia2(x,y)==gcdFranke(x,y));
         }
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  basicgcd(x,y);
    ti1 += timer.split();
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcdwikipedia2(x,y);
    ti2 += timer.split();
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcdwikipedia2fast(x,y);
    ti3 += timer.split();
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcd_recursive(x,y);
    ti4 += timer.split();
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcd_iterative_mod(x,y);
    ti5 += timer.split();
    timer.reset();
    for(unsigned int x = 1; x<=N; ++x)
        for(unsigned int y = 1; y<=N; ++y)
            bogus +=  gcdFranke(x,y);
    ti6 += timer.split();
    double q = (N-1)*(N-1);
    cout<<q*0.001/ti1<<" "<<q*0.001/ti2<<" "<<q*0.001/ti3<<" "<<q*0.001/ti4<<" "<<q*0.001/ti5<<" "<<q*0.001/ti6<<endl;
    return bogus;
}
