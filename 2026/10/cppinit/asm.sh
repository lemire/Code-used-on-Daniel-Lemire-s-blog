# usage: asm.sh file.cpp  (prints walk() for GCC 14/16, = default vs {})
f=$1
for c in 14 16; do
  if [ $c = 16 ]; then source ~/usegcc16.sh >/dev/null; CXX=g++; else CXX=/usr/bin/c++; fi
  for d in "" -DBRACES; do
    echo "=== gcc$c $d"
    $CXX -O2 -march=x86-64 -S -masm=intel $d $f -o - | sed -n '/^"*_Z4walkPKci"*:/,/\.size/p' | grep -v '^\s*\.cfi\|^\.L[A-Z]\|p2align'
  done
done
