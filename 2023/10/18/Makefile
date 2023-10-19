all:   escape.o unit benchmark isaacs_files.txt
benchmark: benchmarks/benchmark.cpp escape.o 
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp escape.o  -Iinclude -Ibenchmarks 

unit: tests/unit.cpp  escape.o
	c++ -std=c++17  -Wall -Wextra -O3 escape.o  -o unit tests/unit.cpp -Iinclude

escape.o: src/escape.cpp include/escape.h
	c++ -std=c++17 -O3 -Wall -Wextra -Wconversion -c src/escape.cpp -Iinclude  
isaacs_files.txt:
	wget https://github.com/ada-url/url-various-datasets/raw/main/files/isaacs_files.txt
clean:
	rm -f escape.o unit benchmark benchmark-clang isaacs_files.txt
