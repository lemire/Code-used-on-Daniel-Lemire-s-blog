bighash: bighash.cpp
	c++ -O3  -march=native -std=c++14 -o bighash bighash.cpp -Wall -I.
sanibighash : bighash.cpp
	c++ -g3 -fsanitize=address -fno-omit-frame-pointer  -march=native -std=c++14 -o bighash bighash.cpp -Wall -I.
clean:
	rm -r -f bighash
