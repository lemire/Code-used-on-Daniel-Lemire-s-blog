

all: binarytrees fasta  mandelbrot 

%: %.c 
	$(CC) -o $@ $< $(CFLAGS) -lm

test: binarytrees fasta  mandelbrot
	bash test.sh

clean:
	rm  -f binarytrees fasta  mandelbrot
