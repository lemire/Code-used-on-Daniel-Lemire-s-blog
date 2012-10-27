set term pngcairo fontscale 1.2
set ylabel "speed (millions int. per s)"                                                                         
set xlabel "density"
set key center
set out "results.png"
plot "data.txt"  u 1:3 ti "naive intersection" with lp lw 2 lt 3,\
   "" u 1:5 ti "bitmap intersection (ewah)" with lp lw 2 lt 7,\
   "" u 1:11 ti "bitmap intersection (sparse)" with lp lw 2 lt 9