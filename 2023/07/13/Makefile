all: benchmark unit

benchmark: benchmarks/benchmark.cpp sse_type.o
	c++ -std=c++17  -O3 -Wall -o benchmark benchmarks/benchmark.cpp  sse_type.o -Iinclude -Ibenchmarks

unit: tests/unit.cpp  sse_type.o
	c++ -std=c++17  -Wall -Wextra -O3 sse_type.o -o unit tests/unit.cpp -Iinclude -fopenmp

sse_type.o: src/sse_type.c include/sse_type.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/sse_type.c -Iinclude 

clean:
	rm -f benchmark unit sse_type.o