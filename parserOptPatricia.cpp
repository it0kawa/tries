#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include "./patricia/optimizedPatriciaTrie.hh"

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
    //string filename = "test_files/moby_dick.txt";
    //string filename = "test_files/alice_wonderland.txt";
    //string filename = "test_files/words_alpha.txt";
    string filename = "test_files/leipzig1m.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    OPTrie trie;
    string raw_word;
    size_t word_position = 1;
    while (file >> raw_word) {
        string word = clean_word(raw_word);
        //string word = raw_word;
        if (!word.empty()) {
            //cout << word << endl;
            trie.insert(word, word_position);
            //trie.printStats();
        }
        word_position++;
    }

    //cout << (int)'a' << endl;
    trie.printStats();
    // Prova
    string word = clean_word("a");
    //string w = clean_word("eoeppdx");
    // vector<size_t> alice_positions = trie.getPositions(w);
    // cout << "trobat a: ";
    // for (auto p : alice_positions) cout << p << ", ";
    // cout << endl;
    //cout << "autocompleta: (" << word << ")" << endl;
    //set<string> s = trie.autocompleta(word);
    //for (string w : s) cout << w << ", ";
    //cout << endl;
}

