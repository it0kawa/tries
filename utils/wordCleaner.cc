#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

string input = "test_files/enwiki-latest-all-titles-in-ns0";
string output = "test_files/enwiki-latest-all-titles-in-ns0_clean";

string clean_word(const string& word) {
    string clean_word;
    for (char c : word) {
        if (isalpha(c)) {
            clean_word += tolower(static_cast<unsigned char>(c));
        }
    }
    return clean_word;
}

int main() {
    ifstream file_in(input);

    if (!file_in.is_open()) {
        cerr << "Error: Could not open file " << input << endl;
        return 1;
    }

    ofstream file_out(output);
    
    string raw_word;
    while (file_in >> raw_word) {
        string word = clean_word(raw_word);
        if (!word.empty()) {
            file_out << word << "\n";
        }
    }

    file_in.close();
    file_out.close();

    return 0;
}
