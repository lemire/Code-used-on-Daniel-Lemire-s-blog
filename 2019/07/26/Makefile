
all: test.txt  readlines.class oldschoolreadlines.class
	@echo "functional, code written by a bro"
	@java readlines test.txt
	@echo "old school, code writen by a retiree..."
	@java oldschoolreadlines test.txt



readlines.class: readlines.java
	javac readlines.java


oldschoolreadlines.class: oldschoolreadlines.java
	javac oldschoolreadlines.java



test.txt:
	python gen.py > test.txt
