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

GCC-8

``
fast_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.17, cycles per value set:  8.808, instructions per value set: 10.262, cycles per word: 53.702, instructions per word: 62.564
 cycles per input byte 0.84 instructions per input byte 0.98
min:  1145041 cycles,  1333980 instructions,       14116 branch mis.,   168759 cache ref.,     2668 cache mis.
avg: 1149696.5 cycles, 1333980.0 instructions,   14246.7 branch mis., 168905.0 cache ref.,   2670.9 cache mis.

simdjson_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.29, cycles per value set:  12.555, instructions per value set: 16.154, cycles per word: 76.544, instructions per word: 98.487
 cycles per input byte 1.20 instructions per input byte 1.54
min:  1632075 cycles,  2099946 instructions,        9757 branch mis.,   542084 cache ref.,     2675 cache mis.
avg: 1668175.3 cycles, 2099946.0 instructions,    9799.5 branch mis., 543010.1 cache ref.,   2677.8 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.07, cycles per value set:  8.516, instructions per value set: 9.119, cycles per word: 51.921, instructions per word: 55.599
 cycles per input byte 0.81 instructions per input byte 0.87
min:  1107068 cycles,  1185475 instructions,       20354 branch mis.,   161100 cache ref.,     2668 cache mis.
avg: 1109260.0 cycles, 1185475.0 instructions,   20468.0 branch mis., 161285.9 cache ref.,   2670.5 cache mis.

faster_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.40, cycles per value set:  10.980, instructions per value set: 15.363, cycles per word: 66.941, instructions per word: 93.665
 cycles per input byte 1.05 instructions per input byte 1.46
min:  1427308 cycles,  1997117 instructions,        9241 branch mis.,   486966 cache ref.,     2671 cache mis.
avg: 1442093.6 cycles, 1997117.0 instructions,    9279.1 branch mis., 487102.2 cache ref.,   2672.9 cache mis.

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
