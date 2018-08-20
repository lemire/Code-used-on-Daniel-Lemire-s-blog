all: fastrangenoavx2 fastrangeavx2
	./fastrangenoavx2
	./fastrangeavx2

fastrangenoavx2:
	$(CC) -O3 -o fastrangenoavx2 fastrange.c -mno-avx2 

fastrangeavx2:
	$(CC) -O3 -o fastrangeavx2 fastrange.c -mavx2

clean:
	rm -f fastrangeavx2 fastrangenoavx2
