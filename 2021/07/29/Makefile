all: memuse

nadeau.o: nadeau.h nadeau.c
	cc -Os -c nadeau.c

memuse: memuse.cpp nadeau.h nadeau.o
	c++ -Os -o memuse memuse.cpp nadeau.o -Wall -Wextra -std=c++11
clean:
	rm -r -f nadeau.o memuse