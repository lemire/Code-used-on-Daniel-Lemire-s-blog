#!/bin/bash
if ! [ $(id -u) = 0 ]; then
   echo "The script need to be run as root." >&2
   exit 1
fi
if [ -z "$1" ]
  then
    echo "The takes a command as a argument. E.g., try ./hugepages.sh './bulk-insert-and-query.exe 100000000 0,1000,96,98,100' "
    exit 1
fi
COMMAND=$@
echo "Running $COMMAND"


origval=$(sudo cat /sys/kernel/mm/transparent_hugepage/enabled)
sudo echo $origval
set -e
function cleanup {
  echo "Restauring hugepages to madvise"
  echo "madvise" > /sys/kernel/mm/transparent_hugepage/enabled
}
trap cleanup EXIT

for mode in "always" "never" ; do
  sudo echo "mode: " $mode
  echo $mode > /sys/kernel/mm/transparent_hugepage/enabled
  echo $(sudo cat /sys/kernel/mm/transparent_hugepage/enabled)
  echo "$COMMAND"
  $COMMAND 
done
echo "Done."