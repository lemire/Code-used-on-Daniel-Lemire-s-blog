"""Python sets and dictionaries can have quadratic-time performance.

CPython hashes an integer to itself, modulo the Mersenne prime 2**61-1.
Hence every multiple of 2**61-1 hashes to zero. A set built out of such
integers degenerates: each insertion and each lookup walks the whole
collision chain, comparing values one by one. No subclassing, no custom
__hash__: just ordinary Python integers.
"""
import statistics
import time

M = (1 << 61) - 1  # CPython's modulus for integer hashing


class Point:
    """A plausible-looking class with a lazy __hash__."""
    __slots__ = ("x", "y")

    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return 0  # legal, and quietly quadratic


def bench(values, trials=3):
    builds, checks = [], []
    for _ in range(trials):
        start = time.perf_counter()
        s = set(values)
        builds.append(time.perf_counter() - start)

        start = time.perf_counter()
        count = sum(v in s for v in values)
        checks.append(time.perf_counter() - start)
        assert count == len(values)
    return statistics.median(builds), statistics.median(checks)


def cases(n):
    return [
        ("consecutive ints", [i for i in range(n)]),
        ("scattered ints", [i * 2654435761 for i in range(n)]),
        ("same low 20 bits", [i << 20 for i in range(n)]),
        ("multiples of 2**61-1", [i * M for i in range(1, n + 1)]),
        ("Point, hash() == 0", [Point(i, i) for i in range(n)]),
    ]


if __name__ == "__main__":
    print(f"{'n':>6}  {'case':22s} {'build (ms)':>12} {'check (ms)':>12}")
    for n in (1000, 2000, 4000, 8000, 16000):
        for name, values in cases(n):
            build, check = bench(values)
            print(f"{n:>6}  {name:22s} {build*1e3:12.2f} {check*1e3:12.2f}")
        print()
