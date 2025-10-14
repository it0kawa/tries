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
    "",
    "dasd",
    "~",
    "12353434",
    "1",
    " ",
    "|!#@$%!"
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
    Trie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : testsNum) {
        t.insert(s);
    } 
    for (string s : testsNum) {
        cout << s << endl;
        assert(t.contains(s));
    }
    for (string s : NoInsertedTestsNum) {
        assert(!t.contains(s));
    }
    cout << "> test 0 (strings de numeros) correcte" << endl;
}


void test1() {
    Trie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : tests) {
        t.insert(s);
    } 
    t.printTrie();
    for (string s : tests) {
        cout << s << endl;
        assert(t.contains(s));
    }
    for (string s : NoInsertedTests) {
        assert(!t.contains(s));
    }
    cout << "> test 1 (insert/contains) correcte" << endl;
}

void test2() {
    string isprefix[10] = {
        "aaaaa",
        "aa",
        "a",
        "bbb",
        "",
        " ",
        "~",
        "|!#",
        "1",
        "1235"
    };
    string isNotprefix[7] = {
        "aaaaaa",
        "aaaaab",
        "d",
        "?",
        "321",
        "!#@$%!",
        "235123434"
    };
    Trie t;
    assert(!t.isPrefix("")); 
    assert(!t.isPrefix("~")); 
    for (string s : tests) {
        t.insert(s);
    } 
    for (string s : isprefix) {
        cout << s << endl;
        assert(t.isPrefix(s)); 
    } 
    for (string s : isNotprefix) {
        assert(!t.isPrefix(s)); 
    } 
    
    cout << "> test 2 (isprefix) correcte" << endl;
}

void test3() {
    pair<string, set<string>> isprefix[8] = {
        {"aaaaa", set<string>{"aaaaa"}},
        {"aa", set<string>{"aaaaa", "aa", "aabb"}},
        {"a", set<string>{"aaaaa", "aa", "aabb", "abcdesasdasd"}},
        {"", set<string>{""}},
        {" ", set<string>{" ", " a"}},
        {"|!#", set<string>{"|!#@$%!"}},
        {"1", set<string>{"1235123434", "1", "123",}},
        {"1235", set<string>{"1235123434"}}
    };
    string isNotprefix[7] = {
        "aaaaaa",
        "aaaaab",
        "d",
        "?",
        "321",
        "!#@$%!",
        "235123434"
    };
    Trie t;
    assert(t.getPrefixed("~") == set<string>()); 
    assert(t.getPrefixed("") == set<string>()); 
    for (string s : tests) {
        t.insert(s);
    } 

    for (const auto &p : isprefix) {
        assert(t.getPrefixed(p.first) == p.second); 
    } 

    for (string s : isNotprefix) {
        assert(t.getPrefixed(s) == set<string>()); 
    } 
    
    cout << "> test 3 (getprefixed) correcte" << endl;
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
    Trie t;
    assert(!t.contains("~"));
    assert(!t.contains(""));

    for (string s : tests) {
        t.insert(s);
    } 
    
    assert(!t.contains("~"));
    assert(!t.contains(""));
    cout << "> test 4 (string buit i `~`) correcte" << endl;
}

int main() {
    //test0();
    test1();
    test2();
    test3();
    test4();
}