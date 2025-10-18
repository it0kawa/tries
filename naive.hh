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

class Naive {
private:
    static constexpr size_t R = 256;
    class Node {
    private:
        // terminal == node on acaba alguna paraula
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
        pair<size_t, size_t> getMemoryUsage() const;
        bool isTerminal() const;
    };
    // naive no guarda un string amb la paraula per tant no te wordMemory
    struct Stats {
        size_t maxHeight = 0;
        size_t totalHeight = 0;
        size_t numNodes = 0;
        size_t numWords = 0;
        size_t totalWordlen = 0;
        size_t staticMemory = 0;
        size_t posMemory = 0;
    };  

    Node* root;

    Node* put(
        Node* node, const string &key, const size_t &val, size_t d);
    Node* get(Node* node, const string &key, size_t d) const;
    void calculateStats(const Node* node, Stats &stats, size_t height) const;

public:
    Naive();
    void put(const string &key, const size_t &val);
    vector<size_t> get(const string &key) const;
    bool contains(const string &key) const;
    void printStats() const;
};

#endif 