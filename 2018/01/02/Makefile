# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
CFLAGS = -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow
all:  fasta simdfasta
test:
	time ./fasta 25000000 > /dev/null
	time ./simdfasta 25000000 > /dev/null


fasta:
	$(CC) $(CFLAGS) -o fasta fasta.c


simdfasta:
	$(CC) $(CFLAGS) -o simdfasta simdfasta.c


clean:
	rm -f fasta simdfasta
