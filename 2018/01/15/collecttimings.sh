clang -O3 -o timings timings.c -march=native  && ./timings | tail -n +4 |  cut -f2 -d " " | sort -n | uniq -c > timings.txt
