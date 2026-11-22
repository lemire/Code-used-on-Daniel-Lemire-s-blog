# Streaming compressed NDJSON through simdjson

The code is at https://github.com/simdjson/simdjson_compressed_demo (commit 7d5b77f).

- `results_big4.txt`: output of `./bench.sh` on big4 (2 x Xeon Gold 6548N, GCC 14.3.1).
  The `gzip file, threads=8` and `threads=64` rows were run separately with
  `./build/gz_stream_demo --threads N data.ndjson.gz 262144` (median of 5).
- `plot.py`: makes `uncompressed.png` and `formats.png` from `results_big4.txt`.

```sh
git clone https://github.com/simdjson/simdjson_compressed_demo
cd simdjson_compressed_demo
cmake -B build && cmake --build build
./bench.sh
```
