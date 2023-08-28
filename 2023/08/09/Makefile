all:  unit benchmark
benchmark: benchmarks/benchmark.cpp name_to_dnswire.o scalar_labels.o
	c++ -std=c++17 -O3 -Wall  -march=native -o benchmark benchmarks/benchmark.cpp name_to_dnswire.o scalar_labels.o -Iinclude -Ibenchmarks

unit: tests/unit.cpp  name_to_dnswire.o scalar_labels.o
	c++ -std=c++17  -Wall -Wextra -march=native -O3 name_to_dnswire.o scalar_labels.o -o unit tests/unit.cpp -Iinclude -fopenmp

name_to_dnswire.o: src/name_to_dnswire.c include/name_to_dnswire.h
	cc -O3 -Wall -Wextra -Wconversion -march=native -c src/name_to_dnswire.c -Iinclude

scalar_labels.o: src/scalar_labels.c include/name_to_dnswire.h
	cc -O3 -Wall -Wextra -Wconversion -march=native -c src/scalar_labels.c -Iinclude

clean:
	rm -f benchmark unit name_to_dnswire.o scalar_labels.o
