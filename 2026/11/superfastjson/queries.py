"""Six queries over six NDJSON files, as Python code would write them.

    python queries.py DATADIR twitter fastsimdjson 64
    python queries.py DATADIR twitter msgspec 64        # a pool of processes
    python queries.py DATADIR twitter orjson 1

Each query gives one result per record (missing values are None), in the
order of the records. The libraries other than fastsimdjson parse the file in
pieces on a pool of processes; each process maps the file once and sends back
the results of its pieces. fastsimdjson parses the whole file with threads
(free-threaded Python). Prints the best time of several runs, the number of
values found and a checksum of the results, to check the libraries against
each other.

The files are those of the Pison benchmark (github.com/AutomataLab/Pison),
one record per line.
"""
import mmap
import os
import sys
import time
import zlib
from typing import Union

import msgspec

_maps = {}


def mapped(path):
    """The file, mapped in memory once per process."""
    m = _maps.get(path)
    if m is None:
        with open(path, "rb") as f:
            m = _maps[path] = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    return m


# --- the queries, for dicts and lists (json, orjson) -------------------------

def at(d, *path):
    for k in path:
        if isinstance(k, int):
            if not isinstance(d, list) or len(d) <= k:
                return None
        elif not isinstance(d, dict) or k not in d:
            return None
        d = d[k]
    return d


def py_twitter(d):
    return at(d, "user", "lang"), at(d, "lang")


def py_bestbuy(d):
    return at(d, "categoryPath", 1, "id"), at(d, "categoryPath", 2, "id")


def py_google_map(d):
    return [s["distance"]["text"] for r in d["routes"] for leg in r["legs"]
            for s in leg["steps"]]


def py_nspl(d):
    return at(d, 8), at(d, 9)


def py_walmart(d):
    return at(d, "bestMarketplacePrice", "price"), at(d, "name")


def py_wiki(d):
    return [c["mainsnak"]["property"] for c in at(d, "claims", "P150") or []]


# --- msgspec: typed structures with only the fields of the query -------------

class User(msgspec.Struct):
    lang: Union[str, None] = None


class Tweet(msgspec.Struct):
    user: Union[User, None] = None
    lang: Union[str, None] = None


class Category(msgspec.Struct):
    id: Union[str, None] = None


class Product(msgspec.Struct):
    categoryPath: list[Category] = []


class Distance(msgspec.Struct):
    text: str


class Step(msgspec.Struct):
    distance: Distance


class Leg(msgspec.Struct):
    steps: list[Step]


class Route(msgspec.Struct):
    legs: list[Leg]


class Directions(msgspec.Struct):
    routes: list[Route] = []


class Price(msgspec.Struct):
    price: Union[float, None] = None


class Item(msgspec.Struct):
    bestMarketplacePrice: Union[Price, None] = None
    name: Union[str, None] = None


class Snak(msgspec.Struct):
    property: str


class Claim(msgspec.Struct):
    mainsnak: Snak


class Claims(msgspec.Struct):
    P150: list[Claim] = []


class Entity(msgspec.Struct):
    claims: Union[Claims, list, None] = None   # an empty claims is []


MSGSPEC = {
    "twitter": (Tweet, lambda t: (t.user.lang if t.user else None, t.lang)),
    "bestbuy": (Product, lambda p: (p.categoryPath[1].id if len(p.categoryPath) > 1 else None,
                                    p.categoryPath[2].id if len(p.categoryPath) > 2 else None)),
    "google_map": (Directions, lambda d: [s.distance.text for r in d.routes for leg in r.legs
                                          for s in leg.steps]),
    "nspl": (list, lambda a: (a[8] if len(a) > 8 else None, a[9] if len(a) > 9 else None)),
    "walmart": (Item, lambda i: (i.bestMarketplacePrice.price if i.bestMarketplacePrice else None,
                                 i.name)),
    "wiki": (Entity, lambda e: [c.mainsnak.property for c in e.claims.P150]
             if isinstance(e.claims, Claims) else []),
}
PYTHON = {"twitter": py_twitter, "bestbuy": py_bestbuy, "google_map": py_google_map,
          "nspl": py_nspl, "walmart": py_walmart, "wiki": py_wiki}


# --- the competitors: each worker returns the results of its piece -----------

_decoders = {}


def run_piece(job):
    library, query, path, start, end = job
    if library == "msgspec":
        if query not in _decoders:
            _decoders[query] = msgspec.json.Decoder(MSGSPEC[query][0])
        fn = MSGSPEC[query][1]
        return [fn(x) for x in _decoders[query].decode_lines(memoryview(mapped(path))[start:end])]
    if library == "orjson":
        import orjson as lib
    else:
        import json as lib
    fn = PYTHON[query]
    return [fn(lib.loads(line)) for line in mapped(path)[start:end].splitlines()]


def pieces(path, count):
    m = mapped(path)
    bounds = [0]
    for k in range(1, count):
        p = m.find(b"\n", max(len(m) * k // count, bounds[-1]))
        bounds.append(len(m) if p < 0 else p + 1)
    bounds.append(len(m))
    return [(a, b) for a, b in zip(bounds, bounds[1:]) if a < b]


# --- fastsimdjson: one call, on threads --------------------------------------

def google_map_steps(doc):
    # doc is a lazy view: only the values visited become Python objects
    return [s["distance"]["text"] for r in doc["routes"] for leg in r["legs"]
            for s in leg["steps"]]


def wiki_properties(claims):
    return [c["mainsnak"]["property"] for c in claims]


FASTSIMDJSON = {
    "twitter": dict(fields=["/user/lang", "/lang"]),
    "bestbuy": dict(fields=["/categoryPath/1/id", "/categoryPath/2/id"]),
    "google_map": dict(lazy=True, fn=google_map_steps),
    "nspl": dict(fields=["/8", "/9"]),
    "walmart": dict(fields=["/bestMarketplacePrice/price", "/name"]),
    "wiki": dict(fields="/claims/P150", default=[], fn=wiki_properties),
}


def run_fastsimdjson(query, path, threads):
    import fastsimdjson
    kw = dict(FASTSIMDJSON[query])
    fn = kw.pop("fn", None)
    data = memoryview(mapped(path))
    if fn is None:
        return fastsimdjson.loads_many(data, threads=threads, **kw)
    return fastsimdjson.map_many(fn, data, threads=threads, **kw)


# --- checking and timing ------------------------------------------------------

def checksum(results):
    """Values found, and a CRC of the results (numbers as floats)."""
    found = 0
    crc = 0
    for r in results:
        values = list(r)
        found += sum(v is not None for v in values)
        norm = [float(v) if isinstance(v, (int, float)) and not isinstance(v, bool) else v
                for v in values]
        crc = zlib.crc32(repr(norm).encode(), crc)
    return found, crc


def main():
    datadir, query, library, workers = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])
    runs = int(os.environ.get("RUNS", "5"))
    path = os.path.join(datadir, query + ".ndjson")
    size = os.path.getsize(path)
    pool = None
    if library == "fastsimdjson":
        kind = "threads"
        count = lambda: run_fastsimdjson(query, path, workers)
    else:
        kind = "processes"
        jobs = [(library, query, path, a, b) for a, b in pieces(path, workers * 4)]
        if workers == 1:
            count = lambda: [r for j in jobs for r in run_piece(j)]
        else:
            from concurrent.futures import ProcessPoolExecutor
            pool = ProcessPoolExecutor(max_workers=workers)
            list(pool.map(abs, range(workers * 4)))   # start the workers
            count = lambda: [r for part in pool.map(run_piece, jobs) for r in part]
    results = count()   # warm up: the workers map the file
    best = float("inf")
    for _ in range(runs):
        del results
        t = time.perf_counter()
        results = count()
        best = min(best, time.perf_counter() - t)
    if pool is not None:
        pool.shutdown()
    found, crc = checksum(results)
    print(f"{query:11s} {library:13s} {kind:9s} {workers:4d} {best:8.4f} s {size / best / 1e9:7.2f} GB/s"
          f"  records {len(results)} found {found} crc {crc:08x}", flush=True)


if __name__ == "__main__":
    main()
