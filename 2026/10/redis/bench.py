"""Benchmark Redis bitmaps: plain bitsets (Redis strings), native Roaring
bitmaps (redis/redis#15331) and the redis-roaring module (R.* commands).

Times are server-side (INFO commandstats usec_per_call), so they exclude the
network and the Python client.

Usage: python bench.py SOCKET OUT.json MODES [DATASET...]
       MODES is a comma-separated subset of bitset,roaring,module
"""
import json
import sys
import time

import numpy as np
import redis

SOCK = sys.argv[1] if len(sys.argv) > 1 else "/tmp/redisroar.sock"
r = redis.Redis(unix_socket_path=SOCK)
rng = np.random.default_rng(42)
U = 100_000_000  # universe for the "user id" datasets

# Per mode: the command names to use (None: the mode has no equivalent).
COMMANDS = {
    "bitset": dict(setbit="SETBIT", getbit="GETBIT", bitcount="BITCOUNT",
                   bitcount_range="BITCOUNT", bitpos="BITPOS",
                   bitfield_ro="BITFIELD_RO", bitop="BITOP"),
    "roaring": dict(setbit="SETBIT", getbit="GETBIT", bitcount="BITCOUNT",
                    bitcount_range="BITCOUNT", bitpos="BITPOS",
                    bitfield_ro="BITFIELD_RO", bitop="BITOP"),
    # R.BITCOUNT and R.BITPOS take no range; there is no R.BITFIELD.
    "module": dict(setbit="R.SETBIT", getbit="R.GETBIT", bitcount="R.BITCOUNT",
                   bitcount_range=None, bitpos=None, bitfield_ro=None,
                   bitop="R.BITOP"),
}
EXPECTED_TYPE = {"bitset": "string", "roaring": "bitmap", "module": "reroaring"}


def roaring_mode(on):
    r.config_set("bitmap-default-roaring", "yes" if on else "no")


def ids_random(n, universe):
    return np.sort(rng.choice(universe, size=n, replace=False))


def ids_runs(nruns, runlen, universe):
    step = universe // nruns
    return np.concatenate([np.arange(k * step, k * step + runlen) for k in range(nruns)])


def to_bytes(ids):
    """Redis bit order: bit 0 is the most significant bit of byte 0."""
    bits = np.zeros(int(ids[-1]) + 1, dtype=bool)
    bits[ids] = True
    return np.packbits(bits, bitorder="big").tobytes()


def cmdstat(name):
    """usec_per_call for a command name, as reported by INFO commandstats."""
    stats = r.info("commandstats")
    for k, v in stats.items():
        if k.lower() == "cmdstat_" + name.lower():
            return v["usec_per_call"]
    raise KeyError(name)


def load(key, ids, mode, via_setbit):
    """Create the bitmap 'key' holding 'ids'. Returns the server-side
    usec per SETBIT when built with SETBIT, else None."""
    if mode is True or mode is False:  # old call style (perfprobe.py)
        mode = "roaring" if mode else "bitset"
    r.delete(key)
    roaring_mode(mode == "roaring")
    build = None
    if via_setbit:
        # random insertion order, like real traffic
        cmd = COMMANDS[mode]["setbit"]
        order = ids.copy()
        rng.shuffle(order)
        r.config_resetstat()
        p = r.pipeline(transaction=False)
        for i, x in enumerate(order):
            p.execute_command(cmd, key, int(x), 1)
            if i % 10000 == 9999:
                p.execute()
        p.execute()
        build = cmdstat(cmd)
    elif mode == "module":
        for i in range(0, len(ids), 100_000):
            r.execute_command("R.APPENDINTARRAY", key, *ids[i:i + 100_000].tolist())
    else:
        roaring_mode(False)
        r.set(key, to_bytes(ids))
        if mode == "roaring":
            roaring_mode(True)
            r.setbit(key, int(ids[0]), 1)  # converts the string in place
    if mode == "module":
        r.execute_command("R.OPTIMIZE", key)  # use run containers when smaller
    roaring_mode(mode == "roaring")
    t = r.type(key).decode()
    assert t == EXPECTED_TYPE[mode], (key, t)
    return build


def timed(calls, budget=0.5):
    """calls: list of argument tuples for one command. Runs them pipelined,
    returns the server-side usec/call."""
    t = time.perf_counter()
    r.execute_command(*calls[0])
    one = time.perf_counter() - t
    reps = int(min(len(calls), max(5, budget / max(one, 1e-6))))
    r.config_resetstat()
    p = r.pipeline(transaction=False)
    for i, c in enumerate(calls[:reps]):
        p.execute_command(*c)
        if i % 1000 == 999:
            p.execute()
    p.execute()
    return cmdstat(calls[0][0])


def bench_mode(mode, name, ids_a, ids_b, via_setbit, nbytes, absent, probes):
    c = COMMANDS[mode]
    a, b, d = f"{mode}:{name}:a", f"{mode}:{name}:b", f"{mode}:{name}:dst"
    out = {}
    build = load(a, ids_a, mode, via_setbit)
    load(b, ids_b, mode, via_setbit)
    if build is not None:
        out["SETBIT (build)"] = build
    out["memory (bytes)"] = r.memory_usage(a)
    out["GETBIT"] = timed([(c["getbit"], a, int(x)) for x in probes])
    # set then clear absent bits so the bitmap does not change
    sc = []
    for x in absent[:5000]:
        if not r.execute_command(c["getbit"], a, int(x)):
            sc += [(c["setbit"], a, int(x), 1), (c["setbit"], a, int(x), 0)]
    # run every pair (no early cut-off) so each set bit is cleared again
    out["SETBIT (update)"] = timed(sc, budget=float("inf"))
    out["BITCOUNT"] = timed([(c["bitcount"], a)] * 20000)
    if c["bitcount_range"]:
        out["BITCOUNT range"] = timed(
            [(c["bitcount_range"], a, nbytes // 4, nbytes // 4 + nbytes // 10)] * 20000)
    if c["bitpos"]:
        out["BITPOS"] = timed([(c["bitpos"], a, 1, int(x) // 8) for x in probes])
    if c["bitfield_ro"]:
        out["BITFIELD_RO GET u16"] = timed(
            [(c["bitfield_ro"], a, "GET", "u16", int(x)) for x in probes])
    for op in ("AND", "OR", "XOR"):
        out[f"BITOP {op}"] = timed([(c["bitop"], op, d, a, b)] * 20000)
    r.execute_command(c["bitop"], "AND", d, a, b)
    check = [r.execute_command(c["bitcount"], k) for k in (a, b, d)]
    r.delete(a, b, d)
    return out, check


def bench_dataset(name, ids_a, ids_b, via_setbit, modes):
    maxoff = int(max(ids_a[-1], ids_b[-1]))
    nbytes = maxoff // 8 + 1
    absent = rng.integers(0, maxoff, size=20000)
    probes = rng.integers(0, maxoff, size=20000)
    res, checks = {}, {}
    for mode in modes:
        res[mode], checks[mode] = bench_mode(
            mode, name, ids_a, ids_b, via_setbit, nbytes, absent, probes)
        print(f"  {mode} done", file=sys.stderr, flush=True)
    assert len({tuple(v) for v in checks.values()}) == 1, checks
    return res, checks[modes[0]]


DATASETS = [
    # name, description, generator, load via SETBIT?
    ("dau_0.1pct", "0.1% of 100M user ids, random",
     lambda: ids_random(U // 1000, U), True),
    ("dau_1pct", "1% of 100M user ids, random",
     lambda: ids_random(U // 100, U), True),
    ("dau_10pct", "10% of 100M user ids, random",
     lambda: ids_random(U // 10, U), False),
    ("dense_50pct", "50% of 100M user ids, random",
     lambda: ids_random(U // 2, U), False),
    ("hashed_2e32", "100k random 32-bit ids",
     lambda: ids_random(100_000, 2**32 - 8), True),
    ("runs", "10 runs of 1M ids in 100M",
     lambda: ids_runs(10, 1_000_000, U), False),
    # appended so that the datasets above keep their seeds
    ("dau_20pct", "20% of 100M user ids, random",
     lambda: ids_random(U // 5, U), False),
]

if __name__ == "__main__":
    out_json = sys.argv[2]
    modes = sys.argv[3].split(",")
    only = sys.argv[4:]
    all_results = {}
    for k, (name, desc, gen, via_setbit) in enumerate(DATASETS):
        if only and name not in only:
            continue
        # seed per dataset so every run and every mode sees the same bitmaps
        rng = np.random.default_rng(1000 + k)
        ids_a, ids_b = gen(), gen()
        print(f"{name}: {desc}", file=sys.stderr, flush=True)
        res, check = bench_dataset(name, ids_a, ids_b, via_setbit, modes)
        all_results[name] = dict(description=desc, bitcounts=check, results=res)
        with open(out_json, "w") as f:
            json.dump(all_results, f, indent=1)
        print(f"\n## {name}: {desc}  (bitcounts a, b, a AND b = {check})")
        print("| operation | " + " | ".join(modes) + " |")
        print("| --- |" + " ---: |" * len(modes))
        ops = list(dict.fromkeys(k for m in modes for k in res[m]))
        for k in ops:
            unit = "" if "memory" in k else " us"
            cells = [f"{res[m][k]:,.2f}{unit}" if k in res[m] else "N/A" for m in modes]
            print(f"| {k} | " + " | ".join(cells) + " |")
        sys.stdout.flush()
