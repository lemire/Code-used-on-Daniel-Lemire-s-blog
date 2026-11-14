"""Which GitHub repositories got the most stars?

Reads a file of GitHub events (NDJSON, one event per line, from GH Archive)
and counts the stars (WatchEvent) of each repository.

    python bench.py events.ndjson fastsimdjson 64
    python bench.py events.ndjson msgspec 64 process
    python bench.py events.ndjson msgspec 64 thread     # free-threaded Python

The libraries other than fastsimdjson parse the file in pieces, on a pool of
processes (or threads), and each piece returns its own counts. The file is
mapped in memory once (in each process) and stays mapped; the pool is started
and the file mapped before the clock starts. Prints the best time of several
runs and the top three repositories, so the libraries can be checked against
each other.
"""
import collections
import mmap
import os
import sys
import time

_maps = {}


def mapped(path):
    """The file, mapped in memory once per process."""
    m = _maps.get(path)
    if m is None:
        with open(path, "rb") as f:
            m = _maps[path] = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    return m


# --- the competitors: each worker counts the stars in its piece of the file

def count_json(job):
    import json
    path, start, end = job
    stars = collections.Counter()
    for line in mapped(path)[start:end].splitlines():
        event = json.loads(line)
        if event["type"] == "WatchEvent":
            stars[event["repo"]["name"]] += 1
    return stars


def count_orjson(job):
    import orjson
    path, start, end = job
    stars = collections.Counter()
    for line in mapped(path)[start:end].splitlines():
        event = orjson.loads(line)
        if event["type"] == "WatchEvent":
            stars[event["repo"]["name"]] += 1
    return stars


_decoder = None


def count_msgspec(job):
    # msgspec decodes into typed structures, skips the other fields, and
    # reads its piece of the file in place
    global _decoder
    import msgspec
    if _decoder is None:
        class Repo(msgspec.Struct):
            name: str

        class Event(msgspec.Struct):
            type: str
            repo: Repo

        _decoder = msgspec.json.Decoder(Event)
    path, start, end = job
    stars = collections.Counter()
    for event in _decoder.decode_lines(memoryview(mapped(path))[start:end]):
        if event.type == "WatchEvent":
            stars[event.repo.name] += 1
    return stars


COMPETITORS = {"json": count_json, "orjson": count_orjson, "msgspec": count_msgspec}


def merge(counters):
    stars = collections.Counter()
    for c in counters:
        stars.update(c)
    return stars


def pieces(path, count):
    """Cuts the file into count pieces that end with a newline."""
    m = mapped(path)
    bounds = [0]
    for k in range(1, count):
        p = m.find(b"\n", max(len(m) * k // count, bounds[-1]))
        bounds.append(len(m) if p < 0 else p + 1)
    bounds.append(len(m))
    return [(path, a, b) for a, b in zip(bounds, bounds[1:]) if a < b]


# --- fastsimdjson: one call, on threads

def starred(event):
    kind, name = event
    return name if kind == "WatchEvent" else None


def count_fastsimdjson(path, threads):
    import fastsimdjson
    names = fastsimdjson.map_many(starred, memoryview(mapped(path)), threads=threads,
                                  fields=["/type", "/repo/name"], drop_none=True)
    return collections.Counter(names)


def main():
    path, library, workers = sys.argv[1], sys.argv[2], int(sys.argv[3])
    kind = sys.argv[4] if len(sys.argv) > 4 else "process"
    runs = int(os.environ.get("RUNS", "5"))
    size = os.path.getsize(path)

    if library == "fastsimdjson":
        kind = "thread"
        mapped(path)
        count = lambda: count_fastsimdjson(path, workers)
        close = lambda: None
    else:
        fn = COMPETITORS[library]
        jobs = pieces(path, workers * 4)
        if workers == 1:
            count = lambda: merge(map(fn, jobs))
            close = lambda: None
        else:
            if kind == "process":
                from concurrent.futures import ProcessPoolExecutor as Pool
            else:
                from concurrent.futures import ThreadPoolExecutor as Pool
            pool = Pool(max_workers=workers)
            list(pool.map(abs, range(workers * 4)))   # start the workers
            count = lambda: merge(pool.map(fn, jobs))
            close = pool.shutdown

    count()   # warm up: the workers map the file
    best = float("inf")
    for _ in range(runs):
        t = time.perf_counter()
        stars = count()
        best = min(best, time.perf_counter() - t)
    close()
    top = ",".join(f"{name}:{n}" for name, n in stars.most_common(3))
    gil = "gil" if getattr(sys, "_is_gil_enabled", lambda: True)() else "nogil"
    print(f"{library:13s} {kind:7s} {workers:4d} {best:9.4f} s {size / best / 1e9:8.3f} GB/s"
          f"  python {sys.version.split()[0]} {gil}  stars {sum(stars.values())} {top}",
          flush=True)


if __name__ == "__main__":
    main()
