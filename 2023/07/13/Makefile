all: benchmark unit

benchmark: benchmarks/benchmark.cpp sse_type.o sse_upper_type.o upper_type.o sse_table.o
	c++ -std=c++17 -O3 -Wall -o benchmark benchmarks/benchmark.cpp sse_type.o sse_upper_type.o upper_type.o sse_table.o -Iinclude -Ibenchmarks

unit: tests/unit.cpp sse_type.o sse_upper_type.o upper_type.o sse_table.o
	c++ -std=c++17  -Wall -Wextra -O3 sse_type.o sse_upper_type.o upper_type.o sse_table.o -o unit tests/unit.cpp -Iinclude -fopenmp

sse_type.o: src/sse_type.c include/sse_type.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/sse_type.c -Iinclude

sse_upper_type.o: src/sse_upper_type.c include/sse_type.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/sse_upper_type.c -Iinclude

upper_type.o: src/upper_type.c include/sse_type.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/upper_type.c -Iinclude

sse_table.o: src/sse_table.c include/sse_type.h
	cc -O3 -Wall -Wextra -Wconversion -march=westmere -c src/sse_table.c -Iinclude

clean:
	rm -f benchmark unit sse_type.o sse_upper_type.o upper_type.o sse_table.o
