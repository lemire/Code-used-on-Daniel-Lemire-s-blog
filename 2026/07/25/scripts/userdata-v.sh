#!/bin/bash
exec > /var/log/mlp-v.log 2>&1
set -x
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get install -y build-essential git python3 dmidecode
cd /root && git clone https://github.com/lemire/testingmlp && cd testingmlp
sed -i 's/int repeat = 30;/int repeat = getenv_int("MLP_REPEAT", 30);/' testingmlp.cpp
make
{ echo "== lscpu =="; lscpu; echo "== dmi =="; dmidecode -t memory; } > /root/sysinfo.txt 2>&1
export MLP_CSV=1 MLP_MAX_MLP=99 MLP_START=131072 MLP_STOP=131072 MLP_REPEAT=5
taskset -c 0 ./testingmlp > /root/mlp2.csv 2> /root/mlp2.err
touch /root/DONE2
