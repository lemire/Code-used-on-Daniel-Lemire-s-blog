fastestrng: m1cycles.cpp benchmark.cpp
	clang++ --version
	clang++ -std=c++20 -O3 -fno-tree-vectorize -c m1cycles.cpp -o m1cycles.o -I include -Wall -Wextra
	ar rcs libm1cycles.a m1cycles.o
	clang++ -std=c++20 -O3 -fno-tree-vectorize -o benchmark benchmark.cpp libm1cycles.a -I include -Wall -Wextra
	echo "run sudo ./benchmark"
clean:
	rm -r -f benchmark
