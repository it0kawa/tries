#include <iostream>
#include <cassert>
#include "patriciaTrie.hh"

using namespace std;

string tests[17] = {
    "A",
    "aaaaa",
    "BB",
    "aa",
    "aaaaab",
    "aabb",
    "bbb",
    "abcdesasdasd",
    "xaxasdasd",
    "",
    "~",
    "|!#@$%!",
    "1235123434",
    "1",
    "123",
    " a",
    " "
};

string NoInsertedTests[13] = {
    "aaaaaa",
    "a",
    "aab",
    "b",
    "abc",
    "x",
    "buit",
    "dasd",
    " ~",
    "12353434",
    " 1",
    "  ",
    "|!#@$%"
};

void test0() {
    string testsNum[5] = {
        "1",
        "123",
        "234",
        "123456767",
        "00000"
    };

    string NoInsertedTestsNum[5] = {
        "12",
        "13",
        "2344",
        "1234567890",
        "0"
    };
    PTrie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : testsNum) {
        t.insert(s);
    } 
    for (string s : testsNum) {
        //cout << s << endl;
        assert(t.contains(s));
    }
    for (string s : NoInsertedTestsNum) {
        assert(!t.contains(s));
    }
    cout << "> test 0 (strings de numeros) correcte" << endl;
}


void test1() {
    PTrie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : tests) {
        t.insert(s);
    } 
    //t.printTrie();
    for (string s : tests) {
        //cout << s << endl;
        assert(t.contains(s));
    }
    for (string s : NoInsertedTests) {
        //cout << s << endl;
        assert(!t.contains(s));
    }
    cout << "> test 1 (insert/contains) correcte" << endl;
}



void test4() {
    string tests2[14] = {
        "aaaaa",
        "aa",
        "aabb",
        "bbb",
        "abcdesasdasd",
        "xaxasdasd",
        " ",
        "  ~",
        "1235123434",
        "1",
        "123",
        " ",
        " a",
        "|!#@$%!"
    };
    PTrie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : tests2) {
        t.insert(s);
    } 
    
    assert(!t.contains("~"));
    assert(!t.contains(""));
    cout << "> test 4 (string buit i `~`) correcte" << endl;
}

int main() {
    test0();
    test1();
    test4();
}