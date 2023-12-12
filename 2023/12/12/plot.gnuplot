# Set the output to a png file
set terminal pngcairo size 800,600 font "Helvetica,12"

# The file we'll write to
set output 'applem2.png'

# The labels for the axes
set xlabel 'stride in bytes'
set ylabel 'GB/s'

set grid xtics ytics
set logscale x 2
# Plot the data from the file
plot 'applem2.txt' using 1:2 with lines lw 3 lc rgb "blue" title 'average', \
     'applem2.txt' using 1:3 with lines lw 1 lc rgb "red" title 'minimum', \
     'applem2.txt' using 1:4 with lines lw 1 lc rgb "orange" title 'maximum'