#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#define TEST_32BIT  (1)   // 0 = 64-bit conversion, 1 = 32-bit conversion

#if TEST_32BIT

bool to_int32_simple (float x, int32_t *out) 
{
    int32_t tmp = int32_t(x);
    *out = tmp;
    return tmp == x;
}

#define BITS_PER_INT     (32)
#define STORED_MANT_BITS (23)
#define EXPO_BIAS        (127)
#define EXPO_BITS        (8)

bool to_int32_njuffa (float x, int32_t *out)
{
    const uint32_t zero = uint32_t(0);
    const uint32_t one = uint32_t(EXPO_BIAS) << STORED_MANT_BITS;
    const uint32_t max_int = one + (uint32_t(BITS_PER_INT - 1) << STORED_MANT_BITS);
    const uint32_t lo_bound = 2 * one;
    uint32_t ix, abs_2x, raw_expo, sign_x, hi_bound;
    bool is_zero, frac_zero;
    int shift;
    *out = int32_t(x);
    memcpy (&ix, &x, sizeof ix);
    abs_2x = 2 * ix;
    raw_expo = abs_2x >> (STORED_MANT_BITS + 1);
    sign_x = ix >> (BITS_PER_INT - 1);
    hi_bound = 2 * max_int + sign_x; // result in non-symmetric interval [-(2**31), (2**31)-1]
    shift = BITS_PER_INT - (EXPO_BIAS + STORED_MANT_BITS + 1 + EXPO_BITS - raw_expo);
    is_zero = (abs_2x == zero);
    frac_zero = (abs_2x >= lo_bound) && (abs_2x < hi_bound) && (((abs_2x << EXPO_BITS) << shift) == 0);
    return is_zero || frac_zero;
}

#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
double second (void)
{
    LARGE_INTEGER t;
    static double oofreq;
    static int checkedForHighResTimer;
    static BOOL hasHighResTimer;

    if (!checkedForHighResTimer) {
        hasHighResTimer = QueryPerformanceFrequency (&t);
        oofreq = 1.0 / (double)t.QuadPart;
        checkedForHighResTimer = 1;
    }
    if (hasHighResTimer) {
        QueryPerformanceCounter (&t);
        return (double)t.QuadPart * oofreq;
    } else {
        return (double)GetTickCount() * 1.0e-3;
    }
}
#elif defined(__linux__) || defined(__APPLE__)
#include <stddef.h>
#include <sys/time.h>
double second (void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}
#else
#error unsupported platform
#endif

int main (void)
{
    uint32_t sum, i = 0;
    double start, stop;
    float x;
    int32_t res_int, ref_int;
    bool res_bool, ref_bool;

    printf ("testing 32-bit conversion\n");

    do {
        memcpy (&x, &i, sizeof x);
        res_bool = to_int32_njuffa (x, &res_int);
        ref_bool = to_int32_simple (x, &ref_int); 
        if (res_bool != ref_bool) {
            printf ("i=%08x %15.8e: res_bool=%d  ref_bool=%d\n", i, x, res_bool, ref_bool);
            return EXIT_FAILURE;
        }
        if (res_int != ref_int) {
            printf ("i=%08x %15.8e: res_int=%d  ref_int=%d\n", i, x, res_int, ref_int);
            return EXIT_FAILURE;
        }
        i++;
    } while (i);

    printf ("exhaustive functional test passed\n");

    sum = 0;
    for (int j = 0; j < 2; j++) {
        start = second();
        i = 0;
        do {
            memcpy (&x, &i, sizeof x);
            ref_bool = to_int32_simple (x, &ref_int);
            sum += uint32_t (ref_int);
            i++;
        } while (i);
        stop = second();
        if (j) printf ("sum=%08x\relapsed simple = %.3f seconds\n", 
                       sum, stop - start);
    }

    sum = 0;
    for (int j = 0; j < 2; j++) {
        start = second();
        i = 0;
        do {
            memcpy (&x, &i, sizeof x);
            res_bool = to_int32_njuffa (x, &res_int);
            sum += uint32_t (res_int);
            i++;
        } while (i);
        stop = second();
        if (j) printf ("sum=%08x\relapsed njuffa = %.3f seconds\n", 
                       sum, stop - start);
    }

    return EXIT_SUCCESS;
}

#else // TEST_32BIT

bool to_int64_simple (double x, int64_t *out) 
{
    int64_t tmp = int64_t(x);
    *out = tmp;
    return tmp == x;
}

#define BITS_PER_INT     (64)
#define STORED_MANT_BITS (52)
#define EXPO_BIAS        (1023)
#define EXPO_BITS        (11)

bool to_int64_njuffa (double x, int64_t *out)
{
    const uint64_t zero = uint64_t(0);
    const uint64_t one = uint64_t(EXPO_BIAS) << STORED_MANT_BITS;
    const uint64_t max_int = one + (uint64_t(BITS_PER_INT - 1) << STORED_MANT_BITS);
    const uint64_t lo_bound = 2 * one;
    uint64_t ix, abs_2x, raw_expo, sign_x, hi_bound;
    bool is_zero, frac_zero;
    int shift;
    *out = int64_t(x);
    memcpy (&ix, &x, sizeof ix);
    abs_2x = 2 * ix;
    raw_expo = abs_2x >> (STORED_MANT_BITS + 1);
    sign_x = ix >> (BITS_PER_INT - 1);
    hi_bound = 2 * max_int + sign_x; // result in non-symmetric interval [-(2**63), (2**63)-1]
    shift = BITS_PER_INT - (EXPO_BIAS + STORED_MANT_BITS + 1 + EXPO_BITS - raw_expo);
    is_zero = (abs_2x == zero);
    frac_zero = (abs_2x >= lo_bound) && (abs_2x < hi_bound) && (((abs_2x << EXPO_BITS) << shift) == 0);
    return is_zero || frac_zero;
}

/*
  https://groups.google.com/forum/#!original/comp.lang.c/qFv18ql_WlU/IK8KGZZFJx4J
  From: geo <gmars...@gmail.com>
  Newsgroups: sci.math,comp.lang.c,comp.lang.fortran
  Subject: 64-bit KISS RNGs
  Date: Sat, 28 Feb 2009 04:30:48 -0800 (PST)

  This 64-bit KISS RNG has three components, each nearly
  good enough to serve alone.    The components are:
  Multiply-With-Carry (MWC), period (2^121+2^63-1)
  Xorshift (XSH), period 2^64-1
  Congruential (CNG), period 2^64
*/
static uint64_t kiss64_x = 1234567890987654321ULL;
static uint64_t kiss64_c = 123456123456123456ULL;
static uint64_t kiss64_y = 362436362436362436ULL;
static uint64_t kiss64_z = 1066149217761810ULL;
static uint64_t kiss64_t;
#define MWC64  (kiss64_t = (kiss64_x << 58) + kiss64_c, \
                kiss64_c = (kiss64_x >> 6), kiss64_x += kiss64_t, \
                kiss64_c += (kiss64_x < kiss64_t), kiss64_x)
#define XSH64  (kiss64_y ^= (kiss64_y << 13), kiss64_y ^= (kiss64_y >> 17), \
                kiss64_y ^= (kiss64_y << 43))
#define CNG64  (kiss64_z = 6906969069ULL * kiss64_z + 1234567ULL)
#define KISS64 (MWC64 + XSH64 + CNG64)

int main (void)
{
    uint64_t i;
    double x;
    int64_t res_int, ref_int;
    bool res_bool, ref_bool;

    printf ("testing 64-bit conversion\n");

    do {
        i = KISS64;
        memcpy (&x, &i, sizeof x);
        res_bool = to_int64_njuffa (x, &res_int);
        ref_bool = to_int64_simple (x, &ref_int); 
        if (res_bool != ref_bool) {
            printf ("i=%016llx %23.16e: res_bool=%d  ref_bool=%d\n", i, x, res_bool, ref_bool);
            return EXIT_FAILURE;
        }
        if (res_int != ref_int) {
            printf ("i=%016llx %23.16e: res_int=%lld  ref_int=%lld\n", i, x, res_int, ref_int);
            return EXIT_FAILURE;
        }
    } while (i);
    return EXIT_SUCCESS;
}

#endif // TEST_32BIT
