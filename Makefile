fastestrng:  benchmark.cpp
	clang++ --version
	clang++ -std=c++20 -O3 -fno-tree-vectorize -o benchmark benchmark.cpp  -I include -Wall -Wextra
	echo "run sudo ./benchmark"
clean:
	rm -r -f benchmark
