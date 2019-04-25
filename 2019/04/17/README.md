```
$ make
$ ./hexparse
N= 1000000
empty function (overhead):
bogus
.instructions per cycle 2.25, cycles per 4-character hex string:  4.000, instructions per 4-character hex string: 9.000
min:  4000106 cycles,  9000020 instructions, 	       2 branch mis.,        0 cache ref.,        0 cache mis.
avg: 4000316.7 cycles, 9000020.3 instructions, 	     2.1 branch mis.,      2.0 cache ref.,      0.3 cache mis.
sum = 0
lookup:
instructions per cycle 3.98, cycles per 4-character hex string:  5.526, instructions per 4-character hex string: 22.000
min:  5526182 cycles, 22000020 instructions, 	       2 branch mis.,   138997 cache ref.,       87 cache mis.
avg: 5529890.1 cycles, 22000020.5 instructions, 	     2.2 branch mis., 139960.4 cache ref.,    210.5 cache mis.
sum = 1638571282800
lookup (alt):
instructions per cycle 3.98, cycles per 4-character hex string:  5.531, instructions per 4-character hex string: 22.000
min:  5531255 cycles, 22000020 instructions, 	       3 branch mis.,   138477 cache ref.,       83 cache mis.
avg: 5537938.3 cycles, 22000020.5 instructions, 	     3.8 branch mis., 139851.3 cache ref.,    188.6 cache mis.
sum = 1638571282800
math:
instructions per cycle 3.52, cycles per 4-character hex string:  10.793, instructions per 4-character hex string: 38.000
min: 10792516 cycles, 38000020 instructions, 	       3 branch mis.,   140129 cache ref.,       50 cache mis.
avg: 10899079.1 cycles, 38000020.9 instructions, 	     3.8 branch mis., 140379.6 cache ref.,    190.4 cache mis.
sum = 1638571282800
lookup4x32:
instructions per cycle 3.19, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 16.000
min:  5012734 cycles, 16000020 instructions, 	       2 branch mis.,   140097 cache ref.,      186 cache mis.
avg: 5013307.7 cycles, 16000020.4 instructions, 	     3.6 branch mis., 140293.1 cache ref.,    219.0 cache mis.
sum = 1638571282800
mayeut:
instructions per cycle 3.19, cycles per 4-character hex string:  5.012, instructions per 4-character hex string: 16.000
min:  5012343 cycles, 16000020 instructions, 	       2 branch mis.,   139624 cache ref.,      146 cache mis.
avg: 5013119.7 cycles, 16000020.4 instructions, 	     2.6 branch mis., 140116.6 cache ref.,    191.1 cache mis.
sum = 1638571282800
mula:
instructions per cycle 3.59, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 18.000
min:  5012700 cycles, 18000020 instructions, 	       2 branch mis.,   139318 cache ref.,       86 cache mis.
avg: 5013466.5 cycles, 18000020.4 instructions, 	     2.9 branch mis., 139764.9 cache ref.,    152.0 cache mis.
sum = 1638571282800
lee:
instructions per cycle 3.83, cycles per 4-character hex string:  6.009, instructions per 4-character hex string: 23.000
min:  6008959 cycles, 23000020 instructions, 	       2 branch mis.,   139634 cache ref.,      111 cache mis.
avg: 6009954.9 cycles, 23000020.5 instructions, 	     2.5 branch mis., 140157.2 cache ref.,    198.4 cache mis.
sum = 1638571282800
aqrit:
instructions per cycle 3.72, cycles per 4-character hex string:  7.251, instructions per 4-character hex string: 27.000
min:  7250647 cycles, 27000020 instructions, 	       2 branch mis.,   139608 cache ref.,      125 cache mis.
avg: 7252564.4 cycles, 27000020.6 instructions, 	     2.5 branch mis., 140006.1 cache ref.,    195.1 cache mis.
sum = 1638571282800
SSE (uint64_t):
instructions per cycle 3.28, cycles per 4-character hex string:  10.979, instructions per 4-character hex string: 36.000
min: 10978962 cycles, 36000020 instructions, 	       3 branch mis.,   139291 cache ref.,      165 cache mis.
avg: 10995976.5 cycles, 36000020.8 instructions, 	     3.8 branch mis., 139655.7 cache ref.,    224.9 cache mis.
sum = 107317154237389250
SSE ver2 (uint64_t):
instructions per cycle 3.69, cycles per 4-character hex string:  5.687, instructions per 4-character hex string: 21.000
min:  5687259 cycles, 21000020 instructions, 	       2 branch mis.,   137465 cache ref.,       58 cache mis.
avg: 5688786.0 cycles, 21000020.5 instructions, 	     3.4 branch mis., 137688.9 cache ref.,    177.2 cache mis.
sum = 107386858749007238
big lookup:
instructions per cycle 2.60, cycles per 4-character hex string:  5.004, instructions per 4-character hex string: 13.000
min:  5004127 cycles, 13000020 instructions, 	       2 branch mis.,   141852 cache ref.,       85 cache mis.
avg: 5004845.7 cycles, 13000020.4 instructions, 	     3.2 branch mis., 142040.9 cache ref.,    143.1 cache mis.
sum = 1638571282800
frozen_map:
instructions per cycle 1.03, cycles per 4-character hex string:  108.257, instructions per 4-character hex string: 112.000
min: 108257046 cycles, 111999557 instructions, 	 2000979 branch mis.,   137345 cache ref.,      244 cache mis.
avg: 108363264.7 cycles, 111999557.3 instructions, 	2002994.6 branch mis., 137884.4 cache ref.,    546.9 cache mis.
sum = 1638571282800
unfrozen_frozen_map:
instructions per cycle 1.57, cycles per 4-character hex string:  91.431, instructions per 4-character hex string: 143.513
min: 91430537 cycles, 143512608 instructions, 	 1336177 branch mis.,   139539 cache ref.,      127 cache mis.
avg: 91924026.1 cycles, 143512609.3 instructions, 	1348780.9 branch mis., 140127.3 cache ref.,   4725.5 cache mis.
sum = 1638571282800
```
