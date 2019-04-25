hexparse: hexparse.cpp
	# -O3 allows vectorization and discards some noinline funcions.
	c++ -O2 -march=native -std=c++14 -o hexparse hexparse.cpp -Wall -I.
clean:
	rm -r -f hexparse
