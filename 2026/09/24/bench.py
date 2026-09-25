import time, sys

N = 10_000_000
buf = [None] * 1024
parts = [chr(97 + k) for k in range(16)]

def loop_only(n):
    b = buf
    for i in range(n):
        b[i & 1023] = None

def literal(n):
    b = buf
    for i in range(n):
        b[i & 1023] = ""

def ctor(n):
    b = buf
    for i in range(n):
        b[i & 1023] = str()

def from_int(n):
    b = buf
    for i in range(n):
        b[i & 1023] = str(i)

def concat(n):
    b = buf; p = parts
    for i in range(n):
        b[i & 1023] = p[i & 15] + "x"

cases = [("loop only", loop_only), ('""', literal), ("str()", ctor),
         ("str(i)", from_int), ("p[i&15] + 'x'", concat)]

print(f"Python {sys.version.split()[0]}")
for name, f in cases:
    best = float("inf")
    for _ in range(5):
        t0 = time.perf_counter_ns()
        f(N)
        best = min(best, time.perf_counter_ns() - t0)
    ns = best / N
    print(f"{name:16s} {ns:7.2f} ns/string  {1e3/ns:8.1f} M/s")
