all: memcmpfun

memcmpfun: memcmpfun.c benchmark.h
	cc -O3 -g3 -o memcmpfun memcmpfun.c -march=native


test:memcmpfun
	./memcmpfun

clean:
	rm -r -f memcmpfun
