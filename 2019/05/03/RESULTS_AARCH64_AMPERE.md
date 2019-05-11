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
