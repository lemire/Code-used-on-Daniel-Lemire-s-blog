all:  benchmark unit
benchmark: benchmarks/benchmark.cpp utf8_to_latin1.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp utf8_to_latin1.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  utf8_to_latin1.o
	c++ -std=c++17  -Wall -Wextra -O3 utf8_to_latin1.o  -o unit tests/unit.cpp -Iinclude -fopenmp

utf8_to_latin1.o: src/utf8_to_latin1.c include/utf8_to_latin1.h
	cc -O3 -Wall -Wextra -Wconversion -march=native -c src/utf8_to_latin1.c -Iinclude


clean:
	rm -f benchmark unit utf8_to_latin1.o 
