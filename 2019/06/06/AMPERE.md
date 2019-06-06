 void ShuffleBenchmark64(size_t) [with uint64_t (* rfnc64)() = lehmer64; size_t = long unsigned int]
Shuffling arrays of size 1000
Time reported in number of ns per array element.
Tests assume that array is in cache as much as possible.
shuffle_go64<rfnc64>(testvalues, size)                      :  41288 ns total,  41.29 ns per input key
shuffle_java64<rfnc64>(testvalues, size)                    :  20666 ns total,  20.67 ns per input key
shuffle_nearlydivisionless64<rfnc64>(testvalues, size)      :  5044 ns total,  5.04 ns per input key
shuffle_floatmult64<rfnc64>(testvalues, size)               :  14733 ns total,  14.73 ns per input key
std::shuffle(testvalues, testvalues + size, gen)            :  27288 ns total,  27.29 ns per input key

