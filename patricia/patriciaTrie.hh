#ifndef PATRICIATRIE_HH
#define PATRICIATRIE_HH

#include <climits>
#include <iostream>
#include <set>
#include <string>

using namespace std;
/*
- Implementacio d'un patricia trie sense:
    - destructor
    - remove

- Falta:
    - documentar una mica, ho faig dema
    - probar una mica que funcioni tot

Problemes amb la implementacio: 
    - string "" generara errors -> No s'accepta string buit.    (Arreglat)
*/

class Trie {
private:
    static const int TERMINALPOS = INT_MAX;
    class Node {
    private:
        int ithBit;
        // empty strong if non-terminal
        string key;
        // 0-branch and 1-branch
        Node* childs[2];
    public:
        Node(int ithBit);
        Node(int ithBit, const string &key);
        
        int getBitPos() const;
        bool isTerminal() const;

        const string& getKey() const;
        void setKey(const string &key);

        Node* getChild(bool branch) const;
        void setChild(bool branch, Node* node);
    };
    
    Node* root;
    void getPrefixed(Node* node, set<string> &prefixed, const string &prefix) const;

public:
    Trie();
    bool getBit(const string &key, int i) const;
    void insert(const string &key);
    bool contains(const string &key) const;
    bool isEmpty() const;
    bool isPrefix(const string &prefix) const;
    set<string> getPrefixed(const string &prefix) const;
};

#endif