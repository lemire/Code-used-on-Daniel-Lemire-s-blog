all:   benchmark 


benchmark: benchmarks/benchmark.cpp include/breakdown.h
	g++ -std=c++20 -O3 -Wall -Wextra -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++2a -Iinclude

clean:
	rm -f benchmark 
