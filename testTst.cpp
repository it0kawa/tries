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

    pair<string, vector<int>> resultTest[3] = {
        {"hola", {1, 4}},
        {"adeu", {2}},
        {"si", {3}},
    };

    Tst<int> tst;

    for (auto &t : tests) {
        tst.put(t.first, t.second);
    } 
    
    for (auto &t : resultTest) {
        vector<int> r = tst.get(t.first);
        for (auto &i : r) cout << i << " ";
        cout << endl;
        assert(tst.get(t.first) == t.second);
    }
    assert(tst.get("noExisteix") == vector<int>());

    tst.put("fi", -1);
    assert(tst.get("fi")[0] == -1);

    cout << "> test 1 (get/put) correcte" << endl;
}

void test2() {
    pair<string, int> tests[4] = {
        {"hola", 1},
        {"adeu", 2},
        {"sii", 3},
        {"hola", 4},
    };

    pair<string, vector<int>> resultTest[3] = {
        {"hola", {1, 4}},
        {"adeu", {2}},
        {"sii", {3}},
    };

    Tst<int> tst(2);

    for (auto &t : tests) {
        tst.put(t.first, t.second);
    } 
    for (auto &t : resultTest) {
        assert(tst.get(t.first) == t.second);
    }
    assert(tst.get("noExisteix") == vector<int>());

    cout << "> test 2 (get/put) clength=2 correcte" << endl;
}

int main() {
    test1();
    test2();
}