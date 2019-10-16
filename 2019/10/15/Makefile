all: condrng 

condrng: condrng.cpp
	$(CXX) -O2 -funroll-loops -std=c++11 -o condrng condrng.cpp -Wall -I.

clean:
	rm -r -f condrng
