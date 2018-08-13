Execute the ``all.sh`` script on a 2-CPU Xeon gold processor.

```
$ python analysis.py
results_test1_20.txt
no avx:  7.61 plus minus 0.03
avx-512:  7.48 plus minus 0.03
results_test2_20.txt
no avx:  7.62 plus minus 0.03
avx-512:  7.43 plus minus 0.03
results_test3_20.txt
no avx:  7.65 plus minus 0.06
avx-512:  7.45 plus minus 0.06
results_test1_40.txt
no avx:  6.24 plus minus 0.3
avx-512:  5.78 plus minus 0.3
results_test2_40.txt
no avx:  6.05 plus minus 0.1
avx-512:  5.81 plus minus 0.1
results_test3_40.txt
no avx:  6.12 plus minus 0.03
avx-512:  5.8 plus minus 0.03
results_test1_80.txt
no avx:  8.55 plus minus 0.4
avx-512:  10.7 plus minus 0.4
results_test2_80.txt
no avx:  8.43 plus minus 0.2
avx-512:  10.7 plus minus 0.2
results_test3_80.txt
no avx:  8.3 plus minus 0.3
avx-512:  10.7 plus minus 0.3
```
