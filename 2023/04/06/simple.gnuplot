set term png font "arial,14.0"
set out "plot.png"
set xlabel "number of lanes"
set ylabel "sigmas"
plot "data.txt" using 1:(($2-$3)/$4)  w lines ti "min" lw 5,\
  "" using 1:(($5-$2)/$4) w lines  ti "max"  lw 5

set ylabel "relative diffence (mean-min)/min in %"
set out "plotmm.png"

plot "data.txt" using 1:(($3-$2)/$2 * 100) w lines  ti "max"  lw 5, \


set ylabel "relative diffence (min-max) in %"
set out "plotmma.png"

plot "data.txt" using 1:(($5-$2)/$2 * 100) w lines  ti "max"  lw 5, \


set ylabel "relative diffence (sigma/mean) in %"
set out "plotsm.png"

plot "data.txt" using 1:($4/$3*100) w lines  noti  lw 5, \
