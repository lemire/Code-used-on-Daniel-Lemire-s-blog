```
$ make
$ ./hexparse
N= 1000000
empty function (overhead):
bogus
.instructions per cycle 2.50, cycles per 4-character hex string:  0.500, instructions per 4-character hex string: 1.250
min:   500181 cycles,  1250047 instructions, 	       2 branch mis.,        0 cache ref.,        0 cache mis.
avg: 500322.1 cycles, 1250047.0 instructions, 	     2.2 branch mis.,      1.1 cache ref.,      0.5 cache mis.
sum = 0
lookup:
instructions per cycle 3.98, cycles per 4-character hex string:  5.529, instructions per 4-character hex string: 22.000
min:  5528537 cycles, 22000020 instructions, 	       2 branch mis.,   136754 cache ref.,      247 cache mis.
avg: 5540678.1 cycles, 22000020.4 instructions, 	     2.1 branch mis., 139911.9 cache ref.,    376.8 cache mis.
sum = 1638571282800
lookup (inline):
instructions per cycle 4.21, cycles per 4-character hex string:  4.756, instructions per 4-character hex string: 20.000
min:  4755894 cycles, 20000020 instructions, 	       3 branch mis.,   139535 cache ref.,      239 cache mis.
avg: 4760440.2 cycles, 20000020.4 instructions, 	     3.7 branch mis., 139824.3 cache ref.,    366.3 cache mis.
sum = 1638571282800
lookup (alt):
instructions per cycle 3.66, cycles per 4-character hex string:  6.007, instructions per 4-character hex string: 22.000
min:  6006628 cycles, 22000020 instructions, 	       2 branch mis.,   137005 cache ref.,      260 cache mis.
avg: 6008005.2 cycles, 22000020.4 instructions, 	     3.4 branch mis., 139404.5 cache ref.,    342.4 cache mis.
sum = 1638571282800
math:
instructions per cycle 3.52, cycles per 4-character hex string:  10.793, instructions per 4-character hex string: 38.000
min: 10793111 cycles, 38000019 instructions, 	       2 branch mis.,   140186 cache ref.,      247 cache mis.
avg: 10918790.3 cycles, 38000019.9 instructions, 	     3.2 branch mis., 140388.0 cache ref.,    312.3 cache mis.
sum = 1638571282800
lookup4x32:
instructions per cycle 2.67, cycles per 4-character hex string:  6.001, instructions per 4-character hex string: 16.000
min:  6000685 cycles, 16000020 instructions, 	       2 branch mis.,   138256 cache ref.,      245 cache mis.
avg: 6002206.3 cycles, 16000020.4 instructions, 	     3.1 branch mis., 139913.5 cache ref.,    317.9 cache mis.
sum = 1637505178050
mayeut:
instructions per cycle 3.19, cycles per 4-character hex string:  5.013, instructions per 4-character hex string: 16.000
min:  5012956 cycles, 16000020 instructions, 	       2 branch mis.,   140299 cache ref.,      240 cache mis.
avg: 5013502.6 cycles, 16000020.4 instructions, 	     3.4 branch mis., 140675.0 cache ref.,    301.7 cache mis.
sum = 49080323397187350
mula:
instructions per cycle 3.59, cycles per 4-character hex string:  5.015, instructions per 4-character hex string: 18.000
min:  5014932 cycles, 18000020 instructions, 	       3 branch mis.,   139797 cache ref.,      244 cache mis.
avg: 5015706.7 cycles, 18000020.4 instructions, 	     3.8 branch mis., 140348.6 cache ref.,    301.1 cache mis.
sum = 1638571282800
lee:
instructions per cycle 3.83, cycles per 4-character hex string:  6.009, instructions per 4-character hex string: 23.000
min:  6008942 cycles, 23000020 instructions, 	       2 branch mis.,   139824 cache ref.,      244 cache mis.
avg: 6025340.2 cycles, 23000020.5 instructions, 	     3.4 branch mis., 140292.2 cache ref.,    333.0 cache mis.
sum = 1638571282800
aqrit:
instructions per cycle 3.72, cycles per 4-character hex string:  7.250, instructions per 4-character hex string: 27.000
min:  7249949 cycles, 27000020 instructions, 	       2 branch mis.,   140018 cache ref.,      248 cache mis.
avg: 7252317.2 cycles, 27000020.5 instructions, 	     3.3 branch mis., 140300.0 cache ref.,    294.6 cache mis.
sum = 1638571282800
SSE (uint64_t):
instructions per cycle 3.45, cycles per 4-character hex string:  9.271, instructions per 4-character hex string: 32.000
min:  9270882 cycles, 32000020 instructions, 	       2 branch mis.,   139337 cache ref.,      244 cache mis.
avg: 9314188.6 cycles, 32000020.7 instructions, 	     3.6 branch mis., 140253.3 cache ref.,    301.0 cache mis.
sum = 107317154237389250
SSE ver2 (uint64_t):
instructions per cycle 3.49, cycles per 4-character hex string:  6.019, instructions per 4-character hex string: 21.000
min:  6018871 cycles, 21000020 instructions, 	       3 branch mis.,   140312 cache ref.,      245 cache mis.
avg: 6019679.6 cycles, 21000020.4 instructions, 	     3.7 branch mis., 140478.4 cache ref.,    305.2 cache mis.
sum = 107386858749007238
big lookup:
instructions per cycle 2.60, cycles per 4-character hex string:  5.007, instructions per 4-character hex string: 13.000
min:  5006802 cycles, 13000020 instructions, 	       3 branch mis.,   141761 cache ref.,      289 cache mis.
avg: 5007456.4 cycles, 13000020.4 instructions, 	     3.7 branch mis., 141988.3 cache ref.,    424.1 cache mis.
sum = 1638571282800
big lookup (inline):
instructions per cycle 1.26, cycles per 4-character hex string:  2.083, instructions per 4-character hex string: 2.625
min:  2083302 cycles,  2625033 instructions, 	       3 branch mis.,   140767 cache ref.,      283 cache mis.
avg: 2085367.9 cycles, 2625033.2 instructions, 	     3.8 branch mis., 141054.2 cache ref.,    392.7 cache mis.
sum = 1638571282800
frozen_map:
instructions per cycle 1.33, cycles per 4-character hex string:  101.485, instructions per 4-character hex string: 135.001
min: 101484634 cycles, 135000618 instructions, 	 1998509 branch mis.,   136455 cache ref.,      253 cache mis.
avg: 101621526.5 cycles, 135000618.6 instructions, 	1999586.1 branch mis., 137022.3 cache ref.,    371.7 cache mis.
sum = 1638571282800
unfrozen_frozen_map:
instructions per cycle 1.55, cycles per 4-character hex string:  92.834, instructions per 4-character hex string: 143.513
min: 92834317 cycles, 143512607 instructions, 	 1357057 branch mis.,   139436 cache ref.,      267 cache mis.
avg: 92903941.6 cycles, 143512608.1 instructions, 	1358706.8 branch mis., 139958.2 cache ref.,    401.7 cache mis.
sum = 1638571282800
```
