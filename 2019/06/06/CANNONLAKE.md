```
shuffle_go64<rfnc64>(testvalues, size)                      :  7161 ns total,  7.16 ns per input key
shuffle_java64<rfnc64>(testvalues, size)                    :  3754 ns total,  3.75 ns per input key
shuffle_nearlydivisionless64<rfnc64>(testvalues, size)      :  2526 ns total,  2.53 ns per input key
shuffle_floatmult64<rfnc64>(testvalues, size)               :  8240 ns total,  8.24 ns per input key
std::shuffle(testvalues, testvalues + size, gen)            :  5387 ns total,  5.39 ns per input key
```
