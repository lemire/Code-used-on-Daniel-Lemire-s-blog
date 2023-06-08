all: benchmark unit

benchmark: benchmarks/benchmark.cpp sse_inet_aton.o
	c++ -std=c++17  -O3 -Wall -o benchmark benchmarks/benchmark.cpp  sse_inet_aton.o -Iinclude -Ibenchmarks

unit: tests/unit.cpp  sse_inet_aton.o
	c++ -std=c++17  -Wall -Wextra -O3 sse_inet_aton.o -o unit tests/unit.cpp -Iinclude

sse_inet_aton.o: src/sse_inet_aton.c include/sse_inet_aton.h
	cc -O3 -Wall -msse4 -c src/sse_inet_aton.c -Iinclude

clean:
	rm -f benchmark unit sse_inet_aton.o