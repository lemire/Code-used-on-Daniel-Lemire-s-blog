#!/bin/bash
# Full build with GNU ld, then with mold 3.0.0; then repeated relinks of out/Release/node.
set -e
MOLD=$HOME/opt/mold-3.0.0-x86_64-linux/bin/mold
cd ~/moldnode
rm -rf node-ld node-mold
cp -a node node-ld; cp -a node node-mold
log=~/moldnode/results_big4.txt
: > $log
echo "gcc: $(gcc --version | head -1); ld: $(ld --version | head -1); mold: $($MOLD --version)" >> $log
echo "node commit: $(git -C node rev-parse HEAD)" >> $log

full() { # dir wrapper...
  d=$1; shift
  cd ~/moldnode/$d
  ./configure > configure.log 2>&1
  s=$(date +%s.%N); "$@" make -j128 > build.log 2>&1; e=$(date +%s.%N)
  echo "full build $d: $(echo "$e - $s" | bc) s" >> $log
}
relink() { # dir wrapper...
  d=$1; shift
  cd ~/moldnode/$d
  for i in 1 2 3 4 5 6; do
    rm -f out/Release/node out/Release/obj.target/node 2>/dev/null || true
    sync; s=$(date +%s.%N); "$@" make -j128 > relink$i.log 2>&1; e=$(date +%s.%N)
    echo "relink $d run $i: $(echo "$e - $s" | bc) s" >> $log
  done
  echo "$d .comment: $(readelf -p .comment out/Release/node | grep -io 'mold[^ ]* [0-9.]*' | head -1)" >> $log
  echo "$d size: $(stat -c %s out/Release/node)" >> $log
}
full node-ld env
full node-mold $MOLD -run
relink node-ld env
relink node-mold $MOLD -run
echo DONE >> $log
