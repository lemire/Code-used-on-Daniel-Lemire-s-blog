all: simdshuf
	
simdshuf: simdshuf.c benchmark.h
	cc -O3 -std=c99 -o simdshuf simdshuf.c -march=native


test:simdshuf
	./simdshuf

clean:
	rm -r -f simdshuf
