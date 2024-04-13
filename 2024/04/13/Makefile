all:   benchmark 


benchmark: benchmarks/benchmark.cpp gcd.o
	clang++ -std=c++20 -O3 -Wall -o benchmark benchmarks/benchmark.cpp -Ibenchmarks  -std=c++2a -Imodule

clean:
	rm -f benchmark 
