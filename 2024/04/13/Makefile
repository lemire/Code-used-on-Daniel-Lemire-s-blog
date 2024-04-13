all:   benchmark 

gcd.pcm: module/gcd.cppm
	c++ -O3 -std=c++20 -fmodules-ts --precompile module/gcd.cppm

gcd.o: gcd.pcm
	c++ -O3 -std=c++20 -fmodules-ts -c gcd.pcm

benchmark: benchmarks/benchmark.cpp gcd.o
	c++ -std=c++20 -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++2a -fmodules-ts -fprebuilt-module-path=.

clean:
	rm -f benchmark gcd.o gcd.pcm
