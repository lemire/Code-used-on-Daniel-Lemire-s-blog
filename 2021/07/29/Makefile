all: memuse

nadeau.o: nadeau.h nadeau.c
	cc -Os -c nadeau.c -Wall -Wextra

memuse: memuse.cpp nadeau.h nadeau.o
	c++ -Os -o memuse memuse.cpp nadeau.o -Wall -Wextra -std=c++17
clean:
	rm -r -f nadeau.o memuse