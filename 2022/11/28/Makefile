all: generate generate_int

generate: generate.cpp
	c++ -O3 -o generate generate.cpp -Wall
generate_int: generate_int.cpp
	c++ -O3 -o generate_int generate_int.cpp -Wall
clean:
	rm -r -f generate generate_int 