#!/usr/bin/env bash
# do not run directly
apt-get update -qq
apt-get install -y g++
echo "compiling round"
g++ -o round round.cpp
./round
echo "compiling round with -O2"
g++ -o round -O2 round.cpp
./round
echo "compiling round with -mfpmath=sse"
g++ -o round -msse -mfpmath=sse round.cpp
./round
