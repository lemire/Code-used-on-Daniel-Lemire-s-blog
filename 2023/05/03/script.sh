./verbose 1000000 > tmp
for i in {1..3}; do cat tmp | ./ohohpipespeed && cat tmp | ./pipespeed && cat tmp | ./cpipespeed; done
