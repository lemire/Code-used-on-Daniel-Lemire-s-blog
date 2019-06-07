```
shuffle_go64<rfnc64>(testvalues, size)                      :  14066 ns total,  14.07 ns per input key
shuffle_java64<rfnc64>(testvalues, size)                    :  7301 ns total,  7.30 ns per input key
shuffle_nearlydivisionless64<rfnc64>(testvalues, size)      :  1912 ns total,  1.91 ns per input key
shuffle_floatmult64<rfnc64>(testvalues, size)               :  6234 ns total,  6.23 ns per input key
std::shuffle(testvalues, testvalues + size, gen)            :  11007 ns total,  11.01 ns per input key
```
