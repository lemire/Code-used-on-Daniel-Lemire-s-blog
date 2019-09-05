all: reference 

reference: reference.cpp
	$(CXX) --version
	$(CXX) -O3 -std=c++11 -o reference reference.cpp -Wall -I.

clean:
	rm -r -f reference
