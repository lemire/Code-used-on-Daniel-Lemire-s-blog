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
