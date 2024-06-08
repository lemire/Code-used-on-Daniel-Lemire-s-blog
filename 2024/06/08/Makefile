all:   unit benchmark

unit : tests/basic.cpp src/vectorclassification.h
	c++ -O3 -Wall -o unit tests/basic.cpp -Isrc  -std=c++20
benchmark: benchmarks/benchmark.cpp src/vectorclassification.h
	c++ -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++20 -Isrc

clean:
	rm -f benchmark unit
