all:   benchmark 
benchmark: benchmarks/benchmark.cpp 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks 

clean:
	rm -f benchmark
