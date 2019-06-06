shuffle_go64<rfnc64>(testvalues, size)                      :  9549 ns total,  9.55 ns per input key
shuffle_java64<rfnc64>(testvalues, size)                    :  4791 ns total,  4.79 ns per input key
shuffle_nearlydivisionless64<rfnc64>(testvalues, size)      :  3369 ns total,  3.37 ns per input key
shuffle_floatmult64<rfnc64>(testvalues, size)               :  10990 ns total,  10.99 ns per input key
std::shuffle(testvalues, testvalues + size, gen)            :  7183 ns total,  7.18 ns per input key

