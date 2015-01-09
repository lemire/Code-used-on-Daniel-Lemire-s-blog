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
set logscale x 2
set term pngcairo fontscale 1.2
set ylabel "speed (millions int. per s)"
set xlabel "bits per integer"
set key top right
set out "results.png"
plot "results.txt"  u 1:2 ti "naive" with lp ls 1,\
   "" u 1:4 ti "popcnt" with lp ls 2,\
   "" u 1:5 ti "table" with lp ls 3,\
   "" u 1:5 ti "tzcnt" with lp ls 4
