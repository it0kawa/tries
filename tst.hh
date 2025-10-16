#ifndef TST_HH
#define TST_HH

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
Implementacio d'un TST
    Estructura d'un TST:
        -Arbre de nodes (utilitzant punters a c++), on els nodes tenen un caracter(s) c' (valor del node) i un vector de values (int's per indicar posicions on apareix al text)
        -l'arbre es ternari, on cada nivell es una posicio d'un caracter en una paraula, i, per un node amb caracter c' en un nivell (posicio) qualsevol:
            l'arbre de l'esquerra conte totes les paraules tals que per aquella posicio comencen per un caracter menor
            l'arbre del mig conte totes les paraules tals que per aquella posicio comencen per el caracter c'
            l'arbre de la dreta conte totes les paraules tals que per aquella posicio comencen per un caracter major
        -funcionament: suposem que mirem un caracter c, i estem al node amb caracter c'.
            si c = c' llavors entrem pel node del mig, 
            si c < c' llavors entrem per l'esquerra i comprobarem si el caracter c'' del node de l'esquerra es igual a c per saber on desviarnos, 
            si c > c' llavors entrem per la dreta i comprobarem si el caracter c'' del node de la dreta es igual a c per saber on desviarnos
        -si entrem per un node del mig sabem que existeix el caracter, i passem al seguent caracter de la paraula
        -si no entrem per tots els nodes del mig per cada caracter d'una paraula llavors no tenim aquesta paraula al nostre conjunt de paraules (el tst)
    **Veure costos a transparencies de la documentacio
*/

typedef unsigned int uint;
template <typename Value>
class Tst {
private:
    class Node {
    private:
        vector<Value> values;
        string c; // mida de c per tot node de l'arbre es com a molt clength, l'ultim node d'una paraula pot tindre mida de c = 1 sii la paraula te mida senar
        Node* left;
        Node* mid;
        Node* right;
    public:
        Node();
        Node(const vector<Value> &valuesP, const string &cP);
        // funcions publiques per accedir des de les funcions put i get de la classe Tst. son funcions basiques de getters i setters:
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

    // Comença els atributs i metodes de la classe:
    Node* root;
    uint clength;
    Node* put(Node* node, const string &key, const Value &val, size_t d);
    Node* get(Node* node, const string &key, size_t d) const;

public:
    Tst();
    Tst(const uint &clengthP);
    // funcions publiques per limitar comportament des de l'exterior. Cada funcio crida a una privada
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
Tst<Value>::Tst() : root(), clength(1) {}

template <typename Value>
Tst<Value>::Tst(const uint &clengthP) : root(), clength(clengthP) {}

template <typename Value>
void Tst<Value>::put(const string &key, const Value &val) {
    root = put(root, key, val, 0); // retorna l'arbre amb la key insertada
}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::put(
    Node* node, const string &key, const Value &val, size_t d) {
    
    // caracter c per la posicio d:
    string c = key.substr(d, clength);
    // en cas de fer put d'una key nova, acabarem afegint nodes per aquells caracters que no estiguin al tst
    // en cas de fer put d'una key que ja esta, acabarem insertant un value al vector del node del final de la paraula 
    if (node == nullptr) { node = new Node(); node->putCharacter(c); }
    // per un caracter c donat i el caracter c' de node,
    //      si c < c' acabem possant c al arbre esquerre
    //      si c > c', acabem possant c al arbre dret
    //      si c = c', llavors c esta a node => no cal afegir res, pero entrem a la branca del mig si encara no sabem que la resta de caracters de la paraula estan al arbre
    //      si c = c', finalment entra i posa un value
    if (c.compare(node->getCharacter()) < 0) node->putLeft(put(node->getLeft(), key, val, d)); 
    else if (c.compare(node->getCharacter()) > 0) node->putRight(put(node->getRight(), key, val, d)); 
    else if (d < key.length() - 1) node->putMid(put(node->getMid(), key, val, d + clength));
    else node->putValue(val);
    return node; // com es recursiu retornara al final l'arbre amb la key insertada
}

template <typename Value>
vector<Value> Tst<Value>::get(const string &key) const {
    // comprobem si esta la key al arbre (root), si no esta retornem valor default. Si esta, retornem el vector de values (vector amb les posicions on esta aquella paraula al text)
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<Value>();
    return node->getValues();
}

template <typename Value>
typename Tst<Value>::Node* Tst<Value>::get(
    Node* node, const string &key, size_t d) const{
    // Si el node inicial es buit, o be anem recorrent nodes i trobem que acaba sent buit, llavors no existeix la paraula sencera al arbre
    if (node == nullptr) return nullptr;
    // caracter c per la posicio d:
    string c = key.substr(d, clength);
    // per un caracter c donat i el caracter c' de node,
    //      si c < c', llavors c nomes pot estar al arbre esquerre
    //      si c > c', llavors c nomes pot estar al arbre dret
    //      si c = c', llavors esta al node del mig, pero entrem a la branca del mig si encara no sabem que la resta de caracters de la paraula estan al arbre
    //      si c = c', finalment sabem que l'ultim caracter, i que tots els anteriors tambe, llavors podem retornar el node (que aquell mateix contindra el vector amb les posicions on esta aquella paraula al text)
    if (c.compare(node->getCharacter()) < 0) return get(node->getLeft(), key, d);
    else if (c.compare(node->getCharacter()) > 0) return get(node->getRight(), key, d);
    else if (d < key.length() - 1) return get(node -> getMid(), key, d + clength);
    else return node;
}


#endif