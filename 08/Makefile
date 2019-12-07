CC=g++ -std=c++17

all: test
	
test: test.o myvector.o
	$(CC) -o myvector_test test.o myvector.o
	./myvector_test
	
test.o: test.cpp myvector.h
	$(CC) -c test.cpp

myvector.o: myvector.cpp myvector.h
	$(CC) -c myvector.cpp

clean:
	rm -rf *.o myvector_test
