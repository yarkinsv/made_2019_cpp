CC=g++ -std=c++17

all: test
	
test: test.o format.o
	$(CC) -o format_test test.o format.o
	./format_test
	
test.o: test.cpp format.cpp
	$(CC) -c test.cpp

format.o: format.cpp
	$(CC) -c format.cpp

clean:
	rm -rf *.o format_test
