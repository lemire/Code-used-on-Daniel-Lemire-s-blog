all: multiplebinarysearch 

multiplebinarysearch: multiplebinarysearch.cpp
	g++ -O3 -o multiplebinarysearch multiplebinarysearch.cpp -std=c++11
multiplebinarysearch.cpp: generator.py
	python3 generator.py > multiplebinarysearch.cpp

clean:
	rm -r -f multiplebinarysearch
