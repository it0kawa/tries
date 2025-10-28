#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include "../../tries/naive.hh"
#include "../../tries/tst.hh"
#include "../../tries/patriciaTrie.hh"
#include "../../tries/optimizedPatriciaTrie.hh"

using namespace std;

const int MIN = 3;
const int MAX = 19;
const int NWORDSTESTS = 530000;
const int NPARAULESMITJANA = 5;

vector<int> get_n_random_numbers (int ini, int fi) {
    vector<int> res(NPARAULESMITJANA);
    set<int> reg;
    int i = 0;
    int interval = fi - ini + 1;
    while (i < NPARAULESMITJANA) {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        srand(seed);

        int num = rand() % interval + ini;
        if (reg.find(num) == reg.end()) {
            res[i] = num;
            reg.insert(num);
            i++;
        }
    }

    return res;
}

int main() {
    vector<string> words = vector<string>(NWORDSTESTS);

    Naive naive;
    Tst tst;
    PTrie ptrie;
    OPTrie optrie;

    // mida_petita
    string filename = "./inputs/test_fixe.txt";
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



    
    for (int n = MIN; n <= MAX; n++) {
        cout << "exp n=" << n << endl;

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle (words.begin(), words.end(), std::default_random_engine(seed));

        
        int nwords = pow(2,n);
        for (int i = 0; i < nwords; i++) {
            naive.insert(words[i],i+1);
            tst.insert(words[i],i+1);
            ptrie.insert(words[i],i+1);
            optrie.insert(words[i],i+1);
        }

        // per paraules als tries:
        int ini_words_in_tries = 0;
        int fi_words_in_tries = nwords - 1;

        vector<double> total_time_in_tries(4,0.0);
        vector<double> total_nodes_in_tries(4,0.0);

        vector<int> rand_words_in_tries = get_n_random_numbers(ini_words_in_tries, fi_words_in_tries);

        for (int i = 0; i < NPARAULESMITJANA; i++) {
            auto ini0 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_naive = naive.getPositions(words[rand_words_in_tries[i]]);
            auto end0 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time0 = end0 - ini0;
            total_time_in_tries[0] = total_time_in_tries[0] + time0.count();
            total_nodes_in_tries[0] = total_nodes_in_tries[0] + naive.getPositionsAndPathLen(words[rand_words_in_tries[i]]).second;

            auto ini1 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_tst = tst.getPositions(words[rand_words_in_tries[i]]);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time1 = end1 - ini1;
            total_time_in_tries[1] = total_time_in_tries[1] + time1.count();
            total_nodes_in_tries[1] = total_nodes_in_tries[1] + tst.getPositionsAndPathLen(words[rand_words_in_tries[i]]).second;

            auto ini2 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_ptrie = ptrie.getPositions(words[rand_words_in_tries[i]]);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time2 = end2 - ini2;
            total_time_in_tries[2] = total_time_in_tries[2] + time2.count();
            total_nodes_in_tries[2] = total_nodes_in_tries[2] + ptrie.getPositionsAndPathLen(words[rand_words_in_tries[i]]).second;

            auto ini3 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_optrie = optrie.getPositions(words[rand_words_in_tries[i]]);
            auto end3 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time3 = end3 - ini3;
            total_time_in_tries[3] = total_time_in_tries[3] + time3.count();
            total_nodes_in_tries[3] = total_nodes_in_tries[3] + optrie.getPositionsAndPathLen(words[rand_words_in_tries[i]]).second;
        }

        // per paraules que no estan als tries:
        int ini_words_not_in_tries = nwords;
        int fi_words_not_in_tries = NWORDSTESTS - 1;

        vector<double> total_time_not_in_tries(4,0.0);
        vector<double> total_nodes_not_in_tries(4,0.0);

        vector<int> rand_words_not_in_tries = get_n_random_numbers(ini_words_not_in_tries, fi_words_not_in_tries);

        for (int i = 0; i < NPARAULESMITJANA; i++) {
            auto ini0 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_naive = naive.getPositions(words[rand_words_not_in_tries[i]]);
            auto end0 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time0 = end0 - ini0;
            total_time_not_in_tries[0] = total_time_not_in_tries[0] + time0.count();
            total_nodes_not_in_tries[0] = total_nodes_not_in_tries[0] + naive.getPositionsAndPathLen(words[rand_words_not_in_tries[i]]).second;
            auto ini1 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_tst = tst.getPositions(words[rand_words_not_in_tries[i]]);
            auto end1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time1 = end1 - ini1;
            total_time_not_in_tries[1] = total_time_not_in_tries[1] + time1.count();
            total_nodes_not_in_tries[1] = total_nodes_not_in_tries[1] + tst.getPositionsAndPathLen(words[rand_words_not_in_tries[i]]).second;

            auto ini2 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_ptrie = ptrie.getPositions(words[rand_words_not_in_tries[i]]);
            auto end2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time2 = end2 - ini2;
            total_time_not_in_tries[2] = total_time_not_in_tries[2] + time2.count();
            total_nodes_not_in_tries[2] = total_nodes_not_in_tries[2] + ptrie.getPositionsAndPathLen(words[rand_words_not_in_tries[i]]).second;

            auto ini3 = std::chrono::high_resolution_clock::now();
            vector<size_t> r_optrie = optrie.getPositions(words[rand_words_not_in_tries[i]]);
            auto end3 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time3 = end3 - ini3;
            total_time_not_in_tries[3] = total_time_not_in_tries[3] + time3.count();
            total_nodes_not_in_tries[3] = total_nodes_not_in_tries[3] + optrie.getPositionsAndPathLen(words[rand_words_not_in_tries[i]]).second;
        }

        // calcul de mitjanes
        vector<string> tries_names = {"Naive", "Tst", "Patricia", "Patricia Optimitzat"};
        vector<double> mean_time_in_tries(4, 0.0);
        vector<double> mean_nodes_in_tries(4, 0.0);
        vector<double> mean_time_not_in_tries(4, 0.0);
        vector<double> mean_nodes_not_in_tries(4, 0.0);
        for (int i = 0; i < 4; i++) {
            mean_time_in_tries[i] = total_time_in_tries[i] / (double) NPARAULESMITJANA;
            mean_nodes_in_tries[i] = total_nodes_in_tries[i] / (double) NPARAULESMITJANA;
            mean_time_not_in_tries[i] = total_time_not_in_tries[i] / (double) NPARAULESMITJANA;
            mean_nodes_not_in_tries[i] = total_nodes_not_in_tries[i] / (double) NPARAULESMITJANA;
        }

        // imprimir experiments
        cout << "n ; " << n << "=" << nwords << "w" << endl;

        cout << "temps mitja en paraules del tries" << endl;
        for (int i = 0; i < 4; i++) {
            cout << tries_names[i] << "; ";
            cout << mean_time_in_tries[i] << endl;
        }
        

        cout << "mitja nodes en paraules del tries" << endl;
        for (int i = 0; i < 4; i++) {
            cout << tries_names[i] << "; ";
            cout << mean_nodes_in_tries[i] << endl;
        }

        cout << "temps mitja en paraules que no estan als tries" << endl;
        for (int i = 0; i < 4; i++) {
            cout << tries_names[i] << "; ";
            cout << mean_time_not_in_tries[i] << endl;
        }

        cout << "mitja nodes en paraules que no estan als tries" << endl;
        for (int i = 0; i < 4; i++) {
            cout << tries_names[i] << "; ";
            cout << mean_nodes_not_in_tries[i] << endl;
        }

        cout << "------------" << endl;
    }

    

}
