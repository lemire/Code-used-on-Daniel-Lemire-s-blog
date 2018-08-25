all: mandelbrot mandelbrot512 mandelbrotheavy512

mandelbrotzero:
	gcc -O3 -o mandelbrotzero mandelbrot.c -march=native -Wall -mno-avx512f -lpthread -DZEROUPPER


mandelbrot:
	gcc -O3 -o mandelbrot mandelbrot.c -march=native -Wall -mno-avx512f -lpthread 

mandelbrot512:
	gcc -O3 -o mandelbrot512 mandelbrot.c  -DUSEAVX512 -march=native -Wall  -lpthread

mandelbrotheavy512:
	gcc -O3 -o mandelbrotheavy512 mandelbrot.c -DUSEHEAVYAVX512 -DUSEAVX512 -march=native -Wall  -lpthread 


clean:
	rm -f mandelbrot512 mandelbrot mandelbrotheavy512


