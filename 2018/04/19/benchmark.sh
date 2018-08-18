time -p for i in {1..10}; do taskset 0x1 ./mandelbrot; done
sleep 2
time -p for i in {1..10}; do taskset 0x1 ./mandelbrot512; done
sleep 2
time -p for i in {1..10}; do taskset 0x1 ./mandelbrotheavy512; done





