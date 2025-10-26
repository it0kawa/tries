FLAGS = -Wall -Werror -O2 -std=c++17

all: tests testNaive testTst testPatricia parsers parserNaive parserPatricia parserOptPatricia parserTst \
	exp1patricia exp1optPatricia exp1trie exp1tst exp2patricia exp2optPatricia exp2trie \
	exp2tst exp3 wordCleaner wordGenerator

# tests:
tests: testNaive testTst testPatricia

testNaive:
	g++ $(FLAGS) -o testNaive.x ./tests/testNaive.cpp ./tries/naive.cpp

testTst:
	g++ $(FLAGS) -o testTst.x ./tests/testTst.cpp ./tries/tst.cpp

testPatricia:
	g++ $(FLAGS) -o testPatricia.x ./tests/testPatricia.cpp ./tries/patriciaTrie.cpp

# parsers:
parsers: parserNaive parserPatricia parserOptPatricia parserTst

parserNaive:
	g++ $(FLAGS) -o parserNaive.x ./parsers/parserNaive.cc ./tries/naive.cpp

parserTst:
	g++ $(FLAGS) -o parserTst.x ./parsers/parserTst.cpp ./tries/tst.cpp
	
parserPatricia:
	g++ $(FLAGS) -o parserPatricia.x ./parsers/parserPatricia.cpp ./tries/patriciaTrie.cpp

parserOptPatricia:
	g++ $(FLAGS) -o parserOptPatricia.x ./parsers/parserOptPatricia.cpp ./tries/optimizedPatriciaTrie.cpp

# experiments:
exp1: exp1patricia exp1optPatricia exp1trie exp1tst

exp1optPatricia:
	g++ $(FLAGS) -o exp1optPatricia.x ./experiments/experiment1/exp1optPatricia.cpp \
	./tries/optimizedPatriciaTrie.cpp

exp1patricia:
	g++ $(FLAGS) -o exp1patricia.x ./experiments/experiment1/exp1patricia.cpp \
	./tries/patriciaTrie.cpp
	
exp1trie:
	g++ $(FLAGS) -o exp1trie.x ./experiments/experiment1/exp1trie.cpp \
	./tries/naive.cpp

exp1tst:
	g++ $(FLAGS) -o exp1tst.x ./experiments/experiment1/exp1tst.cpp \
	./tries/tst.cpp

exp2: exp2patricia exp2optPatricia exp2trie exp2tst

exp2optPatricia:
	g++ $(FLAGS) -o exp2optPatricia.x ./experiments/experiment2/exp2optPatricia.cpp \
	./tries/optimizedPatriciaTrie.cpp

exp2patricia:
	g++ $(FLAGS) -o exp2patricia.x ./experiments/experiment2/exp2patricia.cpp \
	./tries/patriciaTrie.cpp

exp2trie:
	g++ $(FLAGS) -o exp2trie.x ./experiments/experiment2/exp2trie.cpp \
	./tries/naive.cpp

exp2tst:
	g++ $(FLAGS) -o exp2tst.x ./experiments/experiment2/exp2tst.cpp \
	./tries/tst.cpp

exp3: 
	g++ $(FLAGS) -o exp3.x ./experiments/experiment3/exp3.cpp ./tries/naive.cpp ./tries/tst.cpp \
	./tries/patriciaTrie.cpp ./tries/optimizedPatriciaTrie.cpp

# utils:
wordCleaner:
	g++ $(FLAGS) -o wordCleaner.x ./utils/wordCleaner.cc

wordGenerator:
	g++ $(FLAGS) -o wordGenerator.x ./utils/wordGenerator.cc

clean:
	rm -f *.x 