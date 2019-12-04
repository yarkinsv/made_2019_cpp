CC=g++ -std=c++17

all: test
	
test: test.o bigint.o
	$(CC) -o bigint_test test.o bigint.o
	./bigint_test
	
test.o: test.cpp bigint.hpp
	$(CC) -c test.cpp

bigint.o: bigint.cpp bigint.hpp
	$(CC) -c bigint.cpp

clean:
	rm -rf *.o bigint_test
