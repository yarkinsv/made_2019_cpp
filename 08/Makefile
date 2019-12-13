CC=g++ -std=c++17

all: test
	
test: test.o
	$(CC) -o myvector_test test.o
	./myvector_test
	
test.o: test.cpp myvector.h
	$(CC) -c test.cpp

clean:
	rm -rf *.o myvector_test
