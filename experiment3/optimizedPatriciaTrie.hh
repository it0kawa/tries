#ifndef OPTIMIZEDPATRICIATRIE_HH
#define OPTIMIZEDPATRICIATRIE_HH

#include <climits>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <tuple>
#include <memory>

using namespace std;

/*
> Implementacio d'un patricia trie sense:
    + destructor
    + remove

> optimitzacio:
    per estalviarn-nos la memoria estatica del vector i string buits de cada
    node intermediari (tenim aprox n/2 nodes intermediaris per tests grans)

    declararem un vector al trie amb aquesta informacio i als nodes simplement
    guardarem l'index per accedir-hi, sera INDEXINTERMEDIARI per els intermediaris.

    Funciona sota la premisa que mai s'eliminen paraules i que per tant, els
    mai canviaran.
*/

class OPTrie {
private:
    // bit position per identificar nodes terminals facilment
    static constexpr size_t TERMINALNODE = INT_MAX;
    static constexpr size_t INDEXINTERMEDIARI = INT_MAX;
    class Node {
    private:
        // node que representa l'i-essim bit de differencia en el trie
        size_t bit;
        
        size_t index;

        Node* child[2];

    public:
        Node(size_t bit, size_t index=INDEXINTERMEDIARI);
        // retorna cert si el node es terminal, fals en cas contrari
        bool isTerminal() const;

        // getters 
        size_t getBitPos() const;
        size_t getIndex() const;
        Node* getChild(bool branch) const;

        // retorna la memoria (estatica unicament) del node
        size_t getMemoryUsage() const;
        
        // setters
        void setIndex(size_t index);
        void setChild(bool branch, Node* node);
    };

    struct InfoNode {
        string key;
        vector<size_t> textPos;
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
        size_t totalMemory = 0;

        float avgHeight = 0.0;
        float avgWordLen = 0.0;
        float avgHeightRatioWordLen = 0.0;
        float avgNodeRatioWordLen = 0.0;
        float avgNodeRatioWords = 0.0;
    };  


    Node* root;

    // afegeix un centinela per satisfer la precondicio descrita a la implementacio
    static void preventPrefix(string &key);
    // comproba que la paraula `key` satisfaci el nostre llenguatge acceptat
    static bool verify(const string &key);
    // valor (0 o 1) del i-essim bit de key
    static bool getBit(const string &key, size_t i);
    
    // helpers
    void getPrefixed(const Node* node, set<string> &prefixed) const;
    Node* findNode(const string &key) const;
    
    //node management
    vector<InfoNode*> infoNodes;
    
    Node* makeTerminalNode(const string key, size_t pos);

    void addTextPos(const Node* node, size_t pos);
    void setKey(const Node* node, const string key);

    vector<size_t> getTextPos(const Node* node) const;
    const string& getKey(const Node* node) const;

    // per experiments=
    void calculateStats(Stats &stats) const;
    void calculateStatsNode(const Node* node, Stats &stats, size_t height) const;
    void calculateStatsTrie(Stats &stats) const;
    pair<Node*, size_t> findNodeAndPathLen(const string &key) const;

public:
    OPTrie(int numwords = -1);
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

    // retorna True si el trie es buit (nomes conte el node root)
    bool isEmpty() const;

    // per experiments
        // printa les estadistiques referents al trie
    void printStats() const;
        // rettorna les estadistiques referents al trie
    OPTrie::Stats getStats() const;
        // retorna size_t 0 en cas d'error
    pair<vector<size_t>, size_t> getPositionsAndPathLen(string key) const;
    pair<bool, size_t> containsAndPathLen(string key) const;
};

#endif