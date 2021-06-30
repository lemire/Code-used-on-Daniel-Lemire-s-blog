all: fastsearch fastsearch2

fastsearch2: fastsearch2.cpp
	c++ -std=c++11 -O2 -o fastsearch2 fastsearch2.cpp -Wall -Wextra

fastsearch: fastsearch.cpp
	c++ -std=c++11 -O2 -o fastsearch fastsearch.cpp -Wall -Wextra
clean: 
	rm -r -f fastsearch fastsearch2
