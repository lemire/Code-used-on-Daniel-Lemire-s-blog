benchmark: benchmark.cpp simdjson.cpp simdjson.h
	c++ -DSIMDJSON_IMPLEMENTATION_ARM64=1 -O3 -o benchmark benchmark.cpp simdjson.cpp -std=c++11
clean:
	rm -f benchmark