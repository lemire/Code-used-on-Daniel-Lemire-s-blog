all: helloc hellocpp hellocppstatic hellocppfullstatic
	hyperfine --runs 5000 ./helloc
	hyperfine --runs 5000 ./hellocpp
	hyperfine --runs 5000 ./hellocppstatic
	hyperfine --runs 5000 ./hellocppfullstatic

helloc: hello.c
	cc -O2 -o helloc hello.c -Wall

hellocpp: hello.cpp
	c++ -O2 -o hellocpp hello.cpp -Wall

hellocppstatic: hello.cpp
	c++ -O2 -o hellocppstatic hello.cpp -Wall -static-libstdc++

hellocppfullstatic: hello.cpp
	c++ -O2 -o hellocppfullstatic hello.cpp -Wall -static-libstdc++ -static-libgcc

clean:
	rm -r -f hellocpp helloc hellocppstatic hellocppfullstatic
