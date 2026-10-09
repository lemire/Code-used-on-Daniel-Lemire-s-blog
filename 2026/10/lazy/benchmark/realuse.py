# Does the global lazy mode still help when the library is really used?
# Starts a local HTTP server, then times whole processes that use
# requests and numpy for real.
import http.server
import statistics
import subprocess
import sys
import threading
import time

PY = sys.argv[1]
RUNS = 41

class QuietHandler(http.server.SimpleHTTPRequestHandler):
    def log_message(self, *args):
        pass

server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), QuietHandler)
threading.Thread(target=server.serve_forever, daemon=True).start()
url = f"http://127.0.0.1:{server.server_address[1]}/realuse.py"

cases = [
    ("requests.get (real request)",
     f"import requests; r = requests.get({url!r}); assert r.status_code == 200"),
    ("rich.console.Console().print",
     "import rich.console; rich.console.Console().print('[bold]hello[/bold]')"),
    ("numpy array computation",
     "import numpy as np; a = np.arange(1000.0); print(float((a * a).sum()))"),
]
probe = "import sys; b = len(sys.modules); {code}; print(len(sys.modules) - b, file=sys.stderr)"
for label, code in cases:
    for mode in ["normal", "all"]:
        cmd = [PY, "-X", f"lazy_imports={mode}", "-c", code]
        r = subprocess.run([PY, "-X", f"lazy_imports={mode}", "-c", probe.format(code=code)],
                           capture_output=True, text=True)
        if r.returncode != 0:
            print(f"{label:30s} ({mode:6s}) FAILED: {r.stderr.strip().splitlines()[-1]}")
            continue
        nmods = r.stderr.strip().splitlines()[-1]
        for _ in range(3):
            subprocess.run(cmd, capture_output=True)
        times = []
        for _ in range(RUNS):
            t = time.perf_counter_ns()
            subprocess.run(cmd, capture_output=True)
            times.append((time.perf_counter_ns() - t) / 1e6)
        print(f"{label:30s} ({mode:6s}) median {statistics.median(times):6.1f} ms  {nmods:>4s} modules", flush=True)
