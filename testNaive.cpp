#include <iostream>
#include <cassert>
#include "naive.hh"

using namespace std;

void test1() {
    pair<string, int> tests[4] = {
        {"hola", 1},
        {"adeu", 2},
        {"si", 3},
        {"hola", 4},
    };

    pair<string, vector<size_t>> resultsTest[3] = {
        {"hola", {1, 4}},
        {"adeu", {2}},
        {"si", {3}},
    };

    Naive trie;

    for (auto &t : tests) {
        trie.put(t.first, t.second);
    } 
    for (auto &t : resultsTest) {
        assert(trie.get(t.first) == t.second);
    }
    assert(trie.get("noExisteix") == vector<size_t>());
    trie.printStats();
    cout << "> test 1 (get/put) correcte" << endl;
}

void test2() {
    pair<string, int> tests[3] = {
        {"hola", 1},
        {"adeu", 2},
        {"si", 3},
    };
    Naive trie;
    for (auto &t : tests) {
        trie.put(t.first, t.second);
    } 
    for (auto &t : tests) {
        assert(trie.contains(t.first));
    }
    assert(!trie.contains("noExisteix"));
    trie.printStats();
    cout << "> test 2 (contains) correcte" << endl;
}

int main() {
    test1();
    test2();
}