# instructive on RoaringBitmap

- `src/AndCardinality.java`: the example in the post, `./andcardinality.sh` (results in `results_m4max_andcardinality_jdk2*.txt`).
- `src/Profile.java`: ten RoaringBitmap workloads modeled after the JMH benchmarks in the
  RoaringBitmap repository, profiled with [instructive](https://github.com/richardstartin/instructive)
  (commit b2b7f79), with its analysis rules applied to the hottest methods. `./run.sh [workload ...]`;
  `JAVA_OPTS=-Dfull=true` prints every instruction. On macOS, instructive needs JDK 25 or later.
- `src/BitCount.java`: counting the bits of 1024 words in several ways. `./bitcount.sh`.
- `src/chain.c`: the `ins`/`cnt`/`addv`/`umov` sequence C2 emits for `Long.bitCount` on AArch64,
  against `fmov`. `cc -O2 src/chain.c && ./a.out`.
- `lib/`: instructive jars (`./gradlew :agent:jvmJar :parse:jar :analyse:jar` in its checkout, JDK 25),
  RoaringBitmap 1.6.30 and its real datasets (built from RoaringBitmap master, commit 5268d717).

Results are from an Apple M4 Max, Corretto 25.0.1 and Homebrew OpenJDK 27.0.1.1.

Results on big4 (Intel Xeon Gold 6548N, taskset -c 8): `results_big4_andcardinality_jdk2*.txt`, Temurin 25.0.5 and 27.0.1.1.

Diagrams: `python3 diagrams.py`, then `rsvg-convert -z 2 X.svg -o X.png && cwebp -q 90 X.png -o X.webp`.
