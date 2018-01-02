# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
CFLAGS = -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow
all:  fasta simdfasta multicorefasta
test:
	@/usr/bin/time --format='%C took  %e seconds %U seconds ' ./fasta 25000000 > /dev/null
	@/usr/bin/time --format='%C took  %e seconds %U seconds ' ./simdfasta 25000000 > /dev/null
	@/usr/bin/time --format='%C took  %e seconds %U seconds ' ./multicorefasta 25000000 > /dev/null



fasta:
	$(CC) $(CFLAGS) -o fasta fasta.c


simdfasta:
	$(CC) $(CFLAGS) -o simdfasta simdfasta.c

multicorefasta:
	$(CC) $(CFLAGS) -fopenmp -o multicorefasta multicorefasta.c



clean:
	rm -f fasta simdfasta multicorefasta
