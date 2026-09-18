# SVE2 helps simdjson on AWS Graviton processors

Benchmark of simdjson PR #2863 (https://github.com/simdjson/simdjson/pull/2863) on AWS Graviton 3/4/5.

- `post.md`: the blog post.
- `report.md`: full report with per-file tables.
- `results/g3`, `results/g4`, `results/g5`: raw `benchmark/dom/parse -t` output (TSV), one file per build/tree/run.
  Columns: file, (4 empty cycle columns), whole-parse GB/s, stage-1 GB/s, stage-2 GB/s.
- `results/analyze.py`: regenerates `report.md` from the TSVs: `python3 results/analyze.py results report.md`.

Protocol: base = merge-base `9572dd09`, PR = `265ea481`; GCC 15.2 / clang 21.1, default flags and `-mcpu=native`;
`parse -t -n 300` over the 22 simdjson-data files, `taskset -c 3`, 3 interleaved runs, best kept.
`pr-nosve` and `pr-nomixed4` are the PR with, respectively, the SVE2 `match` path and the Mixed4 writer disabled.
The blog post uses the `base` vs `bridge` comparison in `predicate/results/{g4,g5}` (the `match` classifier with the
intrinsics-only predicate extraction, i.e. what simdjson ships; `-mcpu=native`, 3 runs each).
`report.md` covers the full PR #2863 (inline-asm extraction plus the Mixed4 writer) on all three machines;
`predicate/README.md` documents the alternatives to the inline assembly that were tried and measured.
