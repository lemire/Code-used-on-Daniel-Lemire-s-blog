gcc -O3 -o paramvisit paramvisit.c

for n in 100 1000 10000 100000 1000000 10000000 100000000 
do
  echo $n
  perf stat -e branch-misses   ./paramvisit -f -n $n 2>&1 | grep branch-misses
done
