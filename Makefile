all: testNaive testTst

testNaive:
	g++ -Wall -Werror -o testNaive.x testNaive.cpp

testTst:
	g++ -Wall -Werror -o testTst.x testTst.cpp

clean:
	rm -f testNaive.x