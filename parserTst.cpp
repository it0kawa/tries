#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include "tst.hh"

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
    //string filename = "test_files/words_alpha.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return 1;
    }

    Tst trie(3);
    string raw_word;
    size_t word_position = 1;

    while (file >> raw_word) {
        string word = clean_word(raw_word);
        if (!word.empty()) {
            trie.insert(word, word_position);
        }
        word_position++;
    }

    trie.printStats();
    // Prova
    string word = clean_word("Alice's");
    vector<size_t> alice_positions = trie.getPositions(word);
    cout << "trobat a: ";
    for (auto p : alice_positions) cout << p << ", ";
    cout << endl;
    //assert(!alice_positions.empty());
    //assert(alice_positions[0] == 35);

    cout << "Alice trobat a " << alice_positions.size() << endl;

    
}

