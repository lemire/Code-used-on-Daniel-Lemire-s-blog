set term pngcairo fontscale 2
set out "results.png"
set ylabel "speedup factor"
set xlabel "number of parallel searches"
set xrange [2:64]
set yrange [1:]

set grid xtics
set grid ytics

set key center
plot "cannon_huge.txt" using 1:3 w lines lw 5 ti "huge pages", "cannon.txt" using 1:3 w lines lw 5  ti "regular pages"
