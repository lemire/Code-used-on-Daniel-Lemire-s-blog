time python3 ./script.py wealth.json

cmake -B build -D CPM_SOURCE_CACHE=.cache && cmake --build build 

time  ./build/main wealth.json


hyperfine  --runs 1000 "python3 script.py wealth.json" --warmup 100 

hyperfine  --runs 1000 "./build/main wealth.json" --warmup 100 

3.7 ms ±   4.7 ms 