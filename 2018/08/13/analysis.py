
import re
import numpy

for file in ['results_test1_20.txt', 'results_test2_20.txt',  'results_test3_20.txt', 'results_test1_40.txt', 'results_test2_40.txt',  'results_test3_40.txt', 'results_test1_80.txt', 'results_test2_80.txt',  'results_test3_80.txt']:
  print(file)
  c = open(file, 'r').read()
  allnumbers=list(map(float,re.findall("(?<=real\\t0m)[0-9]+\.[0-9]*",c)))

  noavxnumbers=allnumbers[0:5]
  avxnumbers=allnumbers[5:]

  print("no avx: ", '%.3g' %numpy.mean(noavxnumbers),"plus minus",  '%.1g' % numpy.std(noavxnumbers))
  print("avx-512: ", '%.3g' % numpy.mean(avxnumbers),"plus minus",  '%.1g' % numpy.std(noavxnumbers))
