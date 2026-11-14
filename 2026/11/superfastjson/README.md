# Reading JSON at 277 GB/s in Python

Code and data for the post on fastsimdjson and free-threaded Python.

- `bench.py`: counts the stars of each repository in GH Archive events (2024-03-13, 12:00 to 19:59 UTC, 7.6 GB), with json, orjson and msgspec on a pool of processes (or threads), and fastsimdjson on threads.
- `queries.py`: six queries over the six files of the Pison benchmark (github.com/AutomataLab/Pison), one record per line, with orjson and msgspec on processes and fastsimdjson on threads. The libraries are checked against each other (values found, checksum).
- `setup.sh`: two virtual environments with uv (Python 3.14 with the GIL, and free-threaded 3.14t), and the GH Archive data.
- `run.sh`, `run_queries.sh`: the sweeps over the number of workers.
- `aws.sh`: runs both sweeps on an AWS instance, then terminates it.
- `results_c8a.txt`, `results_c8a_queries.txt`: raw output, c8a.metal-48xl (2 x AMD EPYC 9R45, 192 cores), Python 3.14.10, orjson 3.13.0, msgspec 0.22.0, data on a tmpfs spread over both memory nodes.
- `plot.py`, `featured.py`: the figures (need matplotlib, Pillow and cwebp).

The fastsimdjson features used here (`fields=` with On-Demand, `drop_none=`, files mapped with `mmap.ACCESS_READ` read in place) are in its development version: until they are released, pass a source tree (`FSJ_SRC=`, or `FSJ=` with a pip requirement) to `setup.sh` or `aws.sh`.

```
bash setup.sh                      # venvs and /dev/shm/events.ndjson
bash run.sh /dev/shm/events.ndjson 1 16 32 64
bash run_queries.sh path/to/pison/files 1 16 32 64
python plot.py results_c8a.txt stars "AMD EPYC 9R45, 192 cores"
python plot.py queries results_c8a_queries.txt queries "AMD EPYC 9R45, 192 cores"
```
