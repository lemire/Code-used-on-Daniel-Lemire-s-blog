GCC9

```
simdjson_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 1.47, cycles per value set:  7.381, instructions per value set: 10.872, cycles per word: 44.998, instructions per word: 66.287
 cycles per input byte 0.70 instructions per input byte 1.04
min:   959444 cycles,  1413371 instructions, 	    9201 branch mis.,   230902 cache ref.,     2674 cache mis.
avg: 960839.4 cycles, 1413371.0 instructions, 	  9278.4 branch mis., 230965.4 cache ref.,   2676.0 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 0.85, cycles per value set:  11.806, instructions per value set: 10.065, cycles per word: 71.978, instructions per word: 61.362
 cycles per input byte 1.12 instructions per input byte 0.96
min:  1534717 cycles,  1308360 instructions, 	   19474 branch mis.,   152538 cache ref.,     2669 cache mis.
avg: 1537000.0 cycles, 1308360.0 instructions, 	 19596.7 branch mis., 152639.5 cache ref.,   2671.7 cache mis.
```


GCC8

```
simdjson_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 1.51, cycles per value set:  7.363, instructions per value set: 11.143, cycles per word: 44.889, instructions per word: 67.934
 cycles per input byte 0.70 instructions per input byte 1.06
min:   957121 cycles,  1448481 instructions, 	    9759 branch mis.,   230851 cache ref.,     2670 cache mis.
avg: 958733.8 cycles, 1448481.0 instructions, 	  9825.1 branch mis., 230910.8 cache ref.,   2670.4 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 1.07, cycles per value set:  8.517, instructions per value set: 9.119, cycles per word: 51.928, instructions per word: 55.599
 cycles per input byte 0.81 instructions per input byte 0.87
min:  1107215 cycles,  1185475 instructions, 	   20385 branch mis.,   161099 cache ref.,     2669 cache mis.
avg: 1109942.0 cycles, 1185475.0 instructions, 	 20503.6 branch mis., 161313.2 cache ref.,   2670.3 cache mis.
```

CLANG8

```
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 1.26, cycles per value set:  8.287, instructions per value set: 10.467, cycles per word: 50.525, instructions per word: 63.818
 cycles per input byte 0.79 instructions per input byte 1.00
min:  1077302 cycles,  1360720 instructions, 	    9341 branch mis.,   231002 cache ref.,     2669 cache mis.
avg: 1078526.0 cycles, 1360720.0 instructions, 	  9386.6 branch mis., 231025.2 cache ref.,   2671.7 cache mis.

basic_decoder:
Tests passed.
matches = 129996 words = 21322 1-bit density 9.526 % 1-bit per word 6.097
bytes per index = 10.497
instructions per cycle 1.03, cycles per value set:  11.559, instructions per value set: 11.882, cycles per word: 70.472, instructions per word: 72.440
 cycles per input byte 1.10 instructions per input byte 1.13
min:  1502613 cycles,  1544557 instructions, 	   20040 branch mis.,   160084 cache ref.,     2667 cache mis.
avg: 1504803.0 cycles, 1544557.0 instructions, 	 20151.7 branch mis., 160194.9 cache ref.,   2670.3 cache mis.
```
