```
cmake -B build
./build/benchmark
```


# Results on Ice Lake processor (GCC12)

```
$ ./build/benchmark 
memchr                                             :  0.010 ns  95.42 GB/s   2.53 GHz   0.03 c   0.09 i   2.83 i/c 
std::find                                          :  0.235 ns   4.25 GB/s   3.19 GHz   0.75 c   2.75 i   3.66 i/c 
simdutf::find                                      :  0.007 ns  146.78 GB/s   3.09 GHz   0.02 c   0.07 i   3.40 i/c 
naive_find                                         :  0.543 ns   1.84 GB/s   3.19 GHz   1.73 c   5.00 i   2.89 i/c 
memchr                                             :  0.011 ns  95.11 GB/s   3.19 GHz   0.03 c   0.09 i   2.80 i/c 
std::find                                          :  0.235 ns   4.25 GB/s   3.19 GHz   0.75 c   2.75 i   3.67 i/c 
simdutf::find                                      :  0.007 ns  146.14 GB/s   3.09 GHz   0.02 c   0.07 i   3.33 i/c 
naive_find                                         :  0.542 ns   1.84 GB/s   3.19 GHz   1.73 c   5.00 i   2.89 i/c 
```

| Input Size (bytes) | std::find (GB/s) | simdutf::find (GB/s) | naive_find (GB/s) | memchr (GB/s) |
|--------------------|------------------|-----------------------|-------------------|----------------|
| 65536              | 4.25             | 146.78                | 1.84              | 95.42             |  
| 524288             | 4.25             | 146.14                | 1.84              | 95.11             |  

# Results on Apple M4 (Apple LLVM 17)

```
std::find                                          :  0.015 ns  68.00 GB/s   4.28 GHz   0.06 c   0.44 i   6.86 i/c 
simdutf::find                                      :  0.008 ns  118.85 GB/s   4.07 GHz   0.04 c   0.27 i   7.57 i/c 
naive_find                                         :  0.224 ns   4.47 GB/s   4.30 GHz   1.00 c   6.00 i   5.99 i/c 
memchr                                             :  0.015 ns  65.88 GB/s   4.09 GHz   0.06 c   0.44 i   6.94 i/c 
std::find                                          :  0.015 ns  66.21 GB/s   4.06 GHz   0.06 c   0.44 i   6.96 i/c 
simdutf::find                                      :  0.009 ns  112.38 GB/s   3.96 GHz   0.04 c   0.27 i   7.35 i/c 
naive_find                                         :  0.223 ns   4.48 GB/s   4.47 GHz   1.00 c   6.00 i   5.99 i/c 
````

| Input Size (bytes) | std::find (GB/s) | simdutf::find (GB/s) | naive_find (GB/s) | memchr (GB/s) |
|--------------------|------------------|-----------------------|-------------------|----------------|
| 65536              | 68.00            | 118.85                | 4.47              | 67.15             |  
| 524288             | 66.21            | 112.38                | 4.48              | 65.88             |  
