#!/bin/bash
# Remote pass-2 script: bigger array (1 GiB, well past every L3 here),
# up to 99 lanes, CSV mode (skips the O(n*lanes) cycle verification).
exec > /var/log/mlp-pass2.log 2>&1
set -x
cd /root/testingmlp
# wait for pass 1 to be finished
while ! [ -f /root/DONE ]; do sleep 10; done
# make `repeat` configurable so a 1 GiB sweep to 99 lanes stays tractable
sed -i 's/int repeat = 30;/int repeat = getenv_int("MLP_REPEAT", 30);/' testingmlp.cpp
grep -n "MLP_REPEAT" testingmlp.cpp
make GENERATE=0
export MLP_CSV=1 MLP_MAX_MLP=99 MLP_START=131072 MLP_STOP=131072 MLP_REPEAT=5
taskset -c 0 ./testingmlp > /root/mlp2.csv 2> /root/mlp2.err
echo "exit=$?" >> /root/mlp2.err
touch /root/DONE2
