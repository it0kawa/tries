#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include "../tries/optimizedPatriciaTrie.hh"

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
    //string filename = "./inputs/moby_dick.txt";
    //string filename = "./inputs/alice_wonderland.txt";
    string filename = "./inputs/words_alpha.txt";
    //string filename = "./inputs/leipzig1m.txt";
    //string filename = "./inputs/enwiki-latest-all-titles-in-ns0";
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
        if (!word.empty()) {
            trie.insert(word, word_position);
        }
        word_position++;
    }
    cout << "done reading" << endl;
    //cout << (int)'a' << endl;
    trie.printStats();
    // Prova
    string w = clean_word("hello");
    pair<bool, size_t> contained = trie.containsAndPathLen(w);
    cout << "paraula '" << w << "' contained? (" << contained.first << endl;
    cout << "path de llargada: " << contained.second << endl;
    cout << "contained? " << trie.contains(clean_word(w)) << endl;
    // bad alloc depenent de la paraula per test wikipedia
    pair<vector<size_t>, size_t> positions = trie.getPositionsAndPathLen(w);
    cout << "trobat " << positions.first.size() << " vegades a: " << endl;
    for (auto p : positions.first) cout << p << ", ";
    cout << endl;
    cout << "path de llargada: " << positions.second << endl;
}

