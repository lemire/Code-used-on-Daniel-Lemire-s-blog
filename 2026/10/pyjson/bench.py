"""Which languages get the most pull requests? GitHub event logs (GH Archive) in Python.

The input: hours of GH Archive (https://www.gharchive.org/), one JSON event
per line, decompressed in memory before timing. The task: count the events
by type, and count the pull requests opened (PullRequestEvent with action
"opened") per language of the repository they target
(payload.pull_request.base.repo.language). Every method computes the same
two counters, checked against the first one.

  python bench.py --methods all --workers 64      # methods available in this Python

Methods (name: what runs):
  json, orjson, jiter, msgspec                 one thread, one event at a time
  jiter-nocache                                (jiter without its string cache)
                                               (msgspec decodes into a typed Struct
                                               that declares only the fields used)
  fastsimdjson, fastsimdjson-lazy              one thread: map_many(fn, data, threads=1)
  processes-orjson, processes-jiter,           N forked processes (multiprocessing):
  processes-msgspec                            they inherit the input, get line ranges,
                                               and return their counters
  threads-json, threads-jiter, threads-msgspec N threads (concurrent.futures) over
                                               line ranges; free-threaded Python
                                               (jiter without its string cache)
  threads-jiter-cache                          jiter with its string cache, N threads
  threads-fastsimdjson, threads-fastsimdjson-lazy
                                               map_many(fn, data, threads=N), with
                                               per-thread counters; free-threaded Python

Prints CSV: method,python,gil,workers,seconds,gbps,events
"""
import argparse
import collections
import concurrent.futures
import glob
import gzip
import json
import multiprocessing as mp
import os
import sys
import threading
import time

DATA_DIR = os.environ.get("GHARCHIVE", "data")
Counter = collections.Counter


def load(hours):
    files = sorted(glob.glob(os.path.join(DATA_DIR, "*.json.gz")))[:hours]
    parts = [gzip.decompress(open(f, "rb").read()).rstrip(b"\n") for f in files]
    return b"\n".join(parts), files


def line_ranges(buf, n):
    """About n (start, end) ranges of buf, each made of whole lines."""
    out, start, step = [], 0, max(1, len(buf) // n)
    while start < len(buf):
        end = buf.find(b"\n", min(start + step, len(buf) - 1))
        end = len(buf) if end < 0 else end + 1
        out.append((start, end))
        start = end
    return out


def count_dicts(events):
    """The task on parsed events (dicts or views): (types, languages)."""
    types, languages = Counter(), Counter()
    for e in events:
        t = e["type"]
        types[t] += 1
        if t == "PullRequestEvent":
            p = e["payload"]
            if p["action"] == "opened":
                languages[p["pull_request"]["base"]["repo"]["language"]] += 1
    return types, languages


def merge(results):
    """Add the counters of the workers (small: a few dozen types, about a
    hundred languages)."""
    types, languages = Counter(), Counter()
    for t, l in results:
        types.update(t)
        languages.update(l)
    return types, languages


# --- msgspec: decode only the fields used, into typed structs --------------
def msgspec_decoder():
    import msgspec
    from typing import Optional

    class Repo(msgspec.Struct):
        language: Optional[str] = None

    class Base(msgspec.Struct):
        repo: Repo

    class PullRequest(msgspec.Struct):
        base: Base

    class Payload(msgspec.Struct):
        action: Optional[str] = None
        pull_request: Optional[PullRequest] = None

    class Event(msgspec.Struct):
        type: str
        payload: Payload

    dec = msgspec.json.Decoder(Event)

    def count(buf):
        types, languages = Counter(), Counter()
        for e in dec.decode_lines(buf):
            types[e.type] += 1
            if e.type == "PullRequestEvent" and e.payload.action == "opened":
                languages[e.payload.pull_request.base.repo.language] += 1
        return types, languages

    return count


# --- one thread ---------------------------------------------------------------
def one_json(data):
    return count_dicts(json.loads(line) for line in data.split(b"\n"))


def one_orjson(data):
    import orjson
    return count_dicts(orjson.loads(line) for line in data.split(b"\n"))


def one_jiter(data, cache=True):
    import jiter
    return count_dicts(jiter.from_json(line, cache_mode=cache) for line in data.split(b"\n"))


def one_msgspec(data):
    count = msgspec_decoder()
    mv = memoryview(data)
    return merge(count(mv[s:e]) for s, e in line_ranges(data, 256))


# map_many calls one_event in its threads, once per event. Each thread has
# its own counter and list (threading.local, no lock). The function is at
# module level: with free-threaded Python, a closure variable read at every
# call is a shared object whose reference count all threads update, and the
# threads slow each other down.
_LOCAL = threading.local()
_PARTS = []


def one_event(e):
    acc = getattr(_LOCAL, "acc", None)
    if acc is None:
        acc = _LOCAL.acc = (Counter(), Counter())
        _PARTS.append(acc)  # appending to a list is thread-safe
    t = e["type"]
    acc[0][t] += 1
    if t == "PullRequestEvent":
        p = e["payload"]
        if p["action"] == "opened":
            acc[1][p["pull_request"]["base"]["repo"]["language"]] += 1


def map_many_counts(data, threads, lazy):
    import fastsimdjson
    global _LOCAL, _PARTS
    _LOCAL, _PARTS = threading.local(), []
    fastsimdjson.map_many(one_event, data, format="lines", threads=threads, lazy=lazy)
    return merge(_PARTS)


# --- processes ----------------------------------------------------------------
SHARED = b""  # inherited by the forked workers


def _proc_orjson(span):
    import orjson
    s, e = span
    return count_dicts(orjson.loads(line) for line in SHARED[s:e].rstrip(b"\n").split(b"\n"))


def _proc_jiter(span):
    import jiter
    s, e = span
    return count_dicts(jiter.from_json(line) for line in SHARED[s:e].rstrip(b"\n").split(b"\n"))


_DEC = None


def _proc_msgspec(span):
    global _DEC
    if _DEC is None:
        _DEC = msgspec_decoder()
    s, e = span
    return _DEC(memoryview(SHARED)[s:e])


# --- threads (free-threaded Python) ---------------------------------------
def threads_json(data, ex, n):
    def work(span):
        s, e = span
        return count_dicts(json.loads(line) for line in data[s:e].rstrip(b"\n").split(b"\n"))
    return merge(ex.map(work, line_ranges(data, n * 8)))


# jiter's string cache is shared by all threads: with it (the default, or
# cache_mode="keys"), 64 threads run no faster than one. Without it, they scale.
def threads_jiter(data, ex, n, cache=False):
    import jiter

    def work(span):
        s, e = span
        return count_dicts(jiter.from_json(line, cache_mode=cache) for line in data[s:e].rstrip(b"\n").split(b"\n"))
    return merge(ex.map(work, line_ranges(data, n * 8)))


def threads_msgspec(data, ex, n):
    count = msgspec_decoder()
    mv = memoryview(data)
    return merge(ex.map(lambda r: count(mv[r[0]:r[1]]), line_ranges(data, n * 8)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--hours", type=int, default=4)
    ap.add_argument("--workers", type=int, default=64)
    ap.add_argument("--methods", default="all")
    ap.add_argument("--rounds", type=int, default=3)
    ap.add_argument("--slow-rounds", type=int, default=1, help="rounds for the one-thread methods")
    args = ap.parse_args()
    global SHARED
    data, files = load(args.hours)
    SHARED = data
    gil = sys._is_gil_enabled() if hasattr(sys, "_is_gil_enabled") else True
    py = sys.version.split()[0] + ("" if gil else "t")
    n = args.workers
    events = data.count(b"\n") + 1
    print(f"# {len(files)} files, {len(data) / 1e9:.2f} GB, {events} events", file=sys.stderr)

    methods = {}
    if gil:
        methods["json"] = (lambda: one_json(data), 1, True)
        methods["orjson"] = (lambda: one_orjson(data), 1, True)
        methods["jiter"] = (lambda: one_jiter(data), 1, True)
        methods["jiter-nocache"] = (lambda: one_jiter(data, False), 1, True)
        methods["msgspec"] = (lambda: one_msgspec(data), 1, True)
        methods["fastsimdjson"] = (lambda: map_many_counts(data, 1, False), 1, True)
        methods["fastsimdjson-lazy"] = (lambda: map_many_counts(data, 1, True), 1, True)
        ctx = mp.get_context("fork")
        pool = ctx.Pool(n)
        pool.map(abs, range(n))  # start the workers
        spans = line_ranges(data, n * 8)
        methods["processes-orjson"] = (lambda: merge(pool.map(_proc_orjson, spans)), n, False)
        methods["processes-jiter"] = (lambda: merge(pool.map(_proc_jiter, spans)), n, False)
        methods["processes-msgspec"] = (lambda: merge(pool.map(_proc_msgspec, spans)), n, False)
    else:
        ex = concurrent.futures.ThreadPoolExecutor(n)
        list(ex.map(abs, range(n)))  # start the threads
        methods["threads-json"] = (lambda: threads_json(data, ex, n), n, True)
        methods["threads-jiter"] = (lambda: threads_jiter(data, ex, n), n, False)
        methods["threads-jiter-cache"] = (lambda: threads_jiter(data, ex, n, True), n, True)
        methods["threads-msgspec"] = (lambda: threads_msgspec(data, ex, n), n, False)
        methods["threads-fastsimdjson"] = (lambda: map_many_counts(data, n, False), n, False)
        methods["threads-fastsimdjson-lazy"] = (lambda: map_many_counts(data, n, True), n, False)
        map_many_counts(data[: data.find(b"\n", 1 << 22)], n, False)  # start map_many's workers

    wanted = list(methods) if args.methods == "all" else args.methods.split(",")
    reference = None
    print("method,python,gil,workers,seconds,gbps,events", flush=True)
    for name in wanted:
        fn, workers, slow = methods[name]
        best = float("inf")
        for _ in range(args.slow_rounds if slow else args.rounds):
            t0 = time.perf_counter()
            result = fn()
            best = min(best, time.perf_counter() - t0)
        if reference is None:
            reference = result
            top = ", ".join(f"{r} {c}" for r, c in result[1].most_common(8))
            print(f"# {sum(result[0].values())} events; types: {dict(result[0].most_common(4))}; "
                  f"pull requests opened: {sum(result[1].values())}: {top}", file=sys.stderr)
        assert result == reference, f"{name}: different counts"
        print(f"{name},{py},{'on' if gil else 'off'},{workers},{best:.4f},"
              f"{len(data) / best / 1e9:.3f},{events}", flush=True)


if __name__ == "__main__":
    main()
