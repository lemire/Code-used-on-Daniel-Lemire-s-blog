all: benchmark unit

benchmark: benchmarks/benchmark.cpp sse_date.o
	c++ -std=c++17  -O3 -Wall -o benchmark benchmarks/benchmark.cpp  sse_date.o -Iinclude -Ibenchmarks

unit: tests/unit.cpp  sse_date.o
	c++ -std=c++17  -Wall -Wextra -O3 sse_date.o -o unit tests/unit.cpp -Iinclude -fopenmp

sse_date.o: src/sse_date.c include/sse_date.h
	gcc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/sse_date.c -Iinclude

clean:
	rm -f benchmark unit sse_date.o