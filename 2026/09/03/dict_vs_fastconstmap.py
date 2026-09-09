"""Python's `dict` versus fastconstmap's immutable string-to-uint64 maps.

`fastconstmap` builds a compact, immutable lookup structure from a
`dict[str, int]`: a lookup costs one xxhash and a few array reads. Two flavours
are exposed. `ConstMap` is minimal (about 9 bytes per key) and returns an
undefined value for a key that was never inserted. `VerifiedConstMap` is about
twice as large and detects missing keys, so it behaves like a `dict`.

One subtlety dominates the comparison. A Python `str` caches its hash value in
the object the first time it is needed. A `dict` lookup on a key object you
have already used therefore skips hashing entirely, while `fastconstmap` hashes
the bytes every time. We measure both cases: "warm" keys whose hash is already
cached, and "cold" keys that have never been hashed.
"""
import random
import statistics
import string
import sys
import time
from array import array

from fastconstmap import ConstMap, VerifiedConstMap, NOT_FOUND

REPEATS = 5
KEY_LENGTH = 16


def make_keys(n, rng):
    alphabet = string.ascii_lowercase + string.digits
    keys, seen = [], set()
    while len(keys) < n:
        k = "".join(rng.choice(alphabet) for _ in range(KEY_LENGTH))
        if k not in seen:
            seen.add(k)
            keys.append(k)
    return keys


def copy_keys(keys):
    """Fresh str objects with the same text and no cached hash."""
    return [(k + ".")[:-1] for k in keys]


def time_it(fn, *args):
    runs = []
    for _ in range(REPEATS):
        start = time.perf_counter()
        fn(*args)
        runs.append(time.perf_counter() - start)
    return statistics.median(runs)


def dict_lookup(d, keys):
    total = 0
    for k in keys:
        total += d[k]
    return total


def constmap_lookup(m, keys):
    total = 0
    get = m.get
    for k in keys:
        total += get(k)
    return total


def build(sizes, rng):
    print("construction, ns per key (median of %d runs)\n" % REPEATS)
    print(f"{'n':>9} | {'dict':>9} {'ConstMap':>9} {'Verified':>9}")
    print("-" * 42)
    for n in sizes:
        keys = make_keys(n, rng)
        items = list(zip(keys, range(n)))
        d = dict(items)
        t_dict = time_it(dict, items)
        t_const = time_it(ConstMap, d)
        t_ver = time_it(VerifiedConstMap, d)
        print(f"{n:>9} | {t_dict / n * 1e9:9.1f} {t_const / n * 1e9:9.1f} "
              f"{t_ver / n * 1e9:9.1f}")


def lookup(sizes, rng):
    print("\nlookup of every key, ns per key. 'warm' keys have a cached hash,")
    print("'cold' keys are fresh str objects that have never been hashed.\n")
    print(f"{'n':>9} | {'dict warm':>10} {'dict cold':>10} | "
          f"{'ConstMap':>9} {'Verified':>9} | {'get_many':>9} {'many_into':>10}")
    print("-" * 78)
    for n in sizes:
        keys = make_keys(n, rng)
        d = {k: i for i, k in enumerate(keys)}
        cm, vm = ConstMap(d), VerifiedConstMap(d)
        probes = keys[:]
        rng.shuffle(probes)
        assert dict_lookup(d, probes) == constmap_lookup(cm, probes)
        assert cm.get_many(probes) == [d[k] for k in probes]

        warm = time_it(dict_lookup, d, probes)

        # cold: fresh copies each round, created outside the timed region
        cold_runs = []
        for _ in range(REPEATS):
            fresh = copy_keys(probes)
            start = time.perf_counter()
            dict_lookup(d, fresh)
            cold_runs.append(time.perf_counter() - start)
        cold = statistics.median(cold_runs)

        t_cm = time_it(constmap_lookup, cm, probes)
        t_vm = time_it(constmap_lookup, vm, probes)
        t_many = time_it(cm.get_many, probes)
        out = array("Q", bytes(8 * n))
        t_into = time_it(cm.get_many_into, probes, out)

        g = lambda t: t / n * 1e9
        print(f"{n:>9} | {g(warm):10.1f} {g(cold):10.1f} | {g(t_cm):9.1f} "
              f"{g(t_vm):9.1f} | {g(t_many):9.1f} {g(t_into):10.1f}")


def missing(n, rng):
    keys = make_keys(n, rng)
    absent = make_keys(n, rng)
    d = {k: i for i, k in enumerate(keys)}
    vm = VerifiedConstMap(d)
    assert all(k not in vm for k in absent)
    assert all(vm.get(k) is None for k in absent)
    out = array("Q", bytes(8 * n))
    vm.get_many_into(absent, out)
    assert all(v == NOT_FOUND for v in out)

    t_dict = time_it(lambda: [d.get(k) for k in absent])
    t_vm = time_it(lambda: [vm.get(k) for k in absent])
    print(f"\nlookup of {n} absent keys, ns per key")
    print(f"  dict.get:             {t_dict / n * 1e9:7.1f}")
    print(f"  VerifiedConstMap.get: {t_vm / n * 1e9:7.1f}")
    print("  (ConstMap cannot answer this: it returns an undefined value)")


def memory(sizes, rng):
    print("\nmemory, bytes per key\n")
    print(f"{'n':>9} | {'dict':>9} {'+ keys/values':>14} | "
          f"{'ConstMap':>9} {'Verified':>9}")
    print("-" * 60)
    for n in sizes:
        keys = make_keys(n, rng)
        d = {k: i for i, k in enumerate(keys)}
        table = sys.getsizeof(d)
        payload = sum(sys.getsizeof(k) for k in keys) + sum(
            sys.getsizeof(v) for v in d.values())
        cm, vm = ConstMap(d), VerifiedConstMap(d)
        print(f"{n:>9} | {table / n:9.1f} {(table + payload) / n:14.1f} | "
              f"{cm.nbytes() / n:9.1f} {vm.nbytes() / n:9.1f}")


if __name__ == "__main__":
    rng = random.Random(1234)
    sizes = (1_000, 10_000, 100_000, 1_000_000)
    build(sizes, rng)
    lookup(sizes, rng)
    missing(100_000, rng)
    memory(sizes, rng)
