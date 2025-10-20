#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// Number of words to generate
int NWORDS = 500000;

// Lenght of words to generate
int LEN = 9;

// Document name
//string fileName = "test_files/mida_petita.txt";
//string fileName = "test_files/mida_mitjana.txt";
string fileName = "test_files/mida_gran.txt";

string generateWord(const vector<char>& alphabet) {
    string word;
    for (int i = 0; i < LEN; ++i) {
        int j = rand() % alphabet.size();
        word += alphabet[j];
    }
    return word;
}

int main() {
    srand(time(nullptr));

    vector<char> alphabet = {
        'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','z'
    };

    ofstream file(fileName);


    for (int i = 0; i < NWORDS; ++i) {
        string word = generateWord(alphabet);
        file << word << "\n";
    }

    file.close();

    return 0;
}
