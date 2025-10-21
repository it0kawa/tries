#ifndef TST_HH
#define TST_HH

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
Implementacio d'un TST basic generic
*/

template <typename Value>
class Tst {
private:
    class Node {
    private:
        vector<Value> values;
        string c;
        Node* left;
        Node* mid;
        Node* right;
    public:
        Node();
        Node(const vector<Value> &values, const string &c);
        vector<Value> getValues() const;
        string getCharacter() const;
        Node* getLeft();
        Node* getMid();
        Node* getRight();
        void putValue(const Value &val);
        void putCharacter(const string &c);
        void putLeft(Node* node);
        void putMid(Node* node);
        void putRight(Node* node);
    };

    Node* root;
    Node* put(Node* node, const string &key, const Value &val, size_t d);
    Node* get(Node* node, const string &key, size_t d) const;

public:
    Tst();
    void put(const string &key, const Value &val);
    vector<Value> get(const string &key) const;
};

//Node:
template <typename Value>
Tst<Value>::Node::Node() : values(), c(), left(), mid(), right() {}

template <typename Value>
Tst<Value>::Node::Node(const vector<Value> &valuesP, const string &cP) : values(valuesP), c(cP), left(), mid(), right() {} 

template <typename Value>
vector<Value> Tst<Value>::Node::getValues() const {return values;}

template <typename Value>
string Tst<Value>::Node::getCharacter() const {return c;}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::Node::getLeft() {return left;}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::Node::getMid() {return mid;}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::Node::getRight() {return right;}

template <typename Value>
void Tst<Value>::Node::putValue(const Value &val) {this->values.push_back(val);};

template <typename Value>
void Tst<Value>::Node::putCharacter(const string &c) {this->c = c;}

template <typename Value>
void Tst<Value>::Node::putLeft(Node* node) {this->left = node;}

template <typename Value>
void Tst<Value>::Node::putMid(Node* node) {this->mid = node;}

template <typename Value>
void Tst<Value>::Node::putRight(Node* node) {this->right = node;}


//Tst:
template <typename Value>
Tst<Value>::Tst() : root() {}

template <typename Value>
void Tst<Value>::put(const string &key, const Value &val) {
    root = put(root, key, val, 0);
}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::put(
    Node* node, const string &key, const Value &val, size_t d) {

    string c(1,key[d]);
    if (node == nullptr) { cout << "put: crea por nodo null: " << c << endl; node = new Node(); node->putCharacter(c); }
    if (c.compare(node->getCharacter()) < 0) {
        cout << "put: entra left: " << c << endl;
        node->putLeft(put(node->getLeft(), key, val, d));
    } else if (c.compare(node->getCharacter()) > 0) {
        cout << "put: entra right: " << c << endl;
        node->putRight(put(node->getRight(), key, val, d));
    } else if (d < key.length() - 1) {
        cout << "put: entra mid: " << c << endl;
        node->putMid(put(node->getMid(), key, val, d + 1));
    } else {
        cout << "put: entra exacto: " << c << " poniendo value: " << val << endl;
        node->putValue(val);
    }
    return node;
}

template <typename Value>
vector<Value> Tst<Value>::get(const string &key) const {
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<Value>();
    return node->getValues();
}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::get(
    Node* node, const string &key, size_t d) const{
    if (node == nullptr) {
        cout << "get: nodo null" << endl;
        return nullptr;
    }
    string c(1,key[d]);
    if (c.compare(node->getCharacter()) < 0) {
        cout << "get: entra left: " << c << endl;
        return get(node->getLeft(), key, d);
    } else if (c.compare(node->getCharacter()) > 0) {
        cout << "get: entra right: " << c << endl;
        return get(node->getRight(), key, d);
    } else if (d < key.length() - 1) {
        cout << "get: entra mid: " << c << endl;
        return get(node -> getMid(), key, d + 1);
    } else {
        cout << "get: nodo encontrado: " << c << endl;
        return node;
    }
}


#endif