simulate: simulate.c
	cc -O3 -o simulate simulate.c -I. -lm
sanisimulate: simulate.c
	cc -fsanitize=address -fno-omit-frame-pointer -g3  -o simulate simulate.c -I. -lm

clean:
	rm -f simulate
