all: verbose pipespeed cpipespeed

verbose: verbose.cpp
	c++ -O3 -std=c++17 -Wall -Wextra -o verbose verbose.cpp
pipespeed: pipespeed.cpp
	c++ -O3 -std=c++17 -Wall -Wextra -o pipespeed pipespeed.cpp
cpipespeed: cpipespeed.cpp
	c++ -O3 -std=c++17 -Wall -Wextra -o cpipespeed cpipespeed.cpp
clean:
	rm -r -f verbose cpipespeed pipespeed
