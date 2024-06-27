set terminal pngcairo size 1920,1080 enhanced font 'Verdana,32'
set output 'memory_sizes_comparison.png'

set title 'Memory Allocation Comparison'
set xlabel 'Requested Size (bytes)'
set ylabel 'Allocated Size (bytes)'

set grid
set key outside

# Custom line styles for clarity
set style line 1 lc rgb '#1f77b4' lt 1 lw 10 pt 7 ps 1.5 # Linux
set style line 2 lc rgb '#ff7f0e' lt 1 lw 10 pt 11 ps 1.5 # macOS


# Plotting data
plot 'linuxx64.txt' using 1:2 with lines linestyle 1 title 'Linux', \
     'applem2.txt' using 1:2 with lines linestyle 2 title 'macOS',\
     x title 'requested' lw 5
