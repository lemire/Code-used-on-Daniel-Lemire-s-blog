#!/bin/bash
# The two sweeps (star count: run.sh; six queries: run_queries.sh) on one AWS
# instance, then terminate it. The six files of the queries are copied from
# another machine (RELAY, by ssh), where they sit in RELAY_DIR.
#
#   KEY=my-key SG=sg-... RELAY=big4 RELAY_DIR=jsonbench/ndjson FSJ_SRC=../fastpysimdjson \
#     bash aws.sh c8a.metal-48xl results_c8a
#
# All data sit on a tmpfs, spread over the NUMA nodes (numactl --interleave).
set -uo pipefail
TYPE=${1:-c8a.metal-48xl}
OUT=${2:-results_$TYPE}
WORKERS=${WORKERS:-1 16 32 64 96 128 192}
REGION=${REGION:-us-east-1}
KEY=${KEY:?set KEY}; SG=${SG:?set SG}
RELAY=${RELAY:?set RELAY}; RELAY_DIR=${RELAY_DIR:?set RELAY_DIR}
MAX_MINUTES=${MAX_MINUTES:-180}
HERE=$(cd "$(dirname "$0")" && pwd)
SSH=(ssh -i "$HOME/.ssh/$KEY.pem" -o StrictHostKeyChecking=accept-new
     -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR -o ConnectTimeout=20
     -o ServerAliveInterval=30)
RSH="ssh -i $HOME/.ssh/$KEY.pem -o StrictHostKeyChecking=accept-new -o UserKnownHostsFile=/dev/null -o LogLevel=ERROR"

AMI=$(aws ec2 describe-images --region "$REGION" --owners 099720109477 \
  --filters "Name=name,Values=ubuntu/images/hvm-ssd-gp3/ubuntu-noble-24.04-amd64-server-*" \
  --query 'reverse(sort_by(Images,&CreationDate))[0].ImageId' --output text)
UDATA=$(printf '#!/bin/bash\nshutdown -h +%s\n' "$MAX_MINUTES" | base64 | tr -d '\n')
IID=$(aws ec2 run-instances --region "$REGION" --image-id "$AMI" \
  --instance-type "$TYPE" --key-name "$KEY" --security-group-ids "$SG" \
  --associate-public-ip-address --instance-initiated-shutdown-behavior terminate \
  --user-data "$UDATA" \
  --block-device-mappings '[{"DeviceName":"/dev/sda1","Ebs":{"VolumeSize":40,"VolumeType":"gp3","DeleteOnTermination":true}}]' \
  --tag-specifications "ResourceType=instance,Tags=[{Key=Name,Value=superfastjson}]" \
  --query 'Instances[0].InstanceId' --output text) || exit 1
echo "instance $IID ($TYPE)"
trap 'echo "terminating $IID"; aws ec2 terminate-instances --region "$REGION" --instance-ids "$IID" >/dev/null' EXIT
aws ec2 wait instance-running --region "$REGION" --instance-ids "$IID"
IP=$(aws ec2 describe-instances --region "$REGION" --instance-ids "$IID" \
  --query 'Reservations[0].Instances[0].PublicIpAddress' --output text)
for i in $(seq 60); do "${SSH[@]}" ubuntu@$IP true 2>/dev/null && break; sleep 15; done
echo "ssh ubuntu@$IP"

"${SSH[@]}" ubuntu@$IP 'mkdir -p superfastjson'
rsync -az -e "$RSH" "$HERE"/{bench.py,queries.py,setup.sh,run.sh,run_queries.sh} ubuntu@$IP:superfastjson/
FSJ=${FSJ:-fastsimdjson}
if [ -n "${FSJ_SRC:-}" ]; then
  rsync -az --exclude .git --exclude build -e "$RSH" "$FSJ_SRC/" ubuntu@$IP:superfastjson/fsj-src/
  FSJ=./fsj-src
fi
"${SSH[@]}" ubuntu@$IP "set -e
cloud-init status --wait >/dev/null 2>&1 || true
sudo apt-get update -qq; sudo DEBIAN_FRONTEND=noninteractive apt-get install -y -qq build-essential numactl pigz >/dev/null
curl -LsSf https://astral.sh/uv/install.sh | sh >/dev/null 2>&1
sudo mkdir -p /mnt/ram && sudo mount -t tmpfs -o size=48G tmpfs /mnt/ram && sudo chmod 1777 /mnt/ram
export PATH=\$HOME/.local/bin:\$PATH
cd superfastjson && FSJ='$FSJ' DATA=/mnt/ram/tmp.ndjson bash setup.sh
numactl --interleave=all cp /mnt/ram/tmp.ndjson /mnt/ram/events.ndjson && rm /mnt/ram/tmp.ndjson
mkdir -p /mnt/ram/pison" || exit 1
echo "copying the query files from $RELAY ..."
ssh "$RELAY" "cd $RELAY_DIR && tar cf - twitter.ndjson bestbuy.ndjson google_map.ndjson nspl.ndjson walmart.ndjson wiki.ndjson | pigz -3" \
  | "${SSH[@]}" ubuntu@$IP "cd /mnt/ram/pison && pigz -dc | numactl --interleave=all tar xf -" || exit 1
"${SSH[@]}" ubuntu@$IP "ls -l /mnt/ram /mnt/ram/pison; grep Shmem: /sys/devices/system/node/node*/meminfo"
"${SSH[@]}" ubuntu@$IP "cd superfastjson && bash run.sh /mnt/ram/events.ndjson $WORKERS" | tee "$OUT.txt"
"${SSH[@]}" ubuntu@$IP "cd superfastjson && bash run_queries.sh /mnt/ram/pison $WORKERS" | tee "${OUT}_queries.txt"
