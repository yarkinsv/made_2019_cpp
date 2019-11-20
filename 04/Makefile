CC=g++ -std=c++17

all: test
	
test: test.o matrix.o
	$(CC) -o matrix_test test.o matrix.o
	./matrix_test
	
test.o: test.cpp matrix.cpp
	$(CC) -c test.cpp

tokenizer.o: matrix.cpp
	$(CC) -c matrix.cpp

clean:
	rm -rf *.o matrix_test
