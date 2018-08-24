
  for i in {1..20}; do time ./mandelbrot ${i}; done
  sleep 2
  for i in {1..20}; do time ./mandelbrot512 ${i}; done
  sleep 2
  for i in {1..20}; do time ./mandelbrotheavy512 ${i}; done
