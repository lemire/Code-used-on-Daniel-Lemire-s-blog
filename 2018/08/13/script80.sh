set -x

cat /proc/cpuinfo  | grep 'name'| uniq
gcc --version
gcc -O3 -o bubblenoavx512 bubble.c -march=native -mno-avx512dq  -mno-avx512f -lpthread
gcc -O3 -o bubble bubble.c -march=native -lpthread
for num in `seq 1 1 5`; do time ./bubblenoavx512 80; done
for num in `seq 1 1 5`; do time ./bubble 80; done
