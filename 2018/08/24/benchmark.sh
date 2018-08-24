
  time -p for i in {1..10}; do ./mandelbrot ${i}; done
  sleep 2
  time -p for i in {1..10}; do ./mandelbrot512 ${i}; done
  sleep 2
  time -p for i in {1..10}; do ./mandelbrotheavy512 ${i}; done
