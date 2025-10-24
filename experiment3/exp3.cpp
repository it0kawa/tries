#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>
#include "../naive.hh"
#include "../tst.hh"
#include "../patricia/patriciaTrie.hh"
#include "../patricia/optimizedPatriciaTrie.hh"

using namespace std;

const string TEMPS = "temps";
const string NODES_VISITATS = "nodes visitats";
const int MIN = 3;
const int MAX = 19;
const int NWORDSTESTS = 530000;

int main() {
    vector<string> words = vector<string>(NWORDSTESTS);

    Naive naive;
    Tst tst;
    PTrie ptrie;
    OPTrie optrie;

    // mida_petita
    string filename = "../test_files/mida_petita.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }
    string raw_word;
    int i = 0;
    while (file >> raw_word) {
        if (!raw_word.empty()) {
            words[i] = raw_word;
            i++;
        }
    }

    for (int n = MIN; n < MAX; n++) {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (words.begin(), words.end(), std::default_random_engine(seed));

        int nwords = pow(2,n);
        for (int i = 0; i < nwords; i++) {
            naive.insert(words[i], i+1);
            tst.insert(words[i],i+1);
            ptrie.insert(words[i],i+1);
            optrie.insert(words[i],i+1);
        }

        int words_in_tries = nwords; // 0..nwords
        int words_not_in_tries = NWORDSTESTS - nwords; // nwords..NWORDSTESTS

        //
    }

}
