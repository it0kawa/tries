#ifndef TRIE_HH
#define TRIE_HH

#include <iostream>
#include <string>

using namespace std;

/*
Implementacio molt basica de trie (de les transparencies) sense:
    - destructor
    - remove
*/

template <typename Value>
class Trie {
private:
    static constexpr int R = 256;
    class Node {
    private:
        Value val;
        Node* next[R];
    public:
        Node();
        Node(const Value &value);
        Value getValue() const;
        Node* getNext(unsigned char c) const;
        void putValue(const Value &val);
        void putNext(unsigned char c, Node* node);
    };

    Node* root;
    Node* put(
        Node* node, const string &key, const Value &val,size_t d);
    Node* get(Node* node, const string &key, size_t d) const;

public:
    Trie();
    void put(const string &key, const Value &val);
    Value get(const string &key) const;
    bool contains(const string &key) const;
};

// implementacio

template <typename Value>
Trie<Value>::Node::Node() : val(), next() {}

template <typename Value>
Trie<Value>::Node::Node(const Value &value): val(value), next() {}

template <typename Value>
Value Trie<Value>::Node::getValue() const { return val; }

template <typename Value>
typename Trie<Value>::Node* Trie<Value>::Node::getNext(unsigned char c) const { 
    return next[c]; 
}

template <typename Value>
void Trie<Value>::Node::putValue(const Value &val) { this->val = val; }

template <typename Value>
void Trie<Value>::Node::putNext(unsigned char c, Node* node) { next[c] = node; }

template <typename Value>
typename Trie<Value>::Node* Trie<Value>::put(
    Node* node, const string &key, const Value &val,size_t d) {

    if (d == key.length()) { node->putValue(val); return node; }
    unsigned char c = key[d];
    if (node->getNext(c) == nullptr) node->putNext(c, new Node());
    return put(node->getNext(c), key, val, d + 1);
}    

template <typename Value>
typename Trie<Value>::Node* Trie<Value>::get(
    Node* node, const string &key, size_t d) const {

    if (node == nullptr) return nullptr;
    if (d == key.length()) return node;
    unsigned char c = key[d];
    return get(node->getNext(c), key, d + 1);
}

template <typename Value>
Trie<Value>::Trie() : root(new Node()) {}

template <typename Value>
void Trie<Value>::put(const string &key, const Value &val) { 
    put(root, key, val, 0);
}

template <typename Value>
Value Trie<Value>::get(const string &key) const {
    Node* node = get(root, key, 0);
    if (node == nullptr) return Value();
    return node->getValue();
}

template <typename Value>
bool Trie<Value>::contains(const string &key) const {
    return get(key) != Value();
}

#endif