#!/usr/bin/env python3
"""Summarize the simdjson 4.0.0 vs 5.0.0 runs: best of all rounds for each benchmark."""
import glob, json, re, sys

d = sys.argv[1] if len(sys.argv) > 1 else "results"

def best(pattern, parse):
    out = {}
    for f in glob.glob(f"{d}/{pattern}"):
        for k, v in parse(f):
            out[k] = max(out.get(k, 0), v)
    return out

def parse_tsv(col):
    def p(f):
        for line in open(f):
            m = re.search(r'"([^"]+)"\t(.*)', line)
            if m:
                yield m.group(1), float(m.group(2).split("\t")[col])
    return p

def parse_ser(f):
    for line in open(f):
        m = re.match(r"(\S+)\.json\s+serialize\s+([\d.]+) GB/s", line)
        if m:
            yield m.group(1), float(m.group(2))

def parse_gb(f):
    for b in json.load(open(f))["benchmarks"]:
        if b.get("run_type") == "iteration" and "bytes_per_second" in b:
            yield b["run_name"].replace("/manual_time", ""), b["bytes_per_second"] / 1e9

def table(title, pattern, parse):
    v4, v5 = best(pattern.format("v4"), parse), best(pattern.format("v5"), parse)
    print(f"\n### {title} (GB/s, best of 3 rounds)\n")
    print("| benchmark | 4.0.0 | 5.0.0 | speedup |")
    print("|---|--:|--:|--:|")
    for k in v4:
        if k in v5:
            print(f"| {k} | {v4[k]:.2f} | {v5[k]:.2f} | {v5[k] / v4[k]:.2f} |")

table("DOM parse, all stages", "parse_{}_r*.tsv", parse_tsv(4))
table("Stage 1 only", "parse_{}_r*.tsv", parse_tsv(5))
table("Stage 2 only", "parse_{}_r*.tsv", parse_tsv(6))
table("DOM serialization (minify)", "serialize_{}_r*.txt", parse_ser)
table("bench_ondemand", "ondemand_{}_r*.json", parse_gb)
