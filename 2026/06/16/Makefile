# Build the compile-time-JSON demo with GCC 16 and simdjson.
#
# Run inside the gcc16 programming station, e.g. from this directory:
#   ../../../../docker_programming_station/gcc16/run-docker-station 'make run'

CXX      ?= g++
CXXFLAGS ?= -std=c++26 -freflection -DSIMDJSON_STATIC_REFLECTION=1 --embed-dir=. -O3

all: demo

# The simdjson amalgamation (no reflection flags needed here).
simdjson.o: simdjson.cpp simdjson.h
	$(CXX) -std=c++26 -O3 -c simdjson.cpp -o simdjson.o

demo: main.cpp simdjson.o data.json
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
	$(CXX) main.o simdjson.o -o demo

run: demo
	./demo

# Show that the constexpr result is folded into immediates by the optimiser.
asm: demo
	@objdump -d --no-show-raw-insn demo | \
	  awk '/<screen_area>:/{f=1} /<is_fullscreen>:/{f=1} f{print} /ret/{if(f)f=0}'

clean:
	rm -f demo main.o simdjson.o

.PHONY: all run asm clean
