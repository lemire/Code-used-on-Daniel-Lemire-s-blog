set term pngcairo
set ylabel "speed (million int. per s)"                                                                         
set xlabel "sparsity"
set out "results.png"
plot "results.txt" u ($1):3 ti "naive (32 bits)" with lp lw 5,\
 "results.txt" u ($1):4 ti "fast (32 bits)" with lp lw 4,\
  "results.txt" u ($1):6 ti "naive (64 bits)" with lp lw 4,\
   "results.txt" u ($1):7 ti "fast (64 bits)" with lp lw 4