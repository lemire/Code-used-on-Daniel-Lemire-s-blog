all: access dotproduct
dotproduct: dotproduct.cpp
	c++ -o dotproduct dotproduct.cpp -mavx512vl -mavx512f -mavx512bw -Wall -Wextra -std=c++17

access: access.cpp
	c++ -o access access.cpp -mavx512vl -mavx512f -mavx512bw -Wall -Wextra -std=c++17
clean:
	rm -f access dotproduct