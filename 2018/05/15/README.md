See library at https://github.com/lemire/fastvalidate-utf-8

On skylake processor:

```
$ make
cc -O3 -o checkutf8 checkutf8.c -march=native
$ ./checkutf8
string size = 64
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.312 cycles per operation (best)     8.794 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  1.000 cycles per operation (best)     1.869 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.156 cycles per operation (best)     0.356 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.188 cycles per operation (best)     0.550 cycles per operation (avg)


string size = 128
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.406 cycles per operation (best)     8.422 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.875 cycles per operation (best)     1.634 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.109 cycles per operation (best)     0.184 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.125 cycles per operation (best)     0.234 cycles per operation (avg)


string size = 256
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.367 cycles per operation (best)     8.430 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.820 cycles per operation (best)     0.916 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.094 cycles per operation (best)     0.241 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.117 cycles per operation (best)     0.281 cycles per operation (avg)


string size = 512
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.406 cycles per operation (best)     8.434 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.766 cycles per operation (best)     0.790 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.121 cycles per operation (best)     0.142 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.160 cycles per operation (best)     0.186 cycles per operation (avg)


string size = 1024
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.410 cycles per operation (best)     9.030 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.754 cycles per operation (best)     0.778 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.096 cycles per operation (best)     0.107 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.131 cycles per operation (best)     0.152 cycles per operation (avg)


string size = 2048
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.446 cycles per operation (best)     8.457 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.757 cycles per operation (best)     0.765 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.098 cycles per operation (best)     0.102 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.126 cycles per operation (best)     0.132 cycles per operation (avg)


string size = 4096
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.435 cycles per operation (best)     8.453 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.750 cycles per operation (best)     0.754 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.084 cycles per operation (best)     0.091 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.113 cycles per operation (best)     0.114 cycles per operation (avg)


string size = 8192
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.431 cycles per operation (best)     8.441 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.743 cycles per operation (best)     0.746 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.083 cycles per operation (best)     0.086 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.105 cycles per operation (best)     0.110 cycles per operation (avg)


string size = 16384
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.438 cycles per operation (best)     8.442 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.741 cycles per operation (best)     0.744 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.083 cycles per operation (best)     0.084 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.103 cycles per operation (best)     0.103 cycles per operation (avg)


string size = 32768
We are feeding ascii so it is always going to be ok.
validate_utf8(data, N)                                          :  8.439 cycles per operation (best)     8.442 cycles per operation (avg)
validate_utf8_fast(data, N)                                     :  0.741 cycles per operation (best)     0.743 cycles per operation (avg)
validate_ascii_fast(data, N)                                    :  0.083 cycles per operation (best)     0.084 cycles per operation (avg)
clauderoux_validate_ascii(data, N)                              :  0.102 cycles per operation (best)     0.102 cycles per operation (avg)


We are feeding ascii so it is always going to be ok.
```
