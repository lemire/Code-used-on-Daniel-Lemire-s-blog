```
$ make
$ ./hexparse
N= 1000000
empty function (overhead):
bogus
.instructions per cycle 2.50, cycles per 4-character hex string:  0.500, instructions per 4-character hex string: 1.250
min:   500187 cycles,  1250047 instructions, 	       2 branch mis.,        0 cache ref.,        0 cache mis.
avg: 500345.5 cycles, 1250047.0 instructions, 	     2.2 branch mis.,      1.7 cache ref.,      0.4 cache mis.
sum = 0
lookup:
instructions per cycle 3.98, cycles per 4-character hex string:  5.529, instructions per 4-character hex string: 22.000
min:  5529420 cycles, 22000020 instructions, 	       2 branch mis.,   137642 cache ref.,       57 cache mis.
avg: 5532375.6 cycles, 22000020.4 instructions, 	     2.1 branch mis., 138003.7 cache ref.,    198.0 cache mis.
sum = 1638571282800
lookup (inline):
instructions per cycle 4.21, cycles per 4-character hex string:  4.756, instructions per 4-character hex string: 20.000
min:  4755888 cycles, 20000020 instructions, 	       2 branch mis.,   137756 cache ref.,       63 cache mis.
avg: 4760954.4 cycles, 20000020.4 instructions, 	     2.2 branch mis., 138044.3 cache ref.,    171.6 cache mis.
sum = 1638571282800
lookup (alt):
instructions per cycle 3.98, cycles per 4-character hex string:  5.532, instructions per 4-character hex string: 22.000
min:  5531517 cycles, 22000020 instructions, 	       3 branch mis.,   137438 cache ref.,       52 cache mis.
avg: 5535752.2 cycles, 22000020.4 instructions, 	     3.7 branch mis., 138040.2 cache ref.,    105.0 cache mis.
sum = 1638571282800
math:
instructions per cycle 3.52, cycles per 4-character hex string:  10.793, instructions per 4-character hex string: 38.000
min: 10793309 cycles, 38000019 instructions, 	       2 branch mis.,   139894 cache ref.,       52 cache mis.
avg: 10852816.9 cycles, 38000019.9 instructions, 	     2.5 branch mis., 140373.0 cache ref.,     97.8 cache mis.
sum = 1638571282800
lookup4x32:
instructions per cycle 3.19, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 16.000
min:  5012965 cycles, 16000020 instructions, 	       3 branch mis.,   140466 cache ref.,       48 cache mis.
avg: 5020776.5 cycles, 16000020.4 instructions, 	     3.6 branch mis., 140661.3 cache ref.,     96.2 cache mis.
sum = 1637505178050
mayeut:
instructions per cycle 3.19, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 16.000
min:  5013128 cycles, 16000020 instructions, 	       3 branch mis.,   140040 cache ref.,       38 cache mis.
avg: 5013826.0 cycles, 16000020.4 instructions, 	     3.6 branch mis., 140266.0 cache ref.,     90.9 cache mis.
sum = 49080323397187350
mula:
instructions per cycle 3.59, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 18.000
min:  5013140 cycles, 18000020 instructions, 	       2 branch mis.,   139924 cache ref.,       44 cache mis.
avg: 5013850.0 cycles, 18000020.4 instructions, 	     2.8 branch mis., 140291.7 cache ref.,     96.7 cache mis.
sum = 1638571282800
lee:
instructions per cycle 3.83, cycles per 4-character hex string:  6.009, instructions per 4-character hex string: 23.000
min:  6009132 cycles, 23000020 instructions, 	       2 branch mis.,   139871 cache ref.,       50 cache mis.
avg: 6022668.1 cycles, 23000020.4 instructions, 	     2.4 branch mis., 140296.7 cache ref.,     98.3 cache mis.
sum = 1638571282800
aqrit:
instructions per cycle 3.72, cycles per 4-character hex string:  7.250, instructions per 4-character hex string: 27.000
min:  7250307 cycles, 27000020 instructions, 	       2 branch mis.,   139882 cache ref.,       51 cache mis.
avg: 7299364.4 cycles, 27000020.6 instructions, 	     3.5 branch mis., 140283.9 cache ref.,     86.4 cache mis.
sum = 1638571282800
SSE (uint64_t):
instructions per cycle 3.45, cycles per 4-character hex string:  9.287, instructions per 4-character hex string: 32.000
min:  9287142 cycles, 32000020 instructions, 	       3 branch mis.,   140112 cache ref.,       46 cache mis.
avg: 9305621.9 cycles, 32000020.7 instructions, 	     3.4 branch mis., 140431.2 cache ref.,    106.8 cache mis.
sum = 107317154237389250
big lookup:
instructions per cycle 2.60, cycles per 4-character hex string:  5.006, instructions per 4-character hex string: 13.000
min:  5005846 cycles, 13000020 instructions, 	       3 branch mis.,   141466 cache ref.,       45 cache mis.
avg: 5006528.4 cycles, 13000020.4 instructions, 	     3.6 branch mis., 141716.1 cache ref.,     95.9 cache mis.
sum = 1638571282800
big lookup (inline):
instructions per cycle 1.26, cycles per 4-character hex string:  2.083, instructions per 4-character hex string: 2.625
min:  2082753 cycles,  2625034 instructions, 	       3 branch mis.,   140605 cache ref.,       40 cache mis.
avg: 2084454.8 cycles, 2625034.2 instructions, 	     3.7 branch mis., 140909.1 cache ref.,     86.9 cache mis.
sum = 1638571282800
frozen_map:
instructions per cycle 1.33, cycles per 4-character hex string:  101.397, instructions per 4-character hex string: 135.001
min: 101396968 cycles, 135000618 instructions, 	 1997717 branch mis.,   136462 cache ref.,       42 cache mis.
avg: 101560147.8 cycles, 135000618.6 instructions, 	1999547.3 branch mis., 136949.4 cache ref.,    143.6 cache mis.
sum = 1638571282800
unfrozen_frozen_map:
instructions per cycle 1.56, cycles per 4-character hex string:  92.287, instructions per 4-character hex string: 143.513
min: 92287241 cycles, 143512607 instructions, 	 1351225 branch mis.,   139306 cache ref.,       44 cache mis.
avg: 92378910.9 cycles, 143512608.8 instructions, 	1353258.9 branch mis., 139744.8 cache ref.,    166.9 cache mis.
sum = 1638571282800
```
