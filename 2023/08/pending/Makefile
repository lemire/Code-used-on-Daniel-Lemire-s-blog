all:  unit benchmark
benchmark: benchmarks/benchmark.cpp name_to_dnswire.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp name_to_dnswire.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  name_to_dnswire.o
	c++ -std=c++17  -Wall -Wextra -O3  name_to_dnswire.o -o unit tests/unit.cpp -Iinclude -fopenmp

name_to_dnswire.o: src/name_to_dnswire.c include/name_to_dnswire.h
	cc -O3 -Wall -Wextra -Wconversion -march=native -c src/name_to_dnswire.c -Iinclude

clean:
	rm -f benchmark unit name_to_dnswire.o
