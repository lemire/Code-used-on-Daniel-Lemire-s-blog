#! gnuplot searchproposal1.gnuplot
load "linespointsstyle.gnuplot"


set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey
set grid back linestyle 81
set term pdfcairo fontscale 0.75
#set term png fontscale 0.6
set xlabel "array length"
set ylabel "average query time (cycles)"


set key bottom right


set out "searchproposal1.outofcache.pdf"




plot "skylake_28jan2016.txt"  u 1:2 title "binary search" with linespoints  ls 5,\
 "" u 1:3 title "linear-256-16" with linespoints  ls 4,\
 "" u 1:4 title "branchless bin. search" with linespoints  ls 3,\
 "" u 1:5 title "branchless bin. search + prefetch" with linespoints  ls 6

 set out "searchproposal1.incache.pdf"




 plot "skylake_28jan2016.txt"  u 1:6 title "binary search" with linespoints  ls 5,\
  "" u 1:7 title "linear-256-16" with linespoints  ls 4,\
  "" u 1:8 title "branchless bin. search" with linespoints  ls 3,\
  "" u 1:9 title "branchless bin. search + prefetch" with linespoints  ls 6

   set out "searchproposal1.ratio.outofcache.pdf"

   set ylabel "benefit (%) of linear-256-16"

   plot "skylake_28jan2016.txt"  u 1:($2/$3 * 100 - 100) title "binary search/linear-256-16" with linespoints  ls 5
