```
$ ./hexparse
N= 1000000
empty function (overhead):
bogus
.instructions per cycle 0.43, cycles per 4-character hex string:  0.000, instructions per 4-character hex string: 0.000
min:       54 cycles,       23 instructions, 	       0 branch mis.,        0 cache ref.,        0 cache mis.
avg:     59.2 cycles,     23.0 instructions, 	     0.1 branch mis.,      0.1 cache ref.,      0.0 cache mis.
sum = 0
lookup:
instructions per cycle 3.98, cycles per 4-character hex string:  5.530, instructions per 4-character hex string: 22.000
min:  5529743 cycles, 22000018 instructions, 	       2 branch mis.,    32891 cache ref.,        0 cache mis.
avg: 5533191.6 cycles, 22000018.5 instructions, 	     2.1 branch mis.,  33195.5 cache ref.,      0.5 cache mis.
sum = 1637654523550
lookup (inline):
instructions per cycle 4.07, cycles per 4-character hex string:  3.604, instructions per 4-character hex string: 14.667
min:  3603620 cycles, 14666699 instructions, 	       2 branch mis.,    33531 cache ref.,        0 cache mis.
avg: 3608947.5 cycles, 14666699.3 instructions, 	     3.4 branch mis.,  33966.8 cache ref.,      0.4 cache mis.
sum = 1637654523550
lookup (alt):
instructions per cycle 3.97, cycles per 4-character hex string:  5.541, instructions per 4-character hex string: 22.000
min:  5540630 cycles, 22000018 instructions, 	       2 branch mis.,    32316 cache ref.,        0 cache mis.
avg: 5544589.3 cycles, 22000018.5 instructions, 	     3.1 branch mis.,  32596.8 cache ref.,      0.1 cache mis.
sum = 1637654523550
math:
instructions per cycle 3.81, cycles per 4-character hex string:  9.974, instructions per 4-character hex string: 38.000
min:  9973654 cycles, 38000018 instructions, 	       2 branch mis.,    33299 cache ref.,        0 cache mis.
avg: 10006553.2 cycles, 38000018.8 instructions, 	     3.5 branch mis.,  33619.0 cache ref.,      0.4 cache mis.
sum = 1637654523550
mula:
instructions per cycle 3.56, cycles per 4-character hex string:  5.058, instructions per 4-character hex string: 18.000
min:  5058378 cycles, 18000018 instructions, 	       2 branch mis.,    33777 cache ref.,        0 cache mis.
avg: 5059414.8 cycles, 18000018.4 instructions, 	     2.5 branch mis.,  34041.6 cache ref.,     41.4 cache mis.
sum = 1637654523550
lee:
instructions per cycle 3.82, cycles per 4-character hex string:  6.020, instructions per 4-character hex string: 23.000
min:  6019654 cycles, 23000018 instructions, 	       2 branch mis.,    34008 cache ref.,        0 cache mis.
avg: 6022596.9 cycles, 23000018.5 instructions, 	     3.3 branch mis.,  34293.3 cache ref.,      0.1 cache mis.
sum = 1637654523550
aqrit:
instructions per cycle 3.82, cycles per 4-character hex string:  7.069, instructions per 4-character hex string: 27.000
min:  7069056 cycles, 27000018 instructions, 	       2 branch mis.,    34038 cache ref.,        0 cache mis.
avg: 7070929.8 cycles, 27000018.6 instructions, 	     3.4 branch mis.,  34286.8 cache ref.,      0.5 cache mis.
sum = 1637654523550
SSE (uint64_t):
instructions per cycle 3.72, cycles per 4-character hex string:  8.614, instructions per 4-character hex string: 32.000
min:  8613721 cycles, 32000018 instructions, 	       2 branch mis.,    34747 cache ref.,        0 cache mis.
avg: 8693404.5 cycles, 32000018.7 instructions, 	     3.3 branch mis.,  34972.5 cache ref.,      0.0 cache mis.
sum = 107326502502588700
big lookup:
instructions per cycle 2.58, cycles per 4-character hex string:  5.034, instructions per 4-character hex string: 13.000
min:  5034076 cycles, 13000018 instructions, 	       2 branch mis.,    32854 cache ref.,        0 cache mis.
avg: 5049987.9 cycles, 13000018.4 instructions, 	     3.7 branch mis.,  33097.6 cache ref.,      6.8 cache mis.
sum = 1637654523550
big lookup (inline):
instructions per cycle 4.17, cycles per 4-character hex string:  2.280, instructions per 4-character hex string: 9.500
min:  2280029 cycles,  9500028 instructions, 	       3 branch mis.,    34048 cache ref.,        0 cache mis.
avg: 2284335.5 cycles, 9500028.2 instructions, 	     4.6 branch mis.,  34244.9 cache ref.,      1.2 cache mis.
sum = 1637654523550
frozen_map:
instructions per cycle 1.97, cycles per 4-character hex string:  68.505, instructions per 4-character hex string: 135.003
min: 68504568 cycles, 135003138 instructions, 	  501098 branch mis.,    34175 cache ref.,        0 cache mis.
avg: 72108319.4 cycles, 135003138.7 instructions, 	649272.2 branch mis.,  34727.7 cache ref.,    125.6 cache mis.
sum = 1637654523550
unfrozen_frozen_map:
instructions per cycle 2.44, cycles per 4-character hex string:  58.821, instructions per 4-character hex string: 143.544
min: 58820910 cycles, 143543889 instructions, 	  337680 branch mis.,    34574 cache ref.,        0 cache mis.
avg: 59056374.6 cycles, 143543889.6 instructions, 	344375.0 branch mis.,  34956.3 cache ref.,      4.0 cache mis.
sum = 1637654523550
```
