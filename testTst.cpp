#include "tst.hh"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

void test1() {
    pair<string, int> tests[4] = {
        {"hola", 1},
        {"adeu", 2},
        {"si", 3},
        {"hola", 4},
    };

    pair<string, vector<size_t>> resultTest[3] = {
        {"hola", {1, 4}},
        {"adeu", {2}},
        {"si", {3}},
    };

    Tst tst;

    for (auto &t : tests) {
        tst.insert(t.first, t.second);
    } 
    
    for (auto &t : resultTest) {
        vector<size_t> r = tst.getPositions(t.first);
        for (auto &i : r) cout << i << " ";
        cout << endl;
        assert(tst.getPositions(t.first) == t.second);
    }
    assert(tst.getPositions("noExisteix") == vector<size_t>());

    tst.insert("fi", 0);
    assert(tst.getPositions("fi")[0] == 0);

    cout << "> test 1 (get/put) correcte" << endl;
}

void test2() {
    pair<string, int> tests[4] = {
        {"hola", 1},
        {"adeu", 2},
        {"sii", 3},
        {"hola", 4},
    };

    pair<string, vector<size_t>> resultTest[3] = {
        {"hola", {1, 4}},
        {"adeu", {2}},
        {"sii", {3}},
    };

    Tst tst(2);

    for (auto &t : tests) {
        tst.insert(t.first, t.second);
    } 
    for (auto &t : resultTest) {
        assert(tst.getPositions(t.first) == t.second);
    }
    assert(tst.getPositions("noExisteix") == vector<size_t>());

    cout << "> test 2 (get/put) clength=2 correcte" << endl;
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

    Tst trie;

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

    cout << "> test 3 (print stats) correcte" << endl;
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

    Tst trie;

    for (auto &t : tests) {
        trie.insert(t.first, t.second);
    }

    assert(trie.contains("adeu"));
    pair<vector<size_t>, size_t> p1 = trie.getPositionsAndPathLen("adeu");
    assert(p1.first == resultsTest[1].second);
    cout << "adeu: " << p1.second << endl;
    assert(trie.containsAndPathLen("adeu").first);
    assert(trie.containsAndPathLen("adeu").second == p1.second);


    assert(!trie.contains("adue"));
    pair<vector<size_t>, size_t> p2 = trie.getPositionsAndPathLen("adue");
    assert(p2.first == vector<size_t>());
    cout << "adue: " << p2.second << endl;
    assert(!trie.containsAndPathLen("adue").first);
    assert(trie.containsAndPathLen("adue").second == p2.second);

    cout << "> test 4 (get/containsPathLen) correcte" << endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();
}