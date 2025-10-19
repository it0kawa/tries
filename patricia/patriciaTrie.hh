#ifndef PATRICIATRIE_HH
#define PATRICIATRIE_HH

#include <climits>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

/*
> Implementacio d'un patricia trie sense:
    + destructor
    + remove

> falla getPositions i les stats, probablement problema amb insert/findNode
*/

class PTrie {
private:
    // bit position per identificar nodes terminals facilment
    static const size_t TERMINALNODE = INT_MAX;
    class Node {
    private:
        // node que representa l'i-essim bit de differencia en el trie
        size_t bit;
        // empty string si es intermaedi
        string key;
        // 0-branch i 1-branch
        Node* child[2];
        // posicions que ocupa la paraula dins del text
        vector<size_t> textPos;

    public:
        Node(size_t bit, const string &key = "", int pos = -1);
        // retorna cert si el node es terminal, fals en cas contrari
        bool isTerminal() const;

        // getters 
        size_t getBitPos() const;
        vector<size_t> getTextPos();
        const string& getKey() const;
        Node* getChild(bool branch) const;
        // retorna una tuple de memoria estatica, i dinamica per string i vector
        tuple<size_t, size_t, size_t> getMemoryUsage() const;
        
        // setters
        void addTextPos(int pos);
        void setKey(const string &key);
        void setChild(bool branch, Node* node);
    };

    struct Stats {
        size_t maxHeight = 0;
        size_t totalHeight = 0;
        size_t numNodes = 0;
        size_t numWords = 0;
        size_t totalWordlen = 0;
        size_t staticMemory = 0;
        size_t wordsMemory = 0;
        size_t posMemory = 0;
    };  

    Node* root;

    // afegeix un centinela per satisfer la precondicio descrita a la implementacio
    static void preventPrefix(string &key);
    // comproba que la paraula `key` satisfaci el nostre llenguatge acceptat
    static bool verify(const string &key);
    // valor (0 o 1) del i-essim bit de key
    static bool getBit(const string &key, size_t i);
    
    // helpers
    static void getPrefixed(const Node* node, set<string> &prefixed);
    Node* findNode(const string &key) const;
    void calculateStats(const Node* node, Stats &stats, size_t height) const;

public:
    PTrie();
    // inserta i actualitza el vector de posicions dins del text per la paraula `key`
    void insert(string key, size_t pos);
    // retorna cert si key forma part del trie
    bool contains(string key) const;
    // retorna les posicions on apareix `key`
    vector<size_t> getPositions(string key) const;
    // retorna cert si existeix almenys una paraula amb prefix `prefix` al trie
    bool isPrefix(string prefix) const;
    // retorna el set de paraules prefixades per `prefix` del trie
    set<string> autocompleta(string prefix) const;
    // printa les estadistiques referents al trie
    void printStats() const;
    // retorna True si el trie es buit (nomes conte el node root)
    bool isEmpty() const;
};

#endif