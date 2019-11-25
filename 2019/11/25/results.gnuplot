set style line 80 lt rgb "#000000"
set style line 1 lc rgb "#A00000"  lw 3 pt 1 ps 1
set style line 2 lc rgb "#00A000"  lw 3 pt 5 ps 1
set style line 3 lc rgb "#5060D0"  lw 3 pt 7 ps 1
set style line 4 lc rgb "#FF1493"  lw 3 pt 9 ps 1
set border 3 back linestyle 80 # Remove border on top and right.  These
             # borders are useless and make it harder
             # to see plotted lines near the border.
    # Also, put it in grey; no need for so much emphasis on a border.
set xtics nomirror
set ytics nomirror

set term pngcairo fontscale 1.3
set ylabel "bandwidth (GB/s, one core)"
set xlabel "number of lanes"
set key top left
set out "results.png"
plot "rome.txt" using 1:($3/1024) with lines lw 3 ti "AMD Rome", "skylake-x.txt" using 1:($3/1024) with lines lw 3 ti "Intel Skylake X"