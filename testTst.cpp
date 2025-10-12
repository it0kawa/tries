#include "tst.hh"
#include <iostream>
#include <cassert>

using namespace std;

void test1() {
    pair<string, int> tests[3] = {
        {"hola", 1},
        {"adeu", 2},
        {"si", 3},
    };

    Tst<int> tst;

    for (auto &t : tests) {
        tst.put(t.first, t.second);
    } 
    for (auto &t : tests) {
        cout << tst.get(t.first) << endl;
    }
    for (auto &t : tests) {
        assert(tst.get(t.first) == t.second);
    }
    assert(tst.get("noExisteix") == 0);
    cout << "> test 1 (get/put) correcte" << endl;
}

int main() {
    test1();
}