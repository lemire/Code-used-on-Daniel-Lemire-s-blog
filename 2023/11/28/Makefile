all:   parseuint8.o unit benchmark 
benchmark: benchmarks/benchmark.cpp parseuint8.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp parseuint8.o  -Iinclude -Ibenchmarks 

unit: tests/unit.cpp  parseuint8.o
	c++ -std=c++17  -Wall -Wextra -O3 parseuint8.o  -o unit tests/unit.cpp -Iinclude

parseuint8.o: src/parseuint8.cpp include/parseuint8.h
	c++ -std=c++17 -O3 -Wall -Wextra -Wconversion -c src/parseuint8.cpp -Iinclude  
clean:
	rm -f parseuint8.o unit benchmark benchmark-clang isaacs_files.txt
