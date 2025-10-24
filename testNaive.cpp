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
        trie.insert(t.first, t.second);
    } 
    for (auto &t : resultsTest) {
        assert(trie.getPositions(t.first) == t.second);
    }
    assert(trie.getPositions("noExisteix") == vector<size_t>());
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
        trie.insert(t.first, t.second);
    } 
    for (auto &t : tests) {
        assert(trie.contains(t.first));
    }
    assert(!trie.contains("noExisteix"));
    trie.printStats();
    cout << "> test 2 (contains) correcte" << endl;
}

void test3() {
    pair<string, int> tests[5] = {
        {"hola", 1},
        {"holaa", 5},
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

    trie.printStats();
    for (auto &t : tests) {
        trie.insert(t.first, t.second);
        trie.printStats();
    } 
    for (auto &t : resultsTest) {
        assert(trie.getPositions(t.first) == t.second);
    }
    assert(trie.getPositions("noExisteix") == vector<size_t>());
    trie.printStats();

    assert(trie.contains("holaa"));
    assert(!trie.contains("noExisteix"));

    cout << "> test 3 correcte" << endl;
}

void test4() {
    pair<string, int> tests[5] = {
        {"hola", 1},
        {"holaa", 5},
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
        trie.insert(t.first, t.second);
    }

    assert(trie.getPositionsAndPathLen("hola").second == 5);
    assert(trie.getPositionsAndPathLen("hola").first == resultsTest[0].second);
    assert(trie.getPositionsAndPathLen("si").second == 3);
    assert(trie.getPositionsAndPathLen("noExisteix").second == 1);
    assert(trie.getPositionsAndPathLen("noExisteix").first == vector<size_t>());
    assert(trie.getPositionsAndPathLen("holao").second == 5);
    assert(trie.containsAndPathLen("hola").first);
    assert(trie.containsAndPathLen("hola").second == 5);
    assert(trie.containsAndPathLen("si").second == 3);
    assert(trie.containsAndPathLen("si").first);
    assert(trie.containsAndPathLen("noExisteix").second == 1);
    assert(!trie.containsAndPathLen("noExisteix").first);
    assert(trie.containsAndPathLen("holao").second == 5);
    assert(!trie.containsAndPathLen("holao").first);

    cout << "> test 4 (get/containsPathLen) correcte" << endl;
}


int main() {
    test1();
    test2();
    test3();
    test4();
}