set out "timings.png"
set term pngcairo
set xlabel "cycles per operation"
set ylabel "frequency"
set xrange [3.5:4.5]
plot "timings.txt" using 2:1 with linespoints lt 3 lw 3 notitle
