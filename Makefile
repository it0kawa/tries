FLAGS = -Wall -Werror -O2 -std=c++17

all: testNaive testTst testPatricia parserNaive parserPatricia parserOptPatricia

testNaive:
	g++ $(FLAGS) -o testNaive.x testNaive.cpp naive.cpp

testTst:
	g++ $(FLAGS) -o testTst.x testTst.cpp tst.cpp

testPatricia:
	g++ $(FLAGS) -o testPatricia.x ./patricia/patriciaTrie.cpp \
		./patricia/testPatricia.cpp

parserNaive:
	g++ $(FLAGS) -o parserNaive.x Naive.cpp parserNaive.cc
	
parserPatricia:
	g++ $(FLAGS) -o parserPatricia.x ./patricia/patriciaTrie.cpp \
		parserPatricia.cpp

parserOptPatricia:
	g++ $(FLAGS) -o parserOptPatricia.x ./patricia/optimizedPatriciaTrie.cpp \
		parserOptPatricia.cpp
clean:
	rm -f *.x 