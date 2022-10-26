all : main1 lambda variabletemplates

source1.o : source1.cpp header1.h
	c++ -std=c++11 -c source1.cpp  -Wall -Wextra

main1: source1.o header1.h main1.cpp
	c++ -std=c++11 -o main1 main1.cpp source1.o  -Wall -Wextra

lambda: lambda.cpp
	c++ -std=c++20 -o lambda lambda.cpp -Wall -Wextra

variabletemplates: variabletemplates.cpp
	c++ -std=c++20 -o variabletemplates variabletemplates.cpp -Wall -Wextra

clean:
	rm -f main1 source1.o lambda variabletemplates