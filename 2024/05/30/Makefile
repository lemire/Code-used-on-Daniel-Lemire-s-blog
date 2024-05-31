all:   benchmark 


benchmark: benchmarks/benchmark.cpp include/needsescaping.h
	c++ -std=c++20 -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -Iinclude

clean:
	rm -f benchmark 
