We are assuming an aarch64 system.

Usage:
```
cmake -B build
cmake --build build
./build/benchmark
```

Potential result:
```
Normal non-zero: 49152
Subnormal: 8191
Zero: 1
Infinite: 1
QNaN: 4096
SNaN: 4095
SVE registers 16 bytes
is_float_safe: 1
veq_non_zero_float_safe: 0
# check: 5263 5263 5263 5263 5263
# check: 5263
# check: 1415 1415 1415 1415 1415
# check: 1415
Trial 1
veq_non_zero_max                         :   2.93 GB/s  183.4 Ma/s   5.45 ns/d 
veq_non_zero_mov                         :   2.76 GB/s  172.2 Ma/s   5.81 ns/d 
veq_non_zero_narrow                      :   2.76 GB/s  172.2 Ma/s   5.81 ns/d 
sve_check_all_zeros                      :   3.24 GB/s  202.7 Ma/s   4.93 ns/d 
veq_non_zero_float                       :   3.02 GB/s  189.0 Ma/s   5.29 ns/d 
veq_non_zero_float_safe                  :   3.02 GB/s  189.0 Ma/s   5.29 ns/d 
Trial 2
veq_non_zero_max                         :   2.94 GB/s  183.5 Ma/s   5.45 ns/d 
veq_non_zero_mov                         :   2.76 GB/s  172.2 Ma/s   5.81 ns/d 
veq_non_zero_narrow                      :   2.76 GB/s  172.2 Ma/s   5.81 ns/d 
sve_check_all_zeros                      :   3.24 GB/s  202.7 Ma/s   4.93 ns/d 
veq_non_zero_float                       :   3.02 GB/s  189.0 Ma/s   5.29 ns/d 
veq_non_zero_float_safe                  :   3.02 GB/s  189.0 Ma/s   5.29 ns/d 
Trial 3
veq_non_zero_max                         :   2.94 GB/s  183.5 Ma/s   5.45 ns/d 
veq_non_zero_mov                         :   2.76 GB/s  172.2 Ma/s   5.81 ns/d 
veq_non_zero_narrow                      :   2.75 GB/s  172.2 Ma/s   5.81 ns/d 
sve_check_all_zeros                      :   3.24 GB/s  202.7 Ma/s   4.93 ns/d 
veq_non_zero_float                       :   3.02 GB/s  189.0 Ma/s   5.29 ns/d 
veq_non_zero_float_safe                  :   3.02 GB/s  189.0 Ma/s   5.29 ns/d
```
