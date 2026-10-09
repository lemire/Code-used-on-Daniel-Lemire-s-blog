# How many modules end up in sys.modules for each run?
import subprocess
import sys

PY = sys.argv[1]
probe = "import sys, runpy; sys.argv = {argv!r}; runpy.run_path(sys.argv[0], run_name='__main__'); print(len(sys.modules), file=sys.stderr)"
for mode in ["normal", "all"]:
    for script in ["tool_eager.py", "tool_lazy.py"]:
        for argv in [[script, "--version"], [script, "mean", "data.csv"], [script, "stats", "data.csv"]]:
            r = subprocess.run([PY, "-X", f"lazy_imports={mode}", "-c", probe.format(argv=argv)],
                               capture_output=True, text=True)
            print(f"{' '.join(argv):32s} ({mode}): {r.stderr.strip().splitlines()[-1]} modules")
