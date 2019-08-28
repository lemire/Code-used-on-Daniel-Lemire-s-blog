OPT := -O2
CXXFLAGS := -std=c++11 $(OPT) -march=native -g

counter: counter.cpp
	$(CXX) $(CXXFLAGS) $(CXXEXTRA) -o counter counter.cpp

clean:
	rm -f counter
