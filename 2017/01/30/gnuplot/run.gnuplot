
! python simu.py > data.txt
set out "data.png"
set term pngcairo
plot "data.txt" using 1:2 ti "random insert" with lp lw 5, "" using 1:3 ti "adversarial insert" with lp lw 5
