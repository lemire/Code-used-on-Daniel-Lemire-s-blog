# simdjson 4.0.0 vs 5.0.0

Benchmarks for the blog post in `post.md`.

- `benchmark/setup.sh`: clones simdjson twice (v4.0.0 and the 5.0.0 release candidate,
  branch `v5_candidate`, commit 7740055f), builds `bench_ondemand` and `parse` with CMake
  (Release) and `serialize.cpp` against each single header. Uses GCC 16.1.
- `benchmark/run.sh`: three interleaved rounds, pinned to CPU 2
  (DOM parse via `benchmark/dom/parse`, DOM `minify`, and `bench_ondemand`
  without the threaded and accessor benchmarks).
- `benchmark/aggregate.py`: best-of-rounds tables, see `results_big4_gcc16/summary.md`.

Machine: Intel Xeon Gold 6548N (Emerald Rapids), simdjson uses its icelake (AVX-512) kernel.
