#!/usr/bin/env bash
# do not run directly
apt-get update -qq
apt-get install -y g++
echo "compiling round"
g++ -o round round.cpp
./round
