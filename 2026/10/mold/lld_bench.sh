#!/bin/bash
# Time the bare link of the node executable with LLVM lld 21.1.8 (Fedora/EL10 RPM
# extracted into ~/opt, no root), with mold as a control. Run from node-ld after
# link_bench.sh has produced linkcmd.sh.
M=$HOME/opt/mold-3.0.0-x86_64-linux/bin
L=$HOME/opt/lld-21.1.8
export LD_LIBRARY_PATH=$L/usr/lib64/llvm21/lib64
for i in 1 2 3 4 5 6; do
  for l in mold lld lld-threads1 lld-threads8; do
    case $l in
      mold) extra="-B$M -fuse-ld=mold" ;;
      lld) extra="-B$L/b -fuse-ld=lld" ;;
      lld-threads*) extra="-B$L/b -fuse-ld=lld -Wl,--threads=${l#lld-threads}" ;;
    esac
    s=$(date +%s.%N); OUT=/tmp/node_$l eval "$(cat linkcmd.sh) $extra" > /dev/null; e=$(date +%s.%N)
    echo "$l $(echo "$e - $s" | bc)"
  done
done > results_big4_purelink_lld.txt
