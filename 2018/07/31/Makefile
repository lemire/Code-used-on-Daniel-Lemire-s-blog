all: cachelinefun

cachelinefun: cachelinefun.c benchmark.h
	cc -O3 -o cachelinefun cachelinefun.c -march=native


test:cachelinefun
	./cachelinefun

clean:
	rm -r -f cachelinefun
