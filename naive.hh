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

template <typename Value>
class Naive {
private:
    static constexpr int R = 256;
    class Node {
    private:
        vector<Value> values;
        Node* childs[R];
    public:
        Node();
        Node(const vector<Value> &vals);
        vector<Value> getValues() const;
        Node* getChild(unsigned char c) const;
        void putValue(const Value &val);
        void setChild(unsigned char c, Node* node);
    };

    Node* root;
    Node* put(
        Node* node, const string &key, const Value &val,size_t d);
    Node* get(Node* node, const string &key, size_t d) const;

public:
    Naive();
    void put(const string &key, const Value &val);
    vector<Value> get(const string &key) const;
    bool contains(const string &key) const;
};

// implementacio

template <typename Value>
Naive<Value>::Node::Node() : values(), childs() {}

template <typename Value>
Naive<Value>::Node::Node(const vector<Value> &vals): values(vals), childs() {}

template <typename Value>
vector<Value> Naive<Value>::Node::getValues() const { return values; }

template <typename Value>
typename Naive<Value>::Node* Naive<Value>::Node::getChild(unsigned char c) const { 
    return childs[c]; 
}

template <typename Value>
void Naive<Value>::Node::putValue(const Value &val) { values.push_back(val); }

template <typename Value>
void Naive<Value>::Node::setChild(unsigned char c, Node* node) { childs[c] = node; }

template <typename Value>
typename Naive<Value>::Node* Naive<Value>::put(
    Node* node, const string &key, const Value &val,size_t d) {

    if (d == key.length()) { node->putValue(val); return node; }
    unsigned char c = key[d];
    if (node->getChild(c) == nullptr) node->setChild(c, new Node());
    return put(node->getChild(c), key, val, d + 1);
}    

template <typename Value>
typename Naive<Value>::Node* Naive<Value>::get(
    Node* node, const string &key, size_t d) const {

    if (node == nullptr) return nullptr;
    if (d == key.length()) return node;
    unsigned char c = key[d];
    return get(node->getChild(c), key, d + 1);
}

template <typename Value>
Naive<Value>::Naive() : root(new Node()) {}

template <typename Value>
void Naive<Value>::put(const string &key, const Value &val) { 
    put(root, key, val, 0);
}

template <typename Value>
vector<Value> Naive<Value>::get(const string &key) const {
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<Value>();
    return node->getValues();
}

template <typename Value>
bool Naive<Value>::contains(const string &key) const {
    return get(key) != vector<Value>();
}

#endif