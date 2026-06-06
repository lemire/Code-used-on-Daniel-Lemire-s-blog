#!/usr/bin/env bash
#
# run_benchmarks.sh
#
# Pull the latest version of the RoaringBitmap/roaring Go library and run its
# benchmark suite under each of the four amd64 microarchitecture levels
# (GOAMD64=v1, v2, v3, v4). Raw results and a benchstat comparison are written
# to ./results/ along with the environment details needed to reproduce them.
#
# Usage:
#   ./run_benchmarks.sh            # default: -count=10, -benchtime=1s
#   COUNT=5 BENCHTIME=2s ./run_benchmarks.sh
#
set -euo pipefail

# ---- configuration ---------------------------------------------------------
MODULE="github.com/RoaringBitmap/roaring/v2"
COUNT="${COUNT:-8}"
BENCHTIME="${BENCHTIME:-500ms}"
# Default to the compute-heavy, microarchitecture-sensitive benchmarks: popcount,
# trailing-zero counts, checksums, cardinality, and the boolean set operations.
# These are the ones where the amd64 level could plausibly matter. Pass
# BENCHFILTER=. to run the whole suite (much slower).
BENCHFILTER="${BENCHFILTER:-Popcount|CountTrailingZeros|OrigNumberOfTrailingZeros|Checksum|CountRoaring|CountBitset|CardinalityInRange|IntersectionRoaring\$|IntersectionCardinalityRoaring|IntersectionLargeRoaring|UnionRoaring|Ors|FastOrRunContainers|AndNot|Xor|AndAny|FromDense|FromBitmap16|MarshalBinary}"
LEVELS=(v1 v2 v3 v4)
OUTDIR="results"

cd "$(dirname "$0")"
mkdir -p "$OUTDIR"

# ---- local module ----------------------------------------------------------
# We keep everything in this folder; a throwaway module pins the dependency so
# `go test` can build and run the library's own benchmarks from the module cache.
if [ ! -f go.mod ]; then
    go mod init bench
fi
echo "Fetching latest ${MODULE} ..."
# -t pulls the test dependencies too (testify et al.) so the library's own
# *_test.go benchmark files build. We deliberately do NOT run `go mod tidy`:
# nothing in this throwaway module imports the library, so tidy would prune it.
go get -t "${MODULE}@latest"

VERSION="$(go list -m "${MODULE}" | awk '{print $2}')"
echo "Using ${MODULE} ${VERSION}"

# ---- environment capture ---------------------------------------------------
{
    echo "date:        $(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo "library:     ${MODULE} ${VERSION}"
    echo "go:          $(go version)"
    echo "count:       ${COUNT}"
    echo "benchtime:   ${BENCHTIME}"
    echo
    echo "## CPU"
    if command -v lscpu >/dev/null 2>&1; then
        lscpu | grep -E "Model name|Architecture|^CPU\(s\)|Thread|Core|Socket|Flags" || true
    else
        grep -m1 "model name" /proc/cpuinfo || true
    fi
} > "${OUTDIR}/environment.txt"
cat "${OUTDIR}/environment.txt"

# ---- benchmark each amd64 level -------------------------------------------
for lvl in "${LEVELS[@]}"; do
    echo
    echo "===> GOAMD64=${lvl}"
    GOAMD64="${lvl}" go test -run='^$' -bench="${BENCHFILTER}" -benchmem \
        -benchtime="${BENCHTIME}" -count="${COUNT}" -timeout=120m \
        "${MODULE}" | tee "${OUTDIR}/bench-${lvl}.txt"
done

# ---- comparison ------------------------------------------------------------
if command -v benchstat >/dev/null 2>&1; then
    BENCHSTAT=benchstat
elif [ -x "$(go env GOPATH)/bin/benchstat" ]; then
    BENCHSTAT="$(go env GOPATH)/bin/benchstat"
else
    echo "benchstat not found; installing ..."
    go install golang.org/x/perf/cmd/benchstat@latest
    BENCHSTAT="$(go env GOPATH)/bin/benchstat"
fi

echo
echo "===> benchstat comparison (v1 is the baseline)"
"$BENCHSTAT" \
    v1="${OUTDIR}/bench-v1.txt" \
    v2="${OUTDIR}/bench-v2.txt" \
    v3="${OUTDIR}/bench-v3.txt" \
    v4="${OUTDIR}/bench-v4.txt" | tee "${OUTDIR}/benchstat.txt"

echo
echo "Done. See ${OUTDIR}/ for raw output and ${OUTDIR}/benchstat.txt for the comparison."
