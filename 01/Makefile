CC=g++ -std=c++14

all: test main

main: main.o parser.o
	$(CC) -o calc main.o parser.o
	
test: test.o parser.o
	$(CC) -o calc_test test.o parser.o
	./calc_test

main.o: main.cpp parser.hpp
	$(CC) -c main.cpp
	
test.o: test.cpp parser.hpp
	$(CC) -c test.cpp

parser.o: parser.cpp parser.hpp
	$(CC) -c parser.cpp

clean:
	rm -rf *.o calc calc_test
