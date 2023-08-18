all:  benchmark unit
benchmark: benchmarks/benchmark.cpp latin1_to_utf8.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp latin1_to_utf8.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  latin1_to_utf8.o
	c++ -std=c++17  -Wall -Wextra -O3 latin1_to_utf8.o  -o unit tests/unit.cpp -Iinclude -fopenmp

latin1_to_utf8.o: src/latin1_to_utf8.c include/latin1_to_utf8.h
	cc -O3 -Wall -Wextra -Wconversion -march=native -c src/latin1_to_utf8.c -Iinclude


clean:
	rm -f benchmark unit latin1_to_utf8.o 
