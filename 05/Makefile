CC=g++ -std=c++17

all: test
	
test: test.o
	$(CC) -o serializer_test test.o
	./serializer_test
	
test.o: test.cpp serializer.hpp
	$(CC) -c test.cpp

clean:
	rm -rf *.o serializer_test
