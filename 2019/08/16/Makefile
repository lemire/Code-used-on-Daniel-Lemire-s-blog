OPT := -O3
CXXFLAGS := -std=c++11 $(OPT) -mavx2

counter: counter.cpp
	$(CXX) $(CXXFLAGS) $(CXXEXTRA) -o counter counter.cpp

clean:
	rm -f counter
