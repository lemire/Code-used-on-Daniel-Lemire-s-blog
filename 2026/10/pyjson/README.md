# Parsing JSON at tens of gigabytes per second... in Python

Benchmark for the blog post in `post.md`. The input is GitHub's public event
log ([GH Archive](https://www.gharchive.org/)): one JSON event per line, eight
hours of 2024-03-13 (7.6 GB and 2.2 million events once decompressed). The task: count the events by
type, and count the pull requests opened per language of the target
repository. Every method must produce exactly the same counts.

```sh
./run.sh                      # needs uv and curl, Linux; writes results/<host>/
HOURS=4 WORKERS=16 ./run.sh   # less data, fewer workers
```

`run.sh` downloads the hours, creates two Python 3.14 environments with uv
(the default build with the GIL, and the free-threaded build `3.14t`), runs
`bench.py` in both, and draws `one_thread.png` and `many_cores.png` with
`plot.py`.

The methods (see `bench.py`):

- `json`, `orjson`: parse each line into dictionaries, one thread;
- `msgspec`: decode the lines into typed structs that declare only the fields
  used (`decode_lines`), one thread;
- `fastsimdjson`, `fastsimdjson-lazy`: `fastsimdjson.map_many(fn, data,
  format="lines")` with dictionaries, or with lazy views (`lazy=True`);
- `processes-orjson`, `processes-msgspec`: a pool of forked processes
  (`multiprocessing`) that inherit the input and receive line ranges;
- `threads-json`, `threads-msgspec`: a thread pool on free-threaded Python;
- `threads-fastsimdjson`, `threads-fastsimdjson-lazy`: `map_many(...,
  threads=64)` on free-threaded Python.

The input is decompressed in memory before timing. We report the best of seven
runs (one run for the slow one-thread methods). The results of the post are in
`results/big4/` (two Intel Xeon Gold 6548N, 64 cores), with
`CPUS=0-63 ./run.sh`: one CPU per core.
