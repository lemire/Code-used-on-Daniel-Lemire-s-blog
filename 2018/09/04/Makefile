all: scalar

scalar: scalar.c
	gcc -O3 -o scalar scalar.c -march=native -Wall -lpthread 
test : scalar
	perf stat ./scalar
clean:
	rm -f scalar


