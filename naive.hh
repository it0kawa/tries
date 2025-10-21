#ifndef NAIVE_HH
#define NAIVE_HH

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
Implementacio molt basica de trie (de les transparencies) sense:
    - destructor
    - remove
*/

// naive no guarda un string amb la paraula per tant no te wordMemory
struct Stats {
    size_t maxHeight = 0;
    size_t totalHeight = 0;
    size_t numNodes = 0;
    size_t numNodesTerminals = 0;
    size_t numWords = 0;
    size_t totalWordlen = 0;
    size_t staticMemory = 0;
    size_t posMemory = 0;
    size_t valMemory = 0;
    size_t nodesMemory = 0;
    size_t totalMemory = 0;

    float avgHeight = 0.0;
    float avgWordLen = 0.0;
    float avgHeightRatioWordLen = 0.0;
    float avgNodeRatioWordLen = 0.0;
};  

class Naive {
private:
    static constexpr size_t R = 256;
    class Node {
    private:
        // terminal == node si es fulla
        bool is_terminal; 
        vector<size_t> textPos;
        Node* childs[R];
    public:
        Node();
        Node(const vector<size_t> &vals);
        vector<size_t> getTextPos() const;
        Node* getChild(unsigned char c) const;
        void putTextPos(const size_t &val);
        void setChild(unsigned char c, Node* node);
        vector<size_t> getMemoryUsage() const;
        void setTerminal(bool b);
        bool isTerminal() const;
    };
    

    Node* root;
    Node* put(
        Node* node, const string &key, const size_t &val, size_t d);
    Node* get(Node* node, const string &key, size_t d) const;
    void calculateStats(const Node* node, Stats &stats, size_t height);
    Stats calculateStats();

public:
    Naive();
    void put(const string &key, const size_t &val);
    vector<size_t> get(const string &key) const;
    bool contains(const string &key) const;
    Stats getStats();
    void printStats();
};

#endif 