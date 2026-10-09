# Times whole-process runs: median and minimum wall-clock over many runs.
import statistics
import subprocess
import sys
import time

PY = sys.argv[1]
RUNS = 41
CSV = "data.csv"

with open(CSV, "w") as f:
    f.write("x,y\n")
    for i in range(1000):
        f.write(f"{i},{i * i}\n")

def bench(label, args):
    cmd = [PY] + args
    out = subprocess.run(cmd, capture_output=True, text=True)
    if out.returncode != 0:
        print(f"{label:52s} FAILED: {out.stderr.strip().splitlines()[-1]}")
        return
    for _ in range(3):
        subprocess.run(cmd, capture_output=True)
    times = []
    for _ in range(RUNS):
        t = time.perf_counter_ns()
        subprocess.run(cmd, capture_output=True)
        times.append((time.perf_counter_ns() - t) / 1e6)
    print(f"{label:52s} median {statistics.median(times):7.1f} ms  min {min(times):7.1f} ms", flush=True)

bench("baseline: python -c pass", ["-c", "pass"])
for mode in ["normal", "all"]:
    x = ["-X", f"lazy_imports={mode}"]
    bench(f"tool_eager.py --version ({mode})", x + ["tool_eager.py", "--version"])
    bench(f"tool_lazy.py --version ({mode})", x + ["tool_lazy.py", "--version"])
    bench(f"tool_eager.py mean ({mode})", x + ["tool_eager.py", "mean", CSV])
    bench(f"tool_lazy.py mean ({mode})", x + ["tool_lazy.py", "mean", CSV])
    bench(f"tool_eager.py stats ({mode})", x + ["tool_eager.py", "stats", CSV])
    bench(f"tool_lazy.py stats ({mode})", x + ["tool_lazy.py", "stats", CSV])
    # import a library and touch one name so that the import must happen
    for m, attr in [("json", "dumps"), ("asyncio", "run"), ("numpy", "zeros"),
                    ("pandas", "DataFrame"), ("requests", "get"), ("rich.console", "Console")]:
        bench(f"import {m}; {m}.{attr} ({mode})", x + ["-c", f"import {m}; {m}.{attr}"])
