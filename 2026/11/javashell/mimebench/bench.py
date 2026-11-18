import os, platform, sys, time

exp_len, exp_sum = map(int, open("expected.txt").read().split())
data = open("mime.txt", "rb").read()

cands = []
try:
    import fastpybase64
    cands.append((f"fastpybase64 {fastpybase64.__version__}{os.environ.get('FASTPYLABEL', '')} ({fastpybase64.get_implementation()}) b64decode", fastpybase64.b64decode))
except ImportError:
    print("fastpybase64 not installed")

ver = f"python {platform.python_version()}"
for label, f in cands:
    out = f(data)
    if len(out) != exp_len or sum(out) != exp_sum:
        sys.exit(f"bad decode {label}")
    iters = 200
    for _ in range(3 * iters):
        f(data)
    best = float("inf")
    for _ in range(10):
        st = time.perf_counter_ns()
        for _ in range(iters):
            f(data)
        best = min(best, (time.perf_counter_ns() - st) / iters)
    print(f"{ver} mime {label}: {len(data) / best:.2f} GB/s")
