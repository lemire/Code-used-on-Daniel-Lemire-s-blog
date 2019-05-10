bitmapdecoding: bitmapdecoding.cpp
	$(CXX) -O2  -std=c++14 -o bitmapdecoding bitmapdecoding.cpp -Wall -I.
sanibitmapdecoding : bitmapdecoding.cpp
	$(CXX) -g3 -fsanitize=address -fno-omit-frame-pointer   -std=c++14 -o sanibitmapdecoding bitmapdecoding.cpp -Wall -I.
clean:
	rm -r -f bitmapdecoding
