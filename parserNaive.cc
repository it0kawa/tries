#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include "naive.hh"

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
    string filename = "test_files/alice_wonderland.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    Naive trie;
    string raw_word;
    size_t word_position = 1;

    while (file >> raw_word) {
        string word = clean_word(raw_word);
        if (!word.empty()) {
            trie.put(word, word_position);
        }
        word_position++;
    }


    // Prova
    vector<size_t> alice_positions = trie.get("alice");
    assert(!alice_positions.empty());
    assert(alice_positions[0] == 35);

    cout << "Alice trobat a " << alice_positions.size() << " vegades, sa primera a posicio " << alice_positions[0] << "." << endl;
}

