all:   parse_integer.o unit benchmark benchmark-clang
benchmark: benchmarks/benchmark.cpp parse_integer.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp parse_integer.o  -Iinclude -Ibenchmarks
benchmark-clang: benchmarks/benchmark.cpp parse_integer-clang.o 
	c++ -std=c++17 -O3 -Wall -o benchmark-clang benchmarks/benchmark.cpp parse_integer-clang.o   -Iinclude -Ibenchmarks

unit: tests/unit.cpp  parse_integer.o
	c++ -std=c++17  -Wall -Wextra -O3 parse_integer.o  -o unit tests/unit.cpp -Iinclude

parse_integer.o: src/parse_integer.cpp include/parse_integer.h
	c++ -std=c++23 -O3 -Wall -Wextra -Wconversion -c src/parse_integer.cpp -Iinclude  -march=icelake-server

parse_integer-clang.o: src/parse_integer.cpp include/parse_integer.h
	clang++ -std=c++20 -O3 -Wall -Wextra -Wconversion -c src/parse_integer.cpp -o parse_integer-clang.o -Iinclude  -march=icelake-server

clean:
	rm -f parse_integer.o unit benchmark parse_integer-clang.o benchmark-clang
