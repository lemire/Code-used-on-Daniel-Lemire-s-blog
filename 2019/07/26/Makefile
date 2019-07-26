
all: test.txt  readlines.class
	@java readlines test.txt


readlines.class: readlines.java
	javac readlines.java



test.txt:
	python gen.py > test.txt
