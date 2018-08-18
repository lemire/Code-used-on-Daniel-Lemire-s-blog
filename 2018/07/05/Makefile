all: dotproductstandard dotproduct
dotproductstandard: dotproduct.c benchmark.h
	cc -O3 -o dotproductstandard dotproduct.c -march=native 
dotproduct: dotproduct.c benchmark.h
	cc -O3 -o dotproduct dotproduct.c -march=native -ffast-math


test:dotproduct
	./dotproduct

clean:
	rm -r -f dotproduct dotproductstandard
