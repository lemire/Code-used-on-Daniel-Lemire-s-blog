#!/bin/bash
# Two virtual environments (needs uv): Python with the GIL, and free-threaded
# Python. Then the data: eight hours of GH Archive (2024-03-13, 12:00 to
# 19:59 UTC), 7.6 GB, in /dev/shm.
set -e
PY=${PY:-3.14}
uv venv -q --clear --python "$PY" venv-gil
uv venv -q --clear --python "${PY}t" venv-nogil
uv pip install -q --python venv-gil/bin/python fastsimdjson==1.0.0 orjson msgspec
# orjson does not support free-threaded Python
uv pip install -q --python venv-nogil/bin/python "${FSJ:-fastsimdjson}" msgspec
DATA=${DATA:-/dev/shm/events.ndjson}
if [ ! -s "$DATA" ]; then
  for h in $(seq 12 19); do
    curl -sfL "https://data.gharchive.org/2024-03-13-$h.json.gz" | gzip -dc
  done > "$DATA"
fi
ls -l "$DATA"
