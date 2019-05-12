CLANG8:
``
fast_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.14, cycles per value set:  8.659, instructions per value set: 9.901, cycles per word: 52.794, instructions per word: 60.365
 cycles per input byte 0.82 instructions per input byte 0.94
min:  1125675 cycles,  1287100 instructions,       14082 branch mis.,   168647 cache ref.,     2667 cache mis.
avg: 1130474.7 cycles, 1287100.0 instructions,   14261.3 branch mis., 168786.7 cache ref.,   2669.3 cache mis.

simdjson_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.37, cycles per value set:  9.618, instructions per value set: 13.224, cycles per word: 58.637, instructions per word: 80.625
 cycles per input byte 0.92 instructions per input byte 1.26
min:  1250256 cycles,  1719083 instructions,        9115 branch mis.,   230735 cache ref.,     2668 cache mis.
avg: 1252167.6 cycles, 1719083.0 instructions,    9181.2 branch mis., 230791.9 cache ref.,   2669.3 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.01, cycles per value set:  11.780, instructions per value set: 11.882, cycles per word: 71.821, instructions per word: 72.440
 cycles per input byte 1.12 instructions per input byte 1.13
min:  1531360 cycles,  1544557 instructions,       21621 branch mis.,   161523 cache ref.,     2667 cache mis.
avg: 1533487.6 cycles, 1544557.0 instructions,   21761.9 branch mis., 161700.8 cache ref.,   2668.5 cache mis.

faster_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.30, cycles per value set:  11.628, instructions per value set: 15.112, cycles per word: 70.892, instructions per word: 92.135
 cycles per input byte 1.11 instructions per input byte 1.44
min:  1511550 cycles,  1964493 instructions,        9327 branch mis.,   484279 cache ref.,     2670 cache mis.
avg: 1565331.1 cycles, 1964493.0 instructions,    9391.9 branch mis., 484726.4 cache ref.,   2672.0 cache mis.

```



GCC-9

```
fast_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.16, cycles per value set:  8.726, instructions per value set: 10.098, cycles per word: 53.202, instructions per word: 61.564
 cycles per input byte 0.83 instructions per input byte 0.96
min:  1134371 cycles,  1312658 instructions,       13993 branch mis.,   168253 cache ref.,     2668 cache mis.
avg: 1140013.2 cycles, 1312658.0 instructions,   14226.2 branch mis., 168440.8 cache ref.,   2670.9 cache mis.

simdjson_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.37, cycles per value set:  9.739, instructions per value set: 13.388, cycles per word: 59.374, instructions per word: 81.624
 cycles per input byte 0.93 instructions per input byte 1.28
min:  1265973 cycles,  1740391 instructions,        9139 branch mis.,   230798 cache ref.,     2671 cache mis.
avg: 1267620.6 cycles, 1740391.0 instructions,    9204.2 branch mis., 230878.0 cache ref.,   2676.6 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 0.85, cycles per value set:  11.791, instructions per value set: 10.065, cycles per word: 71.888, instructions per word: 61.362
 cycles per input byte 1.12 instructions per input byte 0.96
min:  1532787 cycles,  1308360 instructions,       19386 branch mis.,   152615 cache ref.,     2670 cache mis.
avg: 1536000.8 cycles, 1308360.0 instructions,   19542.8 branch mis., 152715.8 cache ref.,   2673.3 cache mis.

faster_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.57, cycles per value set:  8.275, instructions per value set: 12.969, cycles per word: 50.451, instructions per word: 79.069
 cycles per input byte 0.79 instructions per input byte 1.24
min:  1075714 cycles,  1685900 instructions,        9238 branch mis.,   230859 cache ref.,     2670 cache mis.
avg: 1076417.8 cycles, 1685900.0 instructions,    9274.9 branch mis., 230899.5 cache ref.,   2670.8 cache mis.

```
