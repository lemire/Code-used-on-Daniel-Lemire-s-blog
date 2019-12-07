all: condrng 

condrng: condrng.cpp
	$(CXX) -O3 -march=native -std=c++11 -o condrng condrng.cpp -Wall -I.

clean:
	rm -r -f condrng
