bench: bench.cpp rapidjson/license.txt
	$(CXX) -std=c++11 -O3 -o $@ bench.cpp -Irapidjson/include -Iinclude  -march=native -lm -Wall -Wextra

rapidjson/license.txt:
	git submodule update --init --recursive

clean:
	rm -f bench
