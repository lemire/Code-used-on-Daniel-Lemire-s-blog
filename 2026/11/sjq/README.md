# Running jq filters at 20 GB/s

Benchmark for the post on sjq (https://github.com/simdjson/sjq), a jq-compatible command-line tool built on simdjson.

- `make_data.py`: builds the inputs from simdjson's twitter.json: `tweets.ndjson` (1 GB, one tweet per line) and `timeline.json` (one 840 MB document). It is a copy of `bench/make_data.py` in the sjq repository, and reads `tests/data/twitter.json` there.
- `run_bench.sh`: times jq, sjq (one thread, the default) and `sjq -P` (up to 32 threads) on nine workloads with hyperfine (3 runs of jq, 10 of sjq, one warm-up each).
- `results/`: hyperfine exports, on a server with 2 x Intel Xeon Gold 6548N (64 cores), Linux, GCC 16.2, jq 1.8.2 (official static binary), hyperfine 2.0, sjq 0.4.0. Every run on one thread (jq, sjq without `-P`, and `sjq -P` on the single document) is pinned to one socket (`PIN_DOCUMENT="taskset -c 1-127:2"`).
- `plot.py`, `featured.py`: the figures (need matplotlib, Pillow and cwebp).

```
git clone --branch v0.4.0 https://github.com/simdjson/sjq
cmake -S sjq -B sjq/build && cmake --build sjq/build
python3 sjq/bench/make_data.py data --mb 1024
PIN_DOCUMENT="taskset -c 1-127:2" bash run_bench.sh data results /path/to/jq sjq/build/sjq /path/to/hyperfine
python3 plot.py
```
