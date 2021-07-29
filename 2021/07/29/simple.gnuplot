! make
! ./memuse > data.txt
set term png
set out "plot.png"
plot "data.txt" using 0:1  w lines ti "measured memory" lw 5, "data.txt" using 0:2 w lines  ti "predicted memory"  lw 5