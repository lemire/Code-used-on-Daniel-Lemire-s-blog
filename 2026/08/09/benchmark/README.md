# Profile-guided optimization of JSON parsing in Go

Measured on a dual Intel Xeon Gold 6548N, Rocky Linux 10.2, Go 1.26.3,
pinned to one core.

`main.go` parses JSON documents into `interface{}` with `encoding/json` and
reports the fastest of several timed rounds (median of five such measurements).

```sh
DATA=/path/to/json ./matrix.sh    # baseline, then a PGO build per document,
                                  # then one from the three profiles merged
```

`matrix.sh` does the whole experiment in about four minutes:

1. `go build` for the baseline.
2. For each document: collect a CPU profile with `runtime/pprof` while parsing
   it, then `go build -pgo=<profile>`.
3. Merge the three profiles with `go tool pprof -proto` and build once more.
4. Benchmark every binary against all three documents.

`run.sh` is the single-document version. `plot_go_pgo.py` draws the figure
(needs matplotlib).

The documents are `twitter.json`, `canada.json` and `citm_catalog.json` from
the simdjson corpus.
