#!/bin/sh
# Creates a virtual environment, installs the packages from PyPI and runs the
# benchmark on one core. Usage: [PYTHON=python3.14] ./run.sh [core]
set -eu
"${PYTHON:-python3}" -m venv venv
./venv/bin/pip install -q -r requirements.txt
taskset -c "${1:-2}" ./venv/bin/python bench.py > results.csv
