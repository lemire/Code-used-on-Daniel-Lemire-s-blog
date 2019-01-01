set term pngcairo fontscale 2
set out "results.png"
set ylabel "bandwidth (GB/s)"
set xlabel "number of memory lanes"
set xrange [1:]
set grid x2tics
set x2tics 12 format "" scale 0
plot "cannonhuge.txt" using 1:($3/1000) w lines ti "cannonlake", "skylakehuge.txt" using 1:($3/1000) w lines ti "skylake"
