```
$ make
$ ./bighash
$ ./bighash
N= 536870912 M = 536870912
buffered:
capacity of buffer slots = 131072 slot count = 2048
capacity of buffer slots = 131072 slot count = 2048
capacity of buffer slots = 131072 slot count = 2048
bogus
.0 0 203723863 266878757
instructions per cycle 0.80, cycles per value set:  45.107, instructions per value set: 36.002
min: 24216819307 cycles, 19328457594 instructions, 	   12087 branch mis., 2544825032 cache ref., 600559899 cache mis.
avg: 24391728142.7 cycles, 19328807914.7 instructions, 	 12251.7 branch mis., 2559357451.0 cache ref., 697061606.7 cache mis.
standard:
bogus
.0 0 203723863 266878757
instructions per cycle 0.23, cycles per value set:  57.203, instructions per value set: 13.000
min: 30710489568 cycles, 6979324217 instructions, 	       4 branch mis., 2786705358 cache ref., 1976423114 cache mis.
avg: 31006587630.0 cycles, 6979673763.0 instructions, 	     6.3 branch mis., 2787954071.0 cache ref., 1979561891.7 cache mis.
unrolled:
bogus
.0 0 203723863 266878757
instructions per cycle 0.26, cycles per value set:  57.003, instructions per value set: 14.750
min: 30603430034 cycles, 7918848327 instructions, 	       7 branch mis., 2809418353 cache ref., 1978530177 cache mis.
avg: 30906213011.7 cycles, 7919197886.7 instructions, 	     8.7 branch mis., 2810741285.3 cache ref., 1981889577.3 cache mis.

```
