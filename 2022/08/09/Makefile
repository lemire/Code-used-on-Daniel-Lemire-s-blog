all: helloc hellocpp hellocppstatic hellocppfullstatic helloprintfcpp hellonoendl
	hyperfine  --runs 5000 ./helloc --warmup 100
	hyperfine  --runs 5000 ./hellocpp --warmup 100
	hyperfine  --runs 5000 ./hellocppstatic --warmup 100
	hyperfine  --runs 5000 ./hellocppfullstatic --warmup 100
	hyperfine  --runs 5000 ./helloprintfcpp --warmup 100
	hyperfine  --runs 5000 ./hellonoendl --warmup 100
	LD_DEBUG=statistics ./helloc
	LD_DEBUG=statistics ./hellocpp
	LD_DEBUG=statistics ./hellocppstatic
	LD_DEBUG=statistics ./hellocppfullstatic
	LD_DEBUG=statistics ./helloprintfcpp
	LD_DEBUG=statistics ./hellonoendl

helloc: hello.c
	cc -O2 -o helloc hello.c -Wall

helloprintfcpp: helloprintf.cpp
	c++ -O2 -o helloprintfcpp helloprintf.cpp -Wall

hellonoendl: hellonoendl.cpp
	c++ -O2 -o hellonoendl hellonoendl.cpp -Wall

hellocpp: hello.cpp
	c++ -O2 -o hellocpp hello.cpp -Wall

hellocppstatic: hello.cpp
	c++ -O2 -o hellocppstatic hello.cpp -Wall -static-libstdc++

hellocppfullstatic: hello.cpp
	c++ -O2 -o hellocppfullstatic hello.cpp -Wall -static-libstdc++ -static-libgcc

clean:
	rm -r -f hellocpp helloc hellocppstatic hellocppfullstatic
