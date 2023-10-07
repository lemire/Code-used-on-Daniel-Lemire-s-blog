server: server.cpp httplib.h fmt-10.1.1.zip simdjson.o ada.o
	c++ -O3 -Wall -std=c++20 server.cpp -o server -L fmt-10.1.1/build -lfmt -Ifmt-10.1.1/include simdjson.o ada.o -pthread

fmt-10.1.1.zip: 
	wget https://github.com/fmtlib/fmt/releases/download/10.1.1/fmt-10.1.1.zip
	unzip fmt-10.1.1.zip
	cd fmt-10.1.1
	cmake -B build && cmake --build build -j

ada.o: ada.h 
	c++ -O3 -Wall -std=c++20 -c ada.cpp

ada.h:
	wget https://github.com/ada-url/ada/releases/download/v2.6.10/ada.h
	wget https://github.com/ada-url/ada/releases/download/v2.6.10/ada.cpp

simdjson.o: simdjson.h
	c++ -O3 -Wall -std=c++20 -c simdjson.cpp

simdjson.h:
	wget https://github.com/simdjson/simdjson/releases/download/v3.3.0/simdjson.h
	wget https://github.com/simdjson/simdjson/releases/download/v3.3.0/simdjson.cpp

clean:
	rm -r -f server fmt-10.1.1 fmt-10.1.1.zip