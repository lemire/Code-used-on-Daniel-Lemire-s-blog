fastestrng: m1cycles.cpp benchmark.cpp
	clang++ --version
	clang++ -std=c++17 -O2 -fno-tree-vectorize -o benchmark m1cycles.cpp benchmark.cpp  -I include -Wall -Wextra
	echo "run sudo ./benchmark"
clean:
	rm -r -f benchmark
