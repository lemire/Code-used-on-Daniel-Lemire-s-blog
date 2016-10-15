FLAGS=-std=c++11 -O3 -Wall -Wextra
ALL=shuffle shuffle_avx512

all: $(ALL)

shuffle: shuffle.cpp pcg.h
	$(CXX) $(FLAGS) -march=native shuffle.cpp -o $@

shuffle_avx512: shuffle.cpp pcg.h
	$(CXX) $(FLAGS) -mavx512cd -DHAVE_AVX512CD_INSTRUCTIONS shuffle.cpp -o $@

run: shuffle_avx512
	sde -cnl -- ./$^

clean:
	rm -rf $(ALL)
