countbits: countbits.cpp benchmark.h mklarqvist.h
	$(CXX) -O3 -march=native -o countbits countbits.cpp -Wall -Wextra

test: countbits
	$(CXX) --version
	./countbits

clean:
	rm -f countbits
