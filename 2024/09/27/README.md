Usage:

First build the C++ program.

```
cmake -B build -D CPM_SOURCE_CACHE=.cache && cmake --build build 
````


Then run...
```
hyperfine  --runs 1000 "python3 script.py wealth.json" --warmup 100 
````

and

```
hyperfine  --runs 1000 "./build/main wealth.json" --warmup 100 
```