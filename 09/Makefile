CC=g++ -std=c++17

all: test
	
test: test.o
	$(CC) -o threadpool_test test.o
	./threadpool_test
	
test.o: test.cpp threadpool.hpp
	$(CC) -c test.cpp

clean:
	rm -rf *.o threadpool_test
