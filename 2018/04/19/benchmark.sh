time -p for i in {1..10}; do taskset 0x1 ./mandelbrot 3200; done
sleep 2
time -p for i in {1..10}; do taskset 0x1 ./mandelbrot512 3200; done
sleep 2
time -p for i in {1..10}; do taskset 0x1 ./mandelbrotheavy512 3200; done





