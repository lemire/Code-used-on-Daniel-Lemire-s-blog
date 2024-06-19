all:   benchmark 

target = $(shell if [ $$(uname -m) = 'x86_64' ]; then echo '-march=native'; fi)

benchmark: benchmarks/benchmark.cpp src/matrix_multiply.h
	c++ $(target) -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++20 -Isrc

clean:
	rm -f benchmark 
