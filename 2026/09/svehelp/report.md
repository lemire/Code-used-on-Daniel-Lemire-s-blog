# simdjson PR #2863 on AWS Graviton 3 / 4 / 5

**PR:** [simdjson/simdjson#2863](https://github.com/simdjson/simdjson/pull/2863) — *perf(arm64): optimize Stage 1 matching and index writing* (head `265ea481`), against its merge-base `9572dd09` (master).

**Date:** 2026-09-17

## Findings

1. **The PR is a net win on every Graviton generation with SVE, in every build configuration.** Whole-parse geomean ranges from +0.3% (Graviton 3, `-mcpu=native`) to +7.1% (Graviton 4, clang `-mcpu=native`). Stage 2 is untouched (all within ±0.5%).
2. **The big gains need SVE2 *and* `-mcpu=native`.** With the SVE2 `svmatch` classifier active, stage 1 speeds up by +8% to +18% (geomean) on Graviton 4 and 5, with individual files up to +29% (mesh.pretty on Graviton 4/clang). Graviton 4 benefits more than Graviton 5.
3. **Default builds only get the Mixed4 writer.** Because the SVE2 path is a compile-time opt-in, a stock build (no `-mcpu`) sees +6% to +9% stage 1 with gcc but only +2% with clang, i.e. +0.7% to +3.5% end to end. The gcc number is large mostly because gcc's codegen of the generic writer without `-mcpu` is poor; under `-mcpu=native` the Mixed4 writer alone is worth just +1.3% to +2.4% on all three chips.
4. **Graviton 3 (SVE, no SVE2) gets only the Mixed4 writer:** +1.4% to +6.4% stage 1, +0.3% to +2.1% whole parse. It also shows the only consistent per-file regression: update-center stage 1 drops 5–10% (clang: 4.06 → 3.67 GB/s, reproducible across runs). All other files are neutral or faster.
5. **Two changes, roughly additive.** SVE2 match contributes 3–4× more than the Mixed4 writer wherever both are active.
6. **Correctness:** `basictests`, `parse_many_test`, `document_stream_tests` and `minify_tests` pass on the gcc and clang `-mcpu=native` PR builds on all three machines (i.e. with the SVE2 path exercised on Graviton 4 and 5).

**Recommendation:** merge; the code is safe (pure NEON fallback when SVE2 is absent at compile time). To make the SVE2 gain reach default builds, a runtime-dispatched `arm64` SVE2 kernel would be the natural follow-up, since none of the +8–18% reaches users who don't pass `-mcpu`. The update-center regression on Graviton 3 is small and isolated but worth a look at the Mixed4 writer's group-of-two loop for dense masks.

## What the PR changes

1. **Mixed4 structural-index writer** (`bit_indexer::write` in `src/arm64.cpp`): writes the first 4 indexes, then groups of 2 up to 24, then scalar. Always compiled in on arm64; plain NEON/scalar code, no SVE needed.
2. **SVE2 `svmatch` operator classifier** (`json_character_block::classify`): replaces the NEON `tbl` lookup for the operator class (`,:[]{}`) with `svmatch_u8` on 16-byte NEON-SVE-bridge vectors, and extracts the predicate with `str p`. Whitespace still uses NEON `tbl`. **Compile-time gated** on `__ARM_FEATURE_SVE2 && __ARM_NEON_SVE_BRIDGE`, i.e. only active with `-mcpu=native` / `-march=armv9-a+sve2`; there is no runtime dispatch. `MATCH` is an SVE2 instruction, so this path cannot be used on Graviton 3 (SVE only).

## Method

- One instance per processor type, us-east-1, Ubuntu 26.04 arm64, gcc 15.2 and clang 21.1 from apt.
- `benchmark/dom/parse -t -n 300 <22 files>` (the simdjson-data corpus, ≈24 MB total), `taskset -c 3`, Release build, `SIMDJSON_DEVELOPER_MODE=ON`, static lib.
- `parse -t` reports the *best* of 300 iterations per file; each binary was run 3 times interleaved with its counterpart (base, PR, base, PR, ...) and the best of the 3 is used. Run-to-run spread of stage 1 (geomean over files of max/min across the 3 runs) is reported as *noise* below.
- Four builds per tree: gcc-15 / clang-21 × default flags / `-mcpu=native`. Under `-mcpu=native` two extra PR variants isolate the two changes: **Mixed4 only** (SVE2 macro suppressed) and **SVE2 match only** (Mixed4 macro suppressed).
- I checked in every binary that the SVE `match` instruction appears only where expected (16 copies in PR `-mcpu=native` builds on G4/G5; 0 everywhere else).
- Ratios are **PR / base** throughput (GB/s); >1 means the PR is faster. Geomean over the 22 files.

## Headline: geomean PR / base

| Machine | Build | Stage 1 | Stage 2 | Whole parse | Noise | S1 worst file | S1 best file |
|---|---|---|---|---|---|---|---|
| Graviton 3 | gcc-15, default flags | **1.064** | 0.993 | 1.021 | 0.3% | 0.999 (semanticscholar-corpus) | 1.130 (google_maps_api_compact_response) |
| Graviton 3 | clang-21, default flags | **1.017** | 1.002 | 1.007 | 0.3% | 0.949 (update-center) | 1.055 (mesh.pretty) |
| Graviton 3 | gcc-15, -mcpu=native | **1.014** | 0.995 | 1.003 | 0.7% | 0.946 (update-center) | 1.103 (google_maps_api_response) |
| Graviton 3 | clang-21, -mcpu=native | **1.016** | 0.999 | 1.005 | 0.2% | 0.912 (update-center) | 1.067 (mesh.pretty) |
| Graviton 4 | gcc-15, default flags | **1.086** | 1.000 | 1.034 | 0.2% | 0.994 (semanticscholar-corpus) | 1.183 (instruments) |
| Graviton 4 | clang-21, default flags | **1.025** | 0.995 | 1.006 | 0.1% | 0.953 (semanticscholar-corpus) | 1.071 (canada) |
| Graviton 4 | gcc-15, -mcpu=native | **1.111** | 1.000 | 1.044 | 0.3% | 1.029 (mesh) | 1.203 (random) |
| Graviton 4 | clang-21, -mcpu=native | **1.178** | 1.000 | 1.071 | 0.2% | 1.069 (semanticscholar-corpus) | 1.287 (mesh.pretty) |
| Graviton 5 | gcc-15, default flags | **1.091** | 1.001 | 1.035 | 0.2% | 0.994 (semanticscholar-corpus) | 1.158 (random) |
| Graviton 5 | clang-21, default flags | **1.022** | 1.000 | 1.007 | 0.1% | 0.949 (semanticscholar-corpus) | 1.085 (gsoc-2018) |
| Graviton 5 | gcc-15, -mcpu=native | **1.083** | 1.003 | 1.036 | 0.3% | 1.020 (marine_ik) | 1.225 (gsoc-2018) |
| Graviton 5 | clang-21, -mcpu=native | **1.105** | 1.004 | 1.046 | 0.2% | 0.992 (semanticscholar-corpus) | 1.238 (gsoc-2018) |

## Isolating the two changes (`-mcpu=native`, geomean PR-variant / base)

| Machine | Compiler | Variant | Stage 1 | Stage 2 | Whole parse |
|---|---|---|---|---|---|
| Graviton 3 | gcc-15 | full PR | 1.014 | 0.995 | 1.003 |
| Graviton 3 | gcc-15 | Mixed4 writer only | 1.013 | 0.998 | 1.005 |
| Graviton 3 | gcc-15 | SVE2 match only | 1.000 | 1.000 | 1.000 |
| Graviton 3 | clang-21 | full PR | 1.016 | 0.999 | 1.005 |
| Graviton 3 | clang-21 | Mixed4 writer only | 1.019 | 0.999 | 1.007 |
| Graviton 3 | clang-21 | SVE2 match only | 1.001 | 1.000 | 1.000 |
| Graviton 4 | gcc-15 | full PR | 1.111 | 1.000 | 1.044 |
| Graviton 4 | gcc-15 | Mixed4 writer only | 1.021 | 0.999 | 1.008 |
| Graviton 4 | gcc-15 | SVE2 match only | 1.082 | 0.999 | 1.033 |
| Graviton 4 | clang-21 | full PR | 1.178 | 1.000 | 1.071 |
| Graviton 4 | clang-21 | Mixed4 writer only | 1.024 | 1.000 | 1.009 |
| Graviton 4 | clang-21 | SVE2 match only | 1.144 | 1.001 | 1.061 |
| Graviton 5 | gcc-15 | full PR | 1.083 | 1.003 | 1.036 |
| Graviton 5 | gcc-15 | Mixed4 writer only | 1.016 | 1.001 | 1.006 |
| Graviton 5 | gcc-15 | SVE2 match only | 1.057 | 0.998 | 1.024 |
| Graviton 5 | clang-21 | full PR | 1.105 | 1.004 | 1.046 |
| Graviton 5 | clang-21 | Mixed4 writer only | 1.017 | 0.999 | 1.006 |
| Graviton 5 | clang-21 | SVE2 match only | 1.074 | 1.003 | 1.034 |

On Graviton 3 the SVE2 path does not compile (SVE only, no `MATCH` instruction), so the "SVE2 match only" variant is byte-for-byte the base code there: its 1.000 readings are a null control that confirms the measurement noise floor. The "full PR" row on Graviton 3 is therefore the Mixed4 writer alone.

## Absolute stage-1 throughput, base vs PR (GB/s, best)

| Machine | Build | base S1 | PR S1 | base total | PR total |
|---|---|---|---|---|---|
| Graviton 3 | gcc-15, default flags | 3.86 | 4.11 | 1.55 | 1.58 |
| Graviton 3 | clang-21, default flags | 3.95 | 4.02 | 1.53 | 1.55 |
| Graviton 3 | gcc-15, -mcpu=native | 4.19 | 4.24 | 1.60 | 1.61 |
| Graviton 3 | clang-21, -mcpu=native | 4.00 | 4.07 | 1.55 | 1.55 |
| Graviton 4 | gcc-15, default flags | 4.89 | 5.31 | 1.98 | 2.05 |
| Graviton 4 | clang-21, default flags | 4.84 | 4.97 | 1.95 | 1.96 |
| Graviton 4 | gcc-15, -mcpu=native | 5.46 | 6.06 | 2.08 | 2.17 |
| Graviton 4 | clang-21, -mcpu=native | 4.83 | 5.69 | 1.95 | 2.09 |
| Graviton 5 | gcc-15, default flags | 6.14 | 6.69 | 2.52 | 2.61 |
| Graviton 5 | clang-21, default flags | 6.19 | 6.33 | 2.46 | 2.48 |
| Graviton 5 | gcc-15, -mcpu=native | 7.09 | 7.68 | 2.70 | 2.79 |
| Graviton 5 | clang-21, -mcpu=native | 6.29 | 6.95 | 2.50 | 2.62 |

(geomean over files of the per-file GB/s)

## Per-file stage-1 ratios (PR / base)

### Graviton 3 (c7g.2xlarge, Neoverse-V1, SVE (256-bit), no SVE2)

| file | gcc-15, default flags | clang-21, default flags | gcc-15, -mcpu=native | clang-21, -mcpu=native |
|---|---|---|---|---|
| apache_builds | 1.103 | 1.048 | 1.050 | 1.044 |
| canada | 1.088 | 1.011 | 1.004 | 1.020 |
| citm_catalog | 1.073 | 1.007 | 1.043 | 1.062 |
| github_events | 1.062 | 1.017 | 1.022 | 1.009 |
| google_maps_api_compact_response | 1.130 | 1.016 | 1.046 | 1.016 |
| google_maps_api_response | 1.109 | 1.023 | 1.103 | 1.024 |
| gsoc-2018 | 1.010 | 0.992 | 0.999 | 0.985 |
| instruments | 1.115 | 1.035 | 1.056 | 1.035 |
| marine_ik | 1.028 | 0.968 | 0.961 | 0.966 |
| mesh | 1.075 | 1.029 | 1.011 | 1.033 |
| mesh.pretty | 1.083 | 1.055 | 1.046 | 1.067 |
| numbers | 1.063 | 1.046 | 0.997 | 1.049 |
| random | 1.087 | 1.020 | 1.005 | 1.031 |
| repeat | 1.009 | 0.981 | 1.011 | 1.006 |
| semanticscholar-corpus | 0.999 | 0.980 | 0.964 | 0.968 |
| tree-pretty | 1.042 | 1.038 | 1.040 | 1.031 |
| twitter | 1.058 | 1.042 | 1.004 | 0.985 |
| twitter_api_compact_response | 1.085 | 1.030 | 1.019 | 1.030 |
| twitter_api_response | 1.031 | 1.035 | 0.979 | 1.039 |
| twitter_timeline | 1.059 | 1.039 | 1.002 | 1.037 |
| twitterescaped | 1.104 | 1.021 | 1.010 | 1.012 |
| update-center | 1.003 | 0.949 | 0.946 | 0.912 |
| **geomean** | **1.064** | **1.017** | **1.014** | **1.016** |

### Graviton 4 (c8g.2xlarge, Neoverse-V2, SVE2 (128-bit))

| file | gcc-15, default flags | clang-21, default flags | gcc-15, -mcpu=native | clang-21, -mcpu=native |
|---|---|---|---|---|
| apache_builds | 1.113 | 1.039 | 1.138 | 1.246 |
| canada | 1.093 | 1.071 | 1.075 | 1.146 |
| citm_catalog | 1.095 | 1.057 | 1.151 | 1.287 |
| github_events | 1.064 | 1.027 | 1.138 | 1.247 |
| google_maps_api_compact_response | 1.115 | 1.008 | 1.046 | 1.079 |
| google_maps_api_response | 1.120 | 1.014 | 1.084 | 1.145 |
| gsoc-2018 | 1.007 | 1.026 | 1.141 | 1.228 |
| instruments | 1.183 | 1.028 | 1.137 | 1.154 |
| marine_ik | 1.061 | 0.993 | 1.040 | 1.072 |
| mesh | 1.078 | 1.008 | 1.029 | 1.099 |
| mesh.pretty | 1.099 | 1.057 | 1.177 | 1.287 |
| numbers | 1.094 | 1.033 | 1.091 | 1.151 |
| random | 1.127 | 1.032 | 1.203 | 1.223 |
| repeat | 1.045 | 1.012 | 1.084 | 1.193 |
| semanticscholar-corpus | 0.994 | 0.953 | 1.076 | 1.069 |
| tree-pretty | 1.113 | 1.032 | 1.091 | 1.177 |
| twitter | 1.075 | 1.028 | 1.115 | 1.196 |
| twitter_api_compact_response | 1.097 | 1.027 | 1.120 | 1.171 |
| twitter_api_response | 1.088 | 1.030 | 1.112 | 1.204 |
| twitter_timeline | 1.114 | 1.032 | 1.123 | 1.187 |
| twitterescaped | 1.075 | 1.019 | 1.128 | 1.189 |
| update-center | 1.048 | 1.032 | 1.153 | 1.190 |
| **geomean** | **1.086** | **1.025** | **1.111** | **1.178** |

### Graviton 5 (c9g.2xlarge, Neoverse-V3, SVE2 (128-bit))

| file | gcc-15, default flags | clang-21, default flags | gcc-15, -mcpu=native | clang-21, -mcpu=native |
|---|---|---|---|---|
| apache_builds | 1.097 | 1.031 | 1.079 | 1.129 |
| canada | 1.099 | 1.054 | 1.032 | 1.100 |
| citm_catalog | 1.068 | 1.038 | 1.124 | 1.182 |
| github_events | 1.064 | 1.022 | 1.118 | 1.153 |
| google_maps_api_compact_response | 1.129 | 1.014 | 1.064 | 1.030 |
| google_maps_api_response | 1.124 | 0.994 | 1.048 | 1.063 |
| gsoc-2018 | 1.016 | 1.085 | 1.225 | 1.238 |
| instruments | 1.109 | 1.016 | 1.057 | 1.082 |
| marine_ik | 1.050 | 1.030 | 1.020 | 1.043 |
| mesh | 1.088 | 1.044 | 1.042 | 1.049 |
| mesh.pretty | 1.113 | 1.026 | 1.150 | 1.153 |
| numbers | 1.132 | 1.015 | 1.027 | 1.081 |
| random | 1.158 | 1.019 | 1.090 | 1.127 |
| repeat | 1.046 | 1.005 | 1.110 | 1.142 |
| semanticscholar-corpus | 0.994 | 0.949 | 1.070 | 0.992 |
| tree-pretty | 1.122 | 1.020 | 1.063 | 1.096 |
| twitter | 1.078 | 1.018 | 1.096 | 1.124 |
| twitter_api_compact_response | 1.106 | 1.022 | 1.083 | 1.106 |
| twitter_api_response | 1.094 | 1.028 | 1.091 | 1.127 |
| twitter_timeline | 1.123 | 1.028 | 1.078 | 1.101 |
| twitterescaped | 1.083 | 1.017 | 1.093 | 1.120 |
| update-center | 1.114 | 1.022 | 1.082 | 1.103 |
| **geomean** | **1.091** | **1.022** | **1.083** | **1.105** |


## Reproducing

Raw `parse -t` TSVs and the analysis script are in `benchresults/pr2863_graviton/` (`python3 analyze.py <results_dir> report.md`).
