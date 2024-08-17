all:    stream
stream: benchmarks/stream.cpp include/batched_shuffle.h
	$(CXX) $(CXXFLAGS) -std=c++17 -O3 -Wall  -Wextra  -o stream benchmarks/stream.cpp -Iinclude -Ibenchmarks 

clean:
	rm -f stream