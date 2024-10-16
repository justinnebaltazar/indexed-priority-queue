all: test

test: test.o
	g++ -Wall -o test test.o

test.o: test.cpp IPQ.h
	g++ -Wall -c -o test.o test.cpp

clean: 
	rm -f test *.o
