all: verbose pipespeed cpipespeed ohohpipespeed

verbose: verbose.cpp
	c++ -O3 -std=c++17 -Wall -Wextra -o verbose verbose.cpp
pipespeed: pipespeed.cpp line_iterator.h
	c++ -O3 -std=c++17 -Wall -Wextra -o pipespeed pipespeed.cpp
cpipespeed: cpipespeed.cpp line_iterator.h
	c++ -O3 -std=c++17 -Wall -Wextra -o cpipespeed cpipespeed.cpp
ohohpipespeed: ohohpipespeed.cpp line_iterator.h
	c++ -O3 -std=c++17 -Wall -Wextra -o ohohpipespeed ohohpipespeed.cpp
clean:
	rm -r -f verbose cpipespeed pipespeed ohohpipespeed tmp
