set terminal pngcairo enhanced font "Arial,12" size 800,600
set output "graviton.png"
set ylabel "Bandwidth (GB/s)"
set xlabel "lanes"
plot "graviton3.txt" u 1:3 w lines ti "Graviton 3" lw 5, "graviton4.txt" u 1:3 w lines ti "Graviton 4" lw 5
