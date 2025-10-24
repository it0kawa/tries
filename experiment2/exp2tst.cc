#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <chrono>
#include "../tst.hh"

using namespace std;


string clean_word(const string& word) {
    string clean_word;
    for (char c : word) {
        if (isalpha(c)) {
            clean_word += tolower(static_cast<unsigned char>(c));
        }
    }
    return clean_word;
}

int main()
{
    //string filename = "../test_files/alice_wonderland_clean.txt";
    //string filename = "../test_files/leipzig1m_clean.txt";
    //string filename = "../test_files/words_alpha_clean.txt";
    //string filename = "../test_files/moby_dick_clean.txt";
    string filename = "../test_files/enwiki-latest-all-titles-in-ns0_clean";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    Tst trie;
    cout << "File tested --> " << filename << endl;

    string word;
    size_t word_position = 1;
    auto ini = std::chrono::high_resolution_clock::now();
    while (file >> word) {
        trie.insert(word, word_position);
        word_position++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - ini;
    cout << "Insertion time --> " << time.count() << endl;

    trie.printStats();
    cout << "end of program" << endl;
    
}

