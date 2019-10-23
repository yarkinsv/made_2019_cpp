CC=g++ -std=c++17

all: test
	
test: test.o tokenizer.o
	$(CC) -o tokenizer_test test.o tokenizer.o
	./tokenizer_test
	
test.o: test.cpp tokenizer.hpp
	$(CC) -c test.cpp

tokenizer.o: tokenizer.cpp tokenizer.hpp
	$(CC) -c tokenizer.cpp

clean:
	rm -rf *.o tokenizer_test
