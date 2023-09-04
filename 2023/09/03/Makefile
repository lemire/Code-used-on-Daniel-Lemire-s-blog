all:   unit benchmark input.txt
benchmark: benchmarks/benchmark.cpp identifiers.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp identifiers.o  -Iinclude -Ibenchmarks

unit: tests/unit.cpp  identifiers.o
	c++ -std=c++17  -Wall -Wextra -O3 identifiers.o  -o unit tests/unit.cpp -Iinclude

identifiers.o: src/identifiers.c include/identifiers.h
	cc -O3 -Wall -Wextra -Wconversion -c src/identifiers.c -Iinclude

input.txt: generate.rb
	ruby generate.rb

clean:
	rm -f benchmark unit identifiers.o input.txt
