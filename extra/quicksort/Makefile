
bench: bench.cpp
	c++ -O3 -o bench bench.cpp -Wall -Wextra -std=c++11 

debugbench: bench.cpp
	c++ -g3 -fsanitize=address -fno-omit-frame-pointer -o bench bench.cpp -Wall -Wextra -std=c++11 -fsanitize=undefined


clean:
	rm -r -f bench