gcc -O2 -o fun fun.c -march=native -mno-avx512f 

objdump -d ./fun |egrep "(ymm|zmm)"

perf stat ./fun a b 2>&1 |egrep "(GHz|msec)" 

perf stat ./fun a 2>&1 |egrep "(GHz|msec)" 

perf stat ./fun  2>&1 |egrep "(GHz|msec)"

rm -f fun

echo "more"

gcc -O2 -o morefun morefun.c -march=native -mno-avx512f

perf stat ./morefun 13 b 2>&1 |egrep "(GHz|msec)"

perf stat ./morefun 13 2>&1 |egrep "(GHz|msec)"

rm -f morefun

