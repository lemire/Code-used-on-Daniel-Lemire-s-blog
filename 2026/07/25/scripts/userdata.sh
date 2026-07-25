#!/bin/bash
exec > /var/log/mlp-run.log 2>&1
set -x
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get install -y build-essential git python3 dmidecode lshw numactl util-linux
cd /root
git clone https://github.com/lemire/testingmlp
cd /root/testingmlp
make

{
  echo "===== instance-type ====="
  TOKEN=$(curl -sX PUT http://169.254.169.254/latest/api/token -H "X-aws-ec2-metadata-token-ttl-seconds: 300")
  curl -s -H "X-aws-ec2-metadata-token: $TOKEN" http://169.254.169.254/latest/meta-data/instance-type; echo
  echo "===== uname ====="; uname -a
  echo "===== lscpu ====="; lscpu
  echo "===== lscpu-cache ====="; lscpu -C
  echo "===== cpuinfo ====="; head -40 /proc/cpuinfo
  echo "===== THP ====="; cat /sys/kernel/mm/transparent_hugepage/enabled
  echo "===== meminfo ====="; head -5 /proc/meminfo
  echo "===== dmidecode-memory ====="; dmidecode -t memory
  echo "===== dmidecode-processor ====="; dmidecode -t processor
  echo "===== dmidecode-system ====="; dmidecode -t system
  echo "===== dmidecode-bios ====="; dmidecode -t bios
  echo "===== lshw-memory ====="; lshw -short -c memory
  echo "===== numactl ====="; numactl -H
  echo "===== cpu-freq ====="; cat /proc/cpuinfo | grep -i mhz
} > /root/sysinfo.txt 2>&1

taskset -c 0 ./testingmlp > /root/mlp.txt 2>&1
echo "exit=$?" >> /root/mlp.txt
touch /root/DONE
