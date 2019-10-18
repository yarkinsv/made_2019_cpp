CC=g++ -std=c++17

all: test
	
test: test.o allocator.o
	$(CC) -o allocator_test test.o allocator.o
	./allocator_test
	
test.o: test.cpp allocator.hpp
	$(CC) -c test.cpp

allocator.o: allocator.cpp allocator.hpp
	$(CC) -c allocator.cpp

clean:
	rm -rf *.o allocator_test
