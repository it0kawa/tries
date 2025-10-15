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

> Problemes amb la implementacio: 
    + string "" generara errors -> No s'accepta string buit:
        - Arreglat, mirar Trie::Trie() per explicacio 
    
    + insert falla:
        - Arreglat, no verificavem restriccio que s'ha de cumplir sempre:
            `No hi ha cap paraula que pugui ser prefix d'una altra`

> Falta:
    + netejar, comentar i potser optimitzar alguna cosa
    + l'anterior i testejar isPrefix i getPrefixed
*/

class PTrie {
private:
    static const int TERMINALNODE = INT_MAX;
    class Node {
    private:
        int bit;
        // empty strong if non-terminal
        string key;
        // 0-branch i 1-branch
        Node* child[2];
    public:
        Node(int bit, const string &key);
        bool isTerminal() const;
        // bit de diferencia sobre key que representa el node
        int getBitPos() const;

        const string& getKey() const;
        void setKey(const string &key);

        Node* getChild(bool branch) const;
        void setChild(bool branch, Node* node);
    };
    
    Node* root;
    static void getPrefixed(const Node* node, set<string> &prefixed);

public:
    PTrie();
    // valor (0 o 1) del i-essim bit de key
    static string preventPrefix(string &key);
    static void verify(const string &key);
    static bool getBit(const string &key, int i);
    void insert(string key);
    // retorna cert si key forma part del trie
    bool contains(string key) const;
        // retorna cert si existeix almenys una paraula amb prefix `prefix` al trie
    bool isPrefix(string prefix) const;
    // retorna el set de paraules prefixades per `prefix` del trie
    set<string> getPrefixed(string prefix) const;
    bool isEmpty() const;
};

#endif