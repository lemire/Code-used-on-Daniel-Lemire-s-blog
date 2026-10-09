# Does a lazy import slow down later accesses to the module?
import sys
import time
if sys.argv[1] == "lazy":
    lazy import math
else:
    import math

N = 10_000_000

def loop():
    s = 0.0
    for i in range(N):
        s += math.sqrt(i)
    return s

t0 = time.perf_counter_ns()
math.pi  # first access: reifies the lazy import
t1 = time.perf_counter_ns()
best = float("inf")
for _ in range(5):
    t2 = time.perf_counter_ns()
    loop()
    best = min(best, time.perf_counter_ns() - t2)
print(f"{sys.argv[1]}: first access {t1 - t0} ns, {best / N:.2f} ns per call")
