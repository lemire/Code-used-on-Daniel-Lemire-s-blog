```
$ ./hexparse 
N= 1000000 
empty function (overhead):
bogus
.instructions per cycle 0.43, cycles per 4-character hex string:  0.000, instructions per 4-character hex string: 0.000 
min:       54 cycles,       23 instructions, 	       0 branch mis.,        0 cache ref.,        0 cache mis.
avg:     57.2 cycles,     23.0 instructions, 	     0.1 branch mis.,      0.3 cache ref.,      0.0 cache mis.
sum = 0 
lookup:
instructions per cycle 3.98, cycles per 4-character hex string:  5.528, instructions per 4-character hex string: 22.000 
min:  5528411 cycles, 22000018 instructions, 	       2 branch mis.,    32897 cache ref.,        0 cache mis.
avg: 5530701.3 cycles, 22000018.4 instructions, 	     2.1 branch mis.,  33182.6 cache ref.,      0.3 cache mis.
sum = 1637654523550 
lookup (inline):
instructions per cycle 4.07, cycles per 4-character hex string:  3.605, instructions per 4-character hex string: 14.667 
min:  3604555 cycles, 14666699 instructions, 	       2 branch mis.,    33631 cache ref.,        0 cache mis.
avg: 3609277.7 cycles, 14666699.3 instructions, 	     3.7 branch mis.,  33937.6 cache ref.,      0.0 cache mis.
sum = 1637654523550 
lookup (alt):
instructions per cycle 3.66, cycles per 4-character hex string:  6.018, instructions per 4-character hex string: 22.000 
min:  6018499 cycles, 22000018 instructions, 	       3 branch mis.,    32025 cache ref.,        0 cache mis.
avg: 6020256.4 cycles, 22000018.5 instructions, 	     3.6 branch mis.,  32273.5 cache ref.,      0.2 cache mis.
sum = 1637654523550 
math:
instructions per cycle 3.81, cycles per 4-character hex string:  9.971, instructions per 4-character hex string: 38.000 
min:  9971044 cycles, 38000018 instructions, 	       2 branch mis.,    33449 cache ref.,        0 cache mis.
avg: 10038290.3 cycles, 38000018.8 instructions, 	     3.2 branch mis.,  33787.4 cache ref.,      0.2 cache mis.
sum = 1637654523550 
mula:
instructions per cycle 3.56, cycles per 4-character hex string:  5.058, instructions per 4-character hex string: 18.000 
min:  5058314 cycles, 18000018 instructions, 	       2 branch mis.,    33695 cache ref.,        0 cache mis.
avg: 5059542.6 cycles, 18000018.4 instructions, 	     3.4 branch mis.,  34081.2 cache ref.,      0.0 cache mis.
sum = 1637654523550 
lee:
instructions per cycle 3.82, cycles per 4-character hex string:  6.023, instructions per 4-character hex string: 23.000 
min:  6022584 cycles, 23000018 instructions, 	       2 branch mis.,    33994 cache ref.,        0 cache mis.
avg: 6043915.5 cycles, 23000018.5 instructions, 	     2.8 branch mis.,  34374.7 cache ref.,      0.0 cache mis.
sum = 1637654523550 
aqrit:
instructions per cycle 3.83, cycles per 4-character hex string:  7.048, instructions per 4-character hex string: 27.000 
min:  7047617 cycles, 27000018 instructions, 	       2 branch mis.,    34098 cache ref.,        0 cache mis.
avg: 7060089.7 cycles, 27000018.5 instructions, 	     2.1 branch mis.,  34317.3 cache ref.,      0.0 cache mis.
sum = 1637654523550 
SSE (uint64_t):
instructions per cycle 3.72, cycles per 4-character hex string:  8.606, instructions per 4-character hex string: 32.000 
min:  8606344 cycles, 32000018 instructions, 	       2 branch mis.,    34622 cache ref.,        0 cache mis.
avg: 8753207.6 cycles, 32000018.7 instructions, 	     3.0 branch mis.,  34866.7 cache ref.,      0.5 cache mis.
sum = 107326502502588700 
big lookup:
instructions per cycle 2.55, cycles per 4-character hex string:  5.092, instructions per 4-character hex string: 13.000 
min:  5092020 cycles, 13000018 instructions, 	       2 branch mis.,    33892 cache ref.,        0 cache mis.
avg: 5093569.4 cycles, 13000018.4 instructions, 	     3.4 branch mis.,  34188.9 cache ref.,      1.4 cache mis.
sum = 1637654523550 
big lookup (inline):
instructions per cycle 4.17, cycles per 4-character hex string:  2.279, instructions per 4-character hex string: 9.500 
min:  2278621 cycles,  9500028 instructions, 	       2 branch mis.,    33902 cache ref.,        0 cache mis.
avg: 2283736.7 cycles, 9500028.2 instructions, 	     3.1 branch mis.,  34258.1 cache ref.,      0.6 cache mis.
sum = 1637654523550 
```
