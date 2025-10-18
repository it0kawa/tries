#include <iostream>
#include <cassert>
#include <map>
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
    "~",
    "|!#@$%!",
    "1235123434",
    "1",
    "123",
    " a",
    " ",
    "       a"
};

string noInsertedTests[13] = {
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

    string noInsertedTestsNum[5] = {
        "12",
        "13",
        "2344",
        "1234567890",
        "0"
    };
    PTrie t;

    int pos = 1;
    for (string s : testsNum) {
        t.insert(s, pos);
        ++pos;
    } 
    for (string s : testsNum) {
        assert(t.contains(s));
    }
    for (string s : noInsertedTestsNum) {
        assert(!t.contains(s));
    }
    t.printStats();
    cout << "> test 0 (strings de numeros) correcte\n" << endl;
}


void test1() {
    PTrie t;

    int pos = 1;
    for (string s : tests) {
        t.insert(s, pos);
        ++pos;
    } 
    for (string s : tests) {
        assert(t.contains(s));
    }
    for (string s : noInsertedTests) {
        assert(!t.contains(s));
    }
    t.printStats();
    cout << "> test 1 (insert/contains) correcte\n" << endl;
}

void test2() {
    pair<string, set<string>> isprefix[7] = {
        {"aaaaa", set<string>{"aaaaa", "aaaaab"}},
        {"aa", set<string>{"aaaaa", "aa", "aabb", "aaaaab"}},
        {"a", set<string>{"aaaaa", "aa", "aabb", "aaaaab", "abcdesasdasd"}},
        {" ", set<string>{" ", " a", "       a"}},
        {"|!#", set<string>{"|!#@$%!"}},
        {"1", set<string>{"1235123434", "1", "123",}},
        {"1235", set<string>{"1235123434"}}
    };
    string isNotprefix[7] = {
        "aaaaaa",
        "aaaab",
        "d",
        "?",
        "321",
        "!#@$%!",
        "235123434"
    };
    PTrie t;
    assert(t.autocompleta("~") == set<string>()); 
    size_t pos = 1;
    for (string s : tests) {
        t.insert(s, pos);
    } 

    for (const auto &p : isprefix) {
        assert(t.autocompleta(p.first) == p.second); 
    } 

    for (string s : isNotprefix) {
        assert(t.autocompleta(s) == set<string>()); 
    } 
    t.printStats();
    cout << "> test 2 (autocompleta) correcte\n" << endl;
}

void test3() {
    string isprefix[9] = {
        "aaaaa",
        "aa",
        "a",
        "bbb",
        " ",
        "~",
        "|!#",
        "1",
        "1235"
    };
    string isNotprefix[7] = {
        "aaaaaa",
        "aaaab",
        "d",
        "?",
        "321",
        "!#@$%!",
        "235123434"
    };
    PTrie t;
    size_t pos = 1;
    for (string s : tests) {
        t.insert(s, pos);
    } 
    for (string s : isprefix) {
        assert(t.isPrefix(s)); 
    } 
    for (string s : isNotprefix) {
        assert(!t.isPrefix(s)); 
    } 
    t.printStats();
    cout << "> test 3 (isPrefix) correcte\n" << endl;
}

void test4() {

    PTrie t;
    size_t pos = 0;
    map<string, vector<size_t>> m;
    for (string s : tests) {
        t.insert(s, pos);
        m[s].push_back(pos);
    } 
    for (string s : tests) {
        t.insert(s, pos);
        m[s].push_back(pos);
    } 
    for (string s : noInsertedTests) {
        t.insert(s, pos);
        m[s].push_back(pos);
    } 
    for (string s : noInsertedTests) {
        assert(t.getPositions(s) == m[s]);
    } 
    for (string s : noInsertedTests) {
        assert(t.getPositions(s) == m[s]);
    } 

    assert(t.getPositions("aquest no l'hem afegit!") == vector<size_t>());
    assert(t.getPositions("aaaaa <- hi seria sense aquesta explicacio") == vector<size_t>());
    t.printStats();
    cout << "> test 4 (getPoscions) correcte\n" << endl;
}

void testEdge() {
    PTrie t;
    string edge1 = "blablabla";
    edge1.push_back('\0');
    string edge2 = "bla";
    edge2.push_back('\0');
    edge2 += "bla";
    assert(!t.contains(""));
    assert(!t.contains(edge1));
    assert(!t.contains(edge2));
    
    t.printStats();
    cout << "> test edge cases correcte\n" << endl;
}

int main() {
    test0();
    test1();
    test2();
    test3();
    test4();
    testEdge();
}