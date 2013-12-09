/**
 * blogbitdetect.cpp
 * Daniel Lemire, April 2012
 * http://lemire.me/en/
 * This software is in the public domain.
*
* Bit packing is fast, but integer logarithm is slow
* http://lemire.me/blog/archives/2012/04/05/bit-packing-is-fast-but-integer-logarithm-is-slow/
*
 $ g++-4 --version
 g++-4 (GCC) 4.6.2
 $ g++-4 -Ofast -o blogbitdetect blogbitdetect.cpp
 $ ./blogbitdetect
1	186.4 1	309.3 1	195.1 1	958.7 1	3196 1	1864
2	166.5 2	316.6 2	305 2	1082 2	3196 2	1864
3	156.4 3	305 3	401.8 3	1082 3	3196 3	1864
4	143.4 4	332.2 4	493.4 4	1100 4	3196 4	2034
5	133.2 5	306.4 5	545.6 5	1032 5	3355 5	1766
6	121.8 6	280.8 6	578.5 6	1032 6	3196 6	1678
7	115.3 7	316.6 7	578.5 7	1032 7	3050 7	1525
8	106.9 8	315.1 8	610.1 8	1065 8	3196 8	2165
9	97.83 9	315.1 9	588.7 9	1017 9	3196 9	2165
10	92.95 10	318.1 10	593.9 10	1032 10	3355 10	1525
11	86.26 11	312.1 11	593.9 11	1017 11	3355 11	1370
12	80.37 12	319.6 12	599.2 12	1049 12	3196 12	1428
13	74.32 13	315.1 13	588.7 13	1017 13	3355 13	1316
14	67.99 14	315.1 14	610.1 14	1065 14	2918 14	1291
15	63.37 15	316.6 15	593.9 15	1049 15	3355 15	1370
16	62.08 16	303.7 16	568.7 16	1002 16	3196 16	1766
17	62.37 17	309.3 17	588.7 17	1032 17	3196 17	1198
18	59.87 18	318.1 18	604.6 18	1049 18	3355 18	1198
19	56.21 19	318.1 19	610.1 19	1032 19	2918 19	1198
20	53.82 20	313.6 20	593.9 20	1032 20	2796 20	1177
21	51.62 21	319.6 21	599.2 21	1032 21	3355 21	1118
22	50.19 22	306.4 22	583.6 22	1032 22	3355 22	1177
23	48.42 23	318.1 23	593.9 23	1049 23	3196 23	1049
24	47.43 24	306.4 24	588.7 24	1032 24	3050 24	1137
25	45.87 25	316.6 25	593.9 25	1049 25	2796 25	1032
26	44.35 26	316.6 26	599.2 26	1049 26	3355 26	1017
27	42.85 27	316.6 27	588.7 27	1032 27	3355 27	1032
28	40.87 28	312.1 28	593.9 28	1065 28	3355 28	1032
29	39.52 29	313.6 29	615.7 29	932.1 29	2918 29	945.2
30	38.3 30	310.7 30	593.9 30	1065 30	3196 30	958.7
31	37.64 31	312.1 31	610.1 31	1049 31	3355 31	958.7
32	37.43 32	319.6 31	604.6 32	1032 31	3050 31	972.6	 */
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>
#include <cstdlib>

typedef unsigned int uint32;

using namespace std;

vector<uint32> generateArray32(uint N, const uint mask = 0xFFFFFFFF) {
	vector<uint32> ans(N);
	for (size_t k = 0; k < N; ++k)
		ans[k] = rand() & mask;
	return ans;
}

class ZTimer {
public:
	struct timeval t1, t2;
public:
	ZTimer() :
		t1(), t2() {
		gettimeofday(&t1, 0);
		t2 = t1;
	}
	void reset() {
		gettimeofday(&t1, 0);
		t2 = t1;
	}
	int elapsed() {
		return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
	}
	int split() {
		gettimeofday(&t2, 0);
		return elapsed();
	}
};

/*
 uint asmbits(const uint v) {
 if(v==0) return 0;
 uint32       answer;
 __asm__("bsr %1, %0;" :"=r"(answer) :"r"(v));
 return answer+1;
 }*/

uint gccbits(const uint v) {
	if (v == 0)
		return 0;
	return 32 - __builtin_clz(v);
}

uint slowbits(uint v) {
	uint r = 0;
	while (v) {
		r++;
		v = v >> 1;
	}
	return r;
}
uint bits(uint v) {
	uint r(0);
	if (v >= (1U << 15)) {
		v >>= 16;
		r += 16;
	}
	if (v >= (1U << 7)) {
		v >>= 8;
		r += 8;
	}
	if (v >= (1U << 3)) {
		v >>= 4;
		r += 4;
	}
	if (v >= (1U << 1)) {
		v >>= 2;
		r += 2;
	}
	if (v >= (1U << 0)) {
		v >>= 1;
		r += 1;
	}
	return r;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask1(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 21;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 23;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 25;
	++in;
	*out |= ((*in)) << 26;
	++in;
	*out |= ((*in)) << 27;
	++in;
	*out |= ((*in)) << 28;
	++in;
	*out |= ((*in)) << 29;
	++in;
	*out |= ((*in)) << 30;
	++in;
	*out |= ((*in)) << 31;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask2(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 26;
	++in;
	*out |= ((*in)) << 28;
	++in;
	*out |= ((*in)) << 30;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 26;
	++in;
	*out |= ((*in)) << 28;
	++in;
	*out |= ((*in)) << 30;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask3(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 21;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 27;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (3 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 25;
	++in;
	*out |= ((*in)) << 28;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (3 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 23;
	++in;
	*out |= ((*in)) << 26;
	++in;
	*out |= ((*in)) << 29;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask5(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 25;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (5 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 23;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (5 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 21;
	++in;
	*out |= ((*in)) << 26;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (5 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (5 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 27;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask6(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (6 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (6 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 26;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (6 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (6 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 26;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask7(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 21;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (7 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 24;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (7 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (7 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 23;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (7 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (7 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (7 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 25;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask9(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (9 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 22;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (9 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (9 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 21;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (9 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (9 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (9 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (9 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (9 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 23;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask10(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (10 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (10 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (10 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (10 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 22;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (10 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (10 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (10 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (10 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 22;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask11(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (11 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (11 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (11 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (11 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (11 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (11 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (11 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (11 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 19;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (11 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 20;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (11 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 21;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask12(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (12 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (12 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 20;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (12 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (12 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 20;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (12 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (12 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 20;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (12 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (12 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 20;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask13(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (13 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (13 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (13 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (13 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (13 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (13 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (13 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (13 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 17;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (13 - 11);
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (13 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 18;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (13 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (13 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 19;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask14(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (14 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (14 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (14 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (14 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (14 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (14 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 18;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (14 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (14 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (14 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (14 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (14 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (14 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 18;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask15(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 15;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (15 - 13);
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (15 - 11);
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (15 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (15 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (15 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (15 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (15 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 16;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (15 - 14);
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (15 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (15 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (15 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (15 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (15 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (15 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 17;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask17(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (17 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (17 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (17 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (17 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (17 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (17 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (17 - 14);
	++in;
	*out |= ((*in)) << 14;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (17 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (17 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (17 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (17 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (17 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (17 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (17 - 11);
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (17 - 13);
	++in;
	*out |= ((*in)) << 13;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (17 - 15);
	++in;
	*out |= ((*in)) << 15;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask18(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (18 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (18 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (18 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (18 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (18 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (18 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (18 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (18 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (18 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (18 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (18 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (18 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (18 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (18 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (18 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (18 - 14);
	++in;
	*out |= ((*in)) << 14;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask19(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (19 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (19 - 12);
	++in;
	*out |= ((*in)) << 12;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (19 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (19 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (19 - 11);
	++in;
	*out |= ((*in)) << 11;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (19 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (19 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (19 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (19 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (19 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (19 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (19 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (19 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (19 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (19 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (19 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (19 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (19 - 13);
	++in;
	*out |= ((*in)) << 13;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask20(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (20 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (20 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (20 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (20 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (20 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (20 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (20 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (20 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (20 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (20 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (20 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (20 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (20 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (20 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (20 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (20 - 12);
	++in;
	*out |= ((*in)) << 12;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask21(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (21 - 10);
	++in;
	*out |= ((*in)) << 10;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (21 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (21 - 9);
	++in;
	*out |= ((*in)) << 9;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (21 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (21 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (21 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (21 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (21 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (21 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (21 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (21 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (21 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (21 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (21 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (21 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (21 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (21 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (21 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (21 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (21 - 11);
	++in;
	*out |= ((*in)) << 11;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask22(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (22 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (22 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (22 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (22 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (22 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (22 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (22 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (22 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (22 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (22 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (22 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (22 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (22 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (22 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (22 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (22 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (22 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (22 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (22 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (22 - 10);
	++in;
	*out |= ((*in)) << 10;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask23(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (23 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (23 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (23 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (23 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (23 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (23 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (23 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (23 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (23 - 11);
	++in;
	*out |= ((*in)) << 11;
	++out;
	*out |= ((*in)) >> (23 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (23 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (23 - 7);
	++in;
	*out |= ((*in)) << 7;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (23 - 21);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (23 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (23 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (23 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (23 - 8);
	++in;
	*out |= ((*in)) << 8;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (23 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (23 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (23 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (23 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (23 - 9);
	++in;
	*out |= ((*in)) << 9;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask24(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (24 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (24 - 8);
	++in;
	*out |= ((*in)) << 8;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask25(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (25 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (25 - 11);
	++in;
	*out |= ((*in)) << 11;
	++out;
	*out |= ((*in)) >> (25 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (25 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (25 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (25 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (25 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (25 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (25 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (25 - 5);
	++in;
	*out |= ((*in)) << 5;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (25 - 23);
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (25 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (25 - 9);
	++in;
	*out |= ((*in)) << 9;
	++out;
	*out |= ((*in)) >> (25 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (25 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (25 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (25 - 6);
	++in;
	*out |= ((*in)) << 6;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (25 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (25 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (25 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (25 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (25 - 21);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (25 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (25 - 7);
	++in;
	*out |= ((*in)) << 7;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask26(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (26 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (26 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (26 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (26 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (26 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (26 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (26 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (26 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (26 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (26 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (26 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (26 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (26 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (26 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (26 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (26 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (26 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (26 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (26 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (26 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (26 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (26 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (26 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (26 - 6);
	++in;
	*out |= ((*in)) << 6;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask27(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (27 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (27 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (27 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (27 - 7);
	++in;
	*out |= ((*in)) << 7;
	++out;
	*out |= ((*in)) >> (27 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (27 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (27 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (27 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (27 - 9);
	++in;
	*out |= ((*in)) << 9;
	++out;
	*out |= ((*in)) >> (27 - 4);
	++in;
	*out |= ((*in)) << 4;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (27 - 26);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (27 - 21);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (27 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (27 - 11);
	++in;
	*out |= ((*in)) << 11;
	++out;
	*out |= ((*in)) >> (27 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	*out |= ((*in)) >> (27 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (27 - 23);
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (27 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (27 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (27 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (27 - 3);
	++in;
	*out |= ((*in)) << 3;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (27 - 25);
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (27 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (27 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (27 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (27 - 5);
	++in;
	*out |= ((*in)) << 5;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask28(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (28 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (28 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (28 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (28 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (28 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (28 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (28 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (28 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (28 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (28 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (28 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (28 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (28 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (28 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (28 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (28 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (28 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (28 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (28 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (28 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (28 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (28 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (28 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (28 - 4);
	++in;
	*out |= ((*in)) << 4;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask29(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (29 - 26);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (29 - 23);
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (29 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (29 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (29 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (29 - 11);
	++in;
	*out |= ((*in)) << 11;
	++out;
	*out |= ((*in)) >> (29 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (29 - 5);
	++in;
	*out |= ((*in)) << 5;
	++out;
	*out |= ((*in)) >> (29 - 2);
	++in;
	*out |= ((*in)) << 2;
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (29 - 28);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (29 - 25);
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (29 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (29 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (29 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (29 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (29 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (29 - 7);
	++in;
	*out |= ((*in)) << 7;
	++out;
	*out |= ((*in)) >> (29 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	*out |= ((*in)) >> (29 - 1);
	++in;
	*out |= ((*in)) << 1;
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (29 - 27);
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (29 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (29 - 21);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (29 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (29 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (29 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (29 - 9);
	++in;
	*out |= ((*in)) << 9;
	++out;
	*out |= ((*in)) >> (29 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	*out |= ((*in)) >> (29 - 3);
	++in;
	*out |= ((*in)) << 3;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask30(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (30 - 28);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (30 - 26);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (30 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (30 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (30 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (30 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (30 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (30 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (30 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (30 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (30 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (30 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	*out |= ((*in)) >> (30 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	*out |= ((*in)) >> (30 - 2);
	++in;
	*out |= ((*in)) << 2;
	++out;
	++in;
	*out |= (*in);
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (30 - 28);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (30 - 26);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (30 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (30 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (30 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (30 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (30 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (30 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (30 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (30 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (30 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (30 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	*out |= ((*in)) >> (30 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	*out |= ((*in)) >> (30 - 2);
	++in;
	*out |= ((*in)) << 2;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask31(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out |= (*in);
	++in;
	*out |= ((*in)) << 31;
	++out;
	*out |= ((*in)) >> (31 - 30);
	++in;
	*out |= ((*in)) << 30;
	++out;
	*out |= ((*in)) >> (31 - 29);
	++in;
	*out |= ((*in)) << 29;
	++out;
	*out |= ((*in)) >> (31 - 28);
	++in;
	*out |= ((*in)) << 28;
	++out;
	*out |= ((*in)) >> (31 - 27);
	++in;
	*out |= ((*in)) << 27;
	++out;
	*out |= ((*in)) >> (31 - 26);
	++in;
	*out |= ((*in)) << 26;
	++out;
	*out |= ((*in)) >> (31 - 25);
	++in;
	*out |= ((*in)) << 25;
	++out;
	*out |= ((*in)) >> (31 - 24);
	++in;
	*out |= ((*in)) << 24;
	++out;
	*out |= ((*in)) >> (31 - 23);
	++in;
	*out |= ((*in)) << 23;
	++out;
	*out |= ((*in)) >> (31 - 22);
	++in;
	*out |= ((*in)) << 22;
	++out;
	*out |= ((*in)) >> (31 - 21);
	++in;
	*out |= ((*in)) << 21;
	++out;
	*out |= ((*in)) >> (31 - 20);
	++in;
	*out |= ((*in)) << 20;
	++out;
	*out |= ((*in)) >> (31 - 19);
	++in;
	*out |= ((*in)) << 19;
	++out;
	*out |= ((*in)) >> (31 - 18);
	++in;
	*out |= ((*in)) << 18;
	++out;
	*out |= ((*in)) >> (31 - 17);
	++in;
	*out |= ((*in)) << 17;
	++out;
	*out |= ((*in)) >> (31 - 16);
	++in;
	*out |= ((*in)) << 16;
	++out;
	*out |= ((*in)) >> (31 - 15);
	++in;
	*out |= ((*in)) << 15;
	++out;
	*out |= ((*in)) >> (31 - 14);
	++in;
	*out |= ((*in)) << 14;
	++out;
	*out |= ((*in)) >> (31 - 13);
	++in;
	*out |= ((*in)) << 13;
	++out;
	*out |= ((*in)) >> (31 - 12);
	++in;
	*out |= ((*in)) << 12;
	++out;
	*out |= ((*in)) >> (31 - 11);
	++in;
	*out |= ((*in)) << 11;
	++out;
	*out |= ((*in)) >> (31 - 10);
	++in;
	*out |= ((*in)) << 10;
	++out;
	*out |= ((*in)) >> (31 - 9);
	++in;
	*out |= ((*in)) << 9;
	++out;
	*out |= ((*in)) >> (31 - 8);
	++in;
	*out |= ((*in)) << 8;
	++out;
	*out |= ((*in)) >> (31 - 7);
	++in;
	*out |= ((*in)) << 7;
	++out;
	*out |= ((*in)) >> (31 - 6);
	++in;
	*out |= ((*in)) << 6;
	++out;
	*out |= ((*in)) >> (31 - 5);
	++in;
	*out |= ((*in)) << 5;
	++out;
	*out |= ((*in)) >> (31 - 4);
	++in;
	*out |= ((*in)) << 4;
	++out;
	*out |= ((*in)) >> (31 - 3);
	++in;
	*out |= ((*in)) << 3;
	++out;
	*out |= ((*in)) >> (31 - 2);
	++in;
	*out |= ((*in)) << 2;
	++out;
	*out |= ((*in)) >> (31 - 1);
	++in;
	*out |= ((*in)) << 1;
}

/*assumes that integers fit in the prescribed number of bits */
void __fastpackwithoutmask32(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
	++out;
	++in;
	*out = *in;
}

/*assumes that integers fit in the prescribed number of bits*/
void __fastpackwithoutmask4(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	for (uint outer = 0; outer < 4; ++outer) {
		for (uint inwordpointer = 0; inwordpointer < 32; inwordpointer += 4)
			*out |= ((*(in++))) << inwordpointer;
		++out;
	}
}

/*assumes that integers fit in the prescribed number of bits*/
void __fastpackwithoutmask8(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	for (uint outer = 0; outer < 8; ++outer) {
		for (uint inwordpointer = 0; inwordpointer < 32; inwordpointer += 8)
			*out |= ((*(in++))) << inwordpointer;
		++out;
	}
}

/*assumes that integers fit in the prescribed number of bits*/
void __fastpackwithoutmask16(const uint32 * __restrict__ in,
		uint32 * __restrict__ out) {
	for (uint outer = 0; outer < 16; ++outer) {
		for (uint inwordpointer = 0; inwordpointer < 32; inwordpointer += 16)
			*out |= ((*(in++))) << inwordpointer;
		++out;
	}
}

/*assumes that integers fit in the prescribed number of bits*/
void fastpackwithoutmask(const uint32 * __restrict__ in,
		uint32 * __restrict__ out, const uint bit) {
	// Could have used function pointers instead of switch.
	// Switch calls do offer the compiler more opportunities for optimization in
	// theory. In this case, it makes no difference with a good compiler.
	switch (bit) {
	case 1:
		__fastpackwithoutmask1(in, out);
		break;
	case 2:
		__fastpackwithoutmask2(in, out);
		break;
	case 3:
		__fastpackwithoutmask3(in, out);
		break;
	case 4:
		__fastpackwithoutmask4(in, out);
		break;
	case 5:
		__fastpackwithoutmask5(in, out);
		break;
	case 6:
		__fastpackwithoutmask6(in, out);
		break;
	case 7:
		__fastpackwithoutmask7(in, out);
		break;
	case 8:
		__fastpackwithoutmask8(in, out);
		break;
	case 9:
		__fastpackwithoutmask9(in, out);
		break;
	case 10:
		__fastpackwithoutmask10(in, out);
		break;
	case 11:
		__fastpackwithoutmask11(in, out);
		break;
	case 12:
		__fastpackwithoutmask12(in, out);
		break;
	case 13:
		__fastpackwithoutmask13(in, out);
		break;
	case 14:
		__fastpackwithoutmask14(in, out);
		break;
	case 15:
		__fastpackwithoutmask15(in, out);
		break;
	case 16:
		__fastpackwithoutmask16(in, out);
		break;
	case 17:
		__fastpackwithoutmask17(in, out);
		break;
	case 18:
		__fastpackwithoutmask18(in, out);
		break;
	case 19:
		__fastpackwithoutmask19(in, out);
		break;
	case 20:
		__fastpackwithoutmask20(in, out);
		break;
	case 21:
		__fastpackwithoutmask21(in, out);
		break;
	case 22:
		__fastpackwithoutmask22(in, out);
		break;
	case 23:
		__fastpackwithoutmask23(in, out);
		break;
	case 24:
		__fastpackwithoutmask24(in, out);
		break;
	case 25:
		__fastpackwithoutmask25(in, out);
		break;
	case 26:
		__fastpackwithoutmask26(in, out);
		break;
	case 27:
		__fastpackwithoutmask27(in, out);
		break;
	case 28:
		__fastpackwithoutmask28(in, out);
		break;
	case 29:
		__fastpackwithoutmask29(in, out);
		break;
	case 30:
		__fastpackwithoutmask30(in, out);
		break;
	case 31:
		__fastpackwithoutmask31(in, out);
		break;
	case 32:
		__fastpackwithoutmask32(in, out);
		break;
	default:
		break;
	}
}

int main() {
	const uint N = 1U << 26;

	for (uint b = 1; b <= 32; ++b) {
		cout << b << "\t";
		const uint MAXV = b < 32 ? (1U << b) - 1 : 0xFFFFFFFF;
		vector<uint32> data = generateArray32(N, MAXV);
		for (int k = 0; k < 16; ++k)
			data.push_back(MAXV);
		vector<uint32> out(N);// just a tmp store


		ZTimer z;
		uint detectedb = 0;

		z.reset();
		detectedb = 0;
		for (vector<uint32>::const_iterator i = data.begin(); i != data.end(); ++i) {
			const uint tmpbits = slowbits(*i);
			if (detectedb < tmpbits)
				detectedb = tmpbits;
		}
		cout << std::setprecision(4) << N * 0.001 / z.split() << " "
				<< detectedb << "\t";

		z.reset();
		detectedb = 0;
		for (vector<uint32>::const_iterator i = data.begin(); i != data.end(); ++i) {
			const uint tmpbits = bits(*i);
			if (detectedb < tmpbits)
				detectedb = tmpbits;
		}

		cout << std::setprecision(4) << N * 0.001 / z.split() << " "
				<< detectedb << "\t";
		z.reset();
		uint myclz = 32;
		for (vector<uint32>::const_iterator i = data.begin(); i != data.end(); ++i) {
			if (*i == 0)
				continue;
			const uint tmpbits = __builtin_clz(*i);
			if (myclz > tmpbits)
				myclz = tmpbits;
		}
		detectedb = 32 - myclz;

		cout << std::setprecision(4) << N * 0.001 / z.split() << " "
				<< detectedb << "\t";

		z.reset();
		detectedb = bits(data[0]);
		if (data.size() % 1 != 0) {
			for (vector<uint32>::const_iterator i = data.begin() + 1; i
					!= data.end(); i += 2) {
				if ((*i >> detectedb) > 0)
					detectedb = bits(*i);
				if ((*(i + 1) >> detectedb) > 0)
					detectedb = bits(*(i + 1));
			}
		} else {
			for (vector<uint32>::const_iterator i = data.begin(); i
					!= data.end(); i += 2) {
				if ((*i >> detectedb) > 0)
					detectedb = bits(*i);
				if ((*(i + 1) >> detectedb) > 0)
					detectedb = bits(*(i + 1));
			}
		}
		cout << std::setprecision(4) << N * 0.001 / z.split() << " "
				<< detectedb << "\t";

		z.reset();
		uint accumulator = data[0];
		for (vector<uint32>::const_iterator i = data.begin() + 1; i
				!= data.end(); ++i) {
			accumulator |= *i;
		}
		detectedb = bits(accumulator);
		cout << std::setprecision(4) << N * 0.001 / z.split() << " "
				<< detectedb << "\t";


		z.reset();
		detectedb = bits(data[0]);
		const size_t N = data.size();
		for (uint k = 0; k < N / 32; ++k) {
			fastpackwithoutmask(&data[0] + 32 * k,
					&out[0] + (32 * detectedb) * k / 32, detectedb);
		}
		cout << std::setprecision(4) << N * 0.001 / z.split() << "\t";

		cout << endl;
	}
	return 0;
}
