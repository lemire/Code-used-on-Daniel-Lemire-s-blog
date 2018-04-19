all: mandelbrot mandelbrot512 mandelbrotheavy512

mandelbrot:
	gcc -O3 -o mandelbrot mandelbrot.c -march=native -Wall -mno-avx512f

mandelbrot512:
	gcc -O3 -o mandelbrot512 mandelbrot.c  -DUSEAVX512 -march=native -Wall  

mandelbrotheavy512:
	gcc -O3 -o mandelbrotheavy512 mandelbrot.c -DUSEHEAVYAVX512 -DUSEAVX512 -march=native -Wall  


clean:
	rm -f mandelbrot512 mandelbrot mandelbrotheavy512


