#ifndef PATRICIATRIE_HH
#define PATRICIATRIE_HH

#include <climits>
#include <iostream>
#include <set>
#include <string>

using namespace std;
/*
> Implementacio d'un patricia trie sense:
    + destructor
    + remove

> Falta:
    + probar una mica que funcioni tot

> Problemes amb la implementacio: 
    + string "" generara errors -> No s'accepta string buit:
        - Arreglat, mirar Trie::Trie() per explicacio 
    
    + insert falla si fem insert de paraules w1,w2,w3 tals que:
        ordre d'entrada es w1 ... w2 ... w3
        w2 es prefix w1
        w2 es prefix w3

      D'alguna manera la paraula es guarda al trie perque printTrie printa tot
      pero no es guarda al path correcte
*/

class Trie {
private:
    static const int TERMINALPOS = INT_MAX;
    class Node {
    private:
        int ithBit;
        // empty strong if non-terminal
        string key;
        // 0-branch i 1-branch
        Node* childs[2];
    public:
        Node(int ithBit);
        Node(int ithBit, const string &key);
        
        // bit de diferencia sobre key que representa el node
        int getBitPos() const;
        bool isTerminal() const;

        const string& getKey() const;
        void setKey(const string &key);

        Node* getChild(bool branch) const;
        void setChild(bool branch, Node* node);
    };
    
    Node* root;
    void getPrefixed(Node* node, set<string> &prefixed) const;

public:
    Trie();
    // valor (0 o 1) del i-essim bit de key
    bool getBit(const string &key, int i) const;
    void insert(const string &key);
    // retorna cert si key forma part del trie
    bool contains(const string &key) const;
    bool isEmpty() const;
    // retorna cert si existeix almenys una paraula amb prefix `prefix` al trie
    bool isPrefix(const string &prefix) const;
    // retorna el set de paraules prefixades per `prefix` del trie
    set<string> getPrefixed(const string &prefix) const;
    // debug
    void printTrie() const;
};

#endif