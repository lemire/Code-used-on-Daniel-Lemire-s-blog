all: twitter.json pipespeed

twitter.json:
	cp ../../06/30/twitter.json .
pipespeed: pipespeed.cpp
	c++ -std=c++11 -Wall -Wextra -o pipespeed pipespeed.cpp
clean:
	rm -r -f pipespeed
