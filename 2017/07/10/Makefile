# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
CFLAGS=-O3 -Wall -Wextra -Wshadow -std=c99
.PHONY:    all clean

all: despacebenchmark

despacebenchmark : despacebenchmark.c  despacer.h
	$(CC) $(CFLAGS) -o despacebenchmark despacebenchmark.c

clean:
	-rm despacebenchmark
