
all: test.txt zee.class dan.class
	@echo "daniel"
	@java dan test.txt
	@echo "zeeshan"
	@java zee test.txt

zee.class: zee.java
	javac zee.java

dan.class: dan.java
	javac dan.java



test.txt:
	python gen.py > test.txt
