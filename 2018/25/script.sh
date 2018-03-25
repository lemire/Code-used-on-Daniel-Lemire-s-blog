cc -o cachemissbench cachemissbench.c  -mbmi2 -mavx2 -O3  && perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations ./cachemissbench
