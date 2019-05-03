bitmapdecoding: bitmapdecoding.cpp
	c++ -O2  -march=native -std=c++14 -o bitmapdecoding bitmapdecoding.cpp -Wall -I.
sanibitmapdecoding : bitmapdecoding.cpp
	c++ -g3 -fsanitize=address -fno-omit-frame-pointer  -march=native -std=c++14 -o sanibitmapdecoding bitmapdecoding.cpp -Wall -I.
clean:
	rm -r -f bitmapdecoding
