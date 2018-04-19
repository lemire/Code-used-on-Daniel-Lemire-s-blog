all: mandelbrot mandelbrot512

mandelbrot:
	gcc -O3 -o mandelbrot mandelbrot.c -DUSEAVX512 -march=native -Wall

mandelbrot512:
	gcc -O3 -o mandelbrot512 mandelbrot.c  -march=native -Wall

test: mandelbrot mandelbrot512
	time ./mandelbrot
	time ./mandelbrot512

clean:
	rm -f mandelbrot512 mandelbrot


