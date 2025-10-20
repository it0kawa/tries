#include "tst.hh"

//Node:
Tst::Node::Node() : textPos(), c(), left(), mid(), right() {}

Tst::Node::Node(const vector<size_t> &valuesP, const string &cP) : textPos(valuesP), c(cP), left(), mid(), right() {} 

vector<size_t> Tst::Node::getTextPos() const {return textPos;}

string Tst::Node::getCharacter() const {return c;}

typename Tst::Node* Tst::Node::getLeft() {return left;}

typename Tst::Node* Tst::Node::getMid() {return mid;}

typename Tst::Node* Tst::Node::getRight() {return right;}

void Tst::Node::putTextPos(const size_t &val) {this->textPos.push_back(val);};

void Tst::Node::putCharacter(const string &c) {this->c = c;}

void Tst::Node::putLeft(Node* node) {this->left = node;}

void Tst::Node::putMid(Node* node) {this->mid = node;}

void Tst::Node::putRight(Node* node) {this->right = node;}


//Tst:
Tst::Tst() : root(), clength(1) {}

Tst::Tst(const size_t &clengthP) : root(), clength(clengthP) {}

void Tst::put(const string &key, const size_t &val) {
    root = put(root, key, val, 0); // retorna l'arbre amb la key insertada
}

typename Tst::Node* Tst::put(
    Node* node, const string &key, const size_t &val, size_t d) {
    
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
    else node->putTextPos(val);
    return node; // com es recursiu retornara al final l'arbre amb la key insertada
}

vector<size_t> Tst::get(const string &key) const {
    // comprobem si esta la key al arbre (root), si no esta retornem valor default. Si esta, retornem el vector de values (vector amb les posicions on esta aquella paraula al text)
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<size_t>();
    return node->getTextPos();
}

typename Tst::Node* Tst::get(
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