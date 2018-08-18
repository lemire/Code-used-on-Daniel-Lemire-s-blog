detected:=$(shell gcc -march=native -Q --help=target | grep -- '-march=' | cut -f3)

all: native.s specific.s 
	-diff native.s specific.s

native.s: diffi.c
	@gcc -march=native -O2 -c diffi.c -o native.s

specific.s: diffi.c
	echo "compiling to "$(detected)
	@gcc -march=$(detected) -O2 -c diffi.c -o specific.s


