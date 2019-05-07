CXXFLAGS := -std=c++11

simulate: simulate.cpp
	$(CXX) $(CXXFLAGS) -O3 -o simulate simulate.cpp -I. -lm

sanisimulate: simulate.cpp
	$(CXX) -fsanitize=address -fno-omit-frame-pointer -g3  -o simulate simulate.cpp -I. -lm

clean:
	rm -f simulate
