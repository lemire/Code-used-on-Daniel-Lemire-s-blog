set term png
set out "plot.png"
set xlabel "number of warnings"
set xrange [1:300]
set key left
plot "data.txt" using 1:2  w lines ti "recall" lw 5, "data.txt" using 1:3 w lines  ti "precision"  lw 5, "data.txt" using 1:4 w lines  ti "F score"  lw 5