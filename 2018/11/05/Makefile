CXXFLAGS = -O2 -std=c++11

testingmlp: testingmlp.o generated.o
	$(CXX) $(CXXFLAGS) -o $@ $^


ifneq ($(GENERATE), 0)
generated.cpp : gen.py
	python3 gen.py > generated.cpp
endif

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $^

test: testingmlp
	./testingmlp

clean:
	rm -f testingmlp generated.cpp *.o
