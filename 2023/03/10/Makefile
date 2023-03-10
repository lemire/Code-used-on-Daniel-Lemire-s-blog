all: prune

prune: prune.cpp
	c++ -O3 -Wall prune.cpp -o prune -march=armv8-a+sve -std=c++17

clean:
	rm -r -f prune