#!/bin/bash
# Time the bare link of the node executable with bfd, gold and mold.
# Run from a built Node.js tree (after run.sh). linkcmd.sh holds the link
# command captured with `make V=1`, with the output path replaced by $OUT.
M=$HOME/opt/mold-3.0.0-x86_64-linux/bin
rm -f out/Release/node && make -j128 V=1 > v1.log 2>&1
grep -E "^ *g\+\+ -o [^ ]*/out/Release/node " v1.log | sed "s/^ *//" \
  | sed "s|-o [^ ]*/out/Release/node |-o \$OUT |" > linkcmd.sh
for i in 1 2 3 4 5 6; do
  for l in bfd gold mold; do
    extra="-fuse-ld=$l"; [ $l = mold ] && extra="-B$M -fuse-ld=mold"
    s=$(date +%s.%N); OUT=/tmp/node_$l eval "$(cat linkcmd.sh) $extra" > /dev/null; e=$(date +%s.%N)
    echo "$l $(echo "$e - $s" | bc)"
  done
done > results_big4_purelink.txt
for t in 1 8; do
  for i in 1 2 3 4 5 6; do
    s=$(date +%s.%N); OUT=/tmp/node_m$t eval "$(cat linkcmd.sh) -B$M -fuse-ld=mold -Wl,--threads=$t" > /dev/null; e=$(date +%s.%N)
    echo "mold-threads$t $(echo "$e - $s" | bc)"
  done
done > results_big4_purelink_threads.txt
