
  for i in {1..20}; do time ./mandelbrot ${i} 3200; done
  sleep 2
  for i in {1..20}; do time ./mandelbrot512 ${i} 3200; done
  sleep 2
  for i in {1..20}; do time ./mandelbrotheavy512 ${i} 3200; done
