all:  unit benchmark

benchmark: benchmarks/benchmark.cpp base16.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp base16.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  base16.o
	c++ -std=c++17  -Wall -Wextra -O3  base16.o -o unit tests/unit.cpp -Iinclude -fopenmp

base16.o: src/base16.c include/base16.h
	cc -O3 -Wall -Wextra -Wconversion -march=haswell -c src/base16.c -Iinclude

clean:
	rm -f benchmark unit base16.o
