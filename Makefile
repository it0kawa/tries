FLAGS = -Wall -Werror -O2 -std=c++17

all: testNaive testTst testPatricia

testNaive:
	g++ $(FLAGS) -o testNaive.x testNaive.cpp

testTst:
	g++ $(FLAGS) -o testTst.x testTst.cpp

testPatricia:
	g++ $(FLAGS) -o testPatricia.x ./patricia/patriciaTrie.cpp \
		./patricia/testPatricia.cpp

clean:
	rm -f *.x 