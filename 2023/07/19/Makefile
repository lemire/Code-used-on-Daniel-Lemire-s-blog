all:  unit benchmark

benchmark: benchmarks/benchmark.cpp base32.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp base32.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  base32.o
	c++ -std=c++17  -Wall -Wextra -O3  base32.o -o unit tests/unit.cpp -Iinclude -fopenmp

base32.o: src/base32.c include/base32.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/base32.c -Iinclude

clean:
	rm -f benchmark unit base32.o
