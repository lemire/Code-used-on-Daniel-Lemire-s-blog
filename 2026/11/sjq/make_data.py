#!/usr/bin/env python3
"""Writes the benchmark inputs, built from tests/data/twitter.json.

Usage: make_data.py OUTDIR [--mb SIZE]

  tweets.ndjson    one tweet per line (about SIZE MB; ids made unique)
  timeline.json    one document, {"statuses": [...]}, about SIZE/2 MB
"""
import argparse
import json
import os

HERE = os.path.dirname(os.path.abspath(__file__))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("outdir")
    ap.add_argument("--mb", type=int, default=1024)
    args = ap.parse_args()
    with open(os.path.join(HERE, "..", "tests", "data", "twitter.json"), encoding="utf-8") as f:
        statuses = json.load(f)["statuses"]
    lines = [json.dumps(s, ensure_ascii=False, separators=(",", ":")) for s in statuses]
    os.makedirs(args.outdir, exist_ok=True)
    target = args.mb << 20
    written, n = 0, 0
    with open(os.path.join(args.outdir, "tweets.ndjson"), "w", encoding="utf-8") as f:
        while written < target:
            s = dict(statuses[n % len(statuses)])
            s["id"] = 500000000000000000 + n
            line = json.dumps(s, ensure_ascii=False, separators=(",", ":")) + "\n"
            f.write(line)
            written += len(line.encode("utf-8"))
            n += 1
    docs, size = [], 0
    while size < target // 2:
        s = dict(statuses[len(docs) % len(statuses)])
        s["id"] = 600000000000000000 + len(docs)
        docs.append(s)
        size += len(lines[len(docs) % len(lines)]) + 1
    with open(os.path.join(args.outdir, "timeline.json"), "w", encoding="utf-8") as f:
        json.dump({"statuses": docs}, f, ensure_ascii=False, indent=2)
    print(f"tweets.ndjson: {n} lines; timeline.json: {len(docs)} statuses")


if __name__ == "__main__":
    main()
