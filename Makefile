all: test

test:
	g++ -Wall -Werror -o test.x test.cpp

clean:
	rm -f test.x