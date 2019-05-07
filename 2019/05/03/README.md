```
$ make
$ ./bitmapdecoding
fast_decoder:
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 2.01, cycles per value set:  4.093, instructions per value set: 8.212, cycles per word: 24.955, instructions per word: 50.066
 cycles per input byte 3.12 instructions per input byte 6.26
min:   532101 cycles,  1067497 instructions, 	   12053 branch mis.,     6239 cache ref.,        0 cache mis.
avg: 546124.3 cycles, 1067509.6 instructions, 	 12298.7 branch mis.,  22085.9 cache ref.,      0.8 cache mis.

simdjson_decoder:
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 2.35, cycles per value set:  4.373, instructions per value set: 10.254, cycles per word: 26.659, instructions per word: 62.520
 cycles per input byte 3.33 instructions per input byte 7.81
min:   568415 cycles,  1333042 instructions, 	    7798 branch mis.,     6213 cache ref.,        0 cache mis.
avg: 583004.4 cycles, 1333054.7 instructions, 	  8088.5 branch mis.,  22330.9 cache ref.,     24.2 cache mis.

basic_decoder:
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 1.45, cycles per value set:  5.709, instructions per value set: 8.283, cycles per word: 34.807, instructions per word: 50.502
 cycles per input byte 4.35 instructions per input byte 6.31
min:   742160 cycles,  1076803 instructions, 	   20096 branch mis.,    23975 cache ref.,        0 cache mis.
avg: 745441.8 cycles, 1076803.1 instructions, 	 20223.4 branch mis.,  24174.7 cache ref.,     13.7 cache mis.

faster_decoder:
matches = 129996 words = 21322 1-bit density 9.526 %
bytes per index = 10.497
instructions per cycle 2.57, cycles per value set:  3.852, instructions per value set: 9.902, cycles per word: 23.488, instructions per word: 60.373
 cycles per input byte 2.94 instructions per input byte 7.55
min:   500803 cycles,  1287280 instructions, 	    8291 branch mis.,    24033 cache ref.,        0 cache mis.
avg: 501721.7 cycles, 1287280.1 instructions, 	  8360.0 branch mis.,  24242.4 cache ref.,     11.9 cache mis.
```
