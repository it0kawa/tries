#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <chrono>
#include "../../tries/naive.hh"

using namespace std;

int main()
{
    //string filename = "./inputs/alice_wonderland_clean.txt";
    //string filename = "./inputs/leipzig1m_clean.txt";
    //string filename = "./inputs/words_alpha_clean.txt";
    string filename = "./inputs/moby_dick_clean.txt";

    // Wiki peta, no basten 32GB de ram
    //string filename = "../../inputs/enwiki-latest-all-titles-in-ns0_clean";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    cout << "File tested --> " << filename << endl;

    Naive trie;
    size_t word_position = 1;
    string word;
    auto ini = std::chrono::high_resolution_clock::now();
    while (file >> word) {
        trie.insert(word, word_position);
        word_position++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - ini;
    cout << "Insertion time --> " << time.count() << endl;

    trie.printStats();
    cout << "End of program" << endl;
    
}

