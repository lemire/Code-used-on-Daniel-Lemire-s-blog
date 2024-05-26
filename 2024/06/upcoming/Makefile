all:   benchmark 


benchmark: benchmarks/benchmark.cpp src/to_bitmap.h
	clang++ -march=icelake-server -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++20 -Isrc

clean:
	rm -f benchmark 
