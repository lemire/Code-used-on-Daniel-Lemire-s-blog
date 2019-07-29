
all: test.txt getlines  readlines.class oldschoolreadlines.class
	@echo "functional, code written by a bro"
	@java readlines test.txt
	@echo "old school, code writen by a retiree..."
	@java oldschoolreadlines test.txt
	@echo "cpp"
	@./getlines test.txt

getlines: getlines.cpp
	c++ -O2 -o getlines getlines.cpp -Wall -std=c++11


readlines.class: readlines.java
	javac readlines.java


oldschoolreadlines.class: oldschoolreadlines.java
	javac oldschoolreadlines.java



test.txt:
	python gen.py > test.txt
