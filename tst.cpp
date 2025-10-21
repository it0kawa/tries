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

pair<size_t, size_t> Tst::Node::getMemoryUsage() const {
    // no inclou la memoria dinamica
    size_t staticMem = sizeof(*this);
    // afegim la memoria dinamica del vector i del valor c del node
    size_t posMem = sizeof(size_t) * textPos.size() + sizeof(char) * c.size();
    return {staticMem, posMem};
}

//Tst private:
Tst::Tst() : root(), clength(1) {}

Tst::Tst(const size_t &clengthP) : root(), clength(clengthP) {}



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

void Tst::calculateStats(const Node *node, Stats &stats, size_t height) {
    ++stats.numNodes;

    if (node->isTerminal()) {
        stats.maxHeight = max(stats.maxHeight, height);
        stats.totalHeight += height;
        stats.totalWordlen += height;
        ++stats.numWords;
    }
    for (size_t i = 0; i < R; ++i) {
        Node* child = node->getChild(i);
        if (child == nullptr) continue;
        
        pair<size_t, size_t> mem = child->getMemoryUsage();
        stats.staticMemory += mem.first;
        stats.posMemory += mem.second;
        
        calculateStats(child, stats, height + 1);
    }
}

// Tst public:

void Tst::put(const string &key, const size_t &val) {
    root = put(root, key, val, 0); // retorna l'arbre amb la key insertada
}

vector<size_t> Tst::get(const string &key) const {
    // comprobem si esta la key al arbre (root), si no esta retornem valor default. Si esta, retornem el vector de values (vector amb les posicions on esta aquella paraula al text)
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<size_t>();
    return node->getTextPos();
}

Stats Tst::calculateStats() const {
    Stats stats;
    calculateStats(root, stats, 0);
    return stats;
}

void Tst::printStats() const {
    Stats stats;

    calculateStats(root, stats, 0);
    
    float avgHeight = 0, avgWordLen = 0;
    float avgHeightRatioWordLen = 0, avgNodeRatioWordLen = 0;
    if (stats.numWords > 0) {
        avgHeight = (float)stats.totalHeight / stats.numNodesTerminals;
        avgWordLen = (float)stats.totalWordlen / stats.numWords;
    }
    if (avgWordLen > 0) {
        avgHeightRatioWordLen = avgHeight / avgWordLen;
        avgNodeRatioWordLen = stats.numNodes / avgWordLen;
    }

    size_t memory = stats.staticMemory + stats.posMemory;
    cout << "\n=================================" << endl;
    cout << "       Naive trie stats" << endl;
    cout << "---------------------------------" << endl;
    cout << "> numNodes: " << stats.numNodes << "\n";
    cout << "> numWords(terminals): " << stats.numNodesTerminals << "\n";
    cout << "> maxHeight: " << stats.maxHeight << "\n";
    cout << "> avgHeight: " << avgHeight << "\n";
    cout << "> avg word length: " << avgWordLen << "\n";
    cout << "> avg height/wordLen: " << avgHeightRatioWordLen << "\n";
    cout << "> avg Node/wordLen: " << avgNodeRatioWordLen << endl;
    cout << "\n---------------------------------" << endl;
    cout << "MemoryUsage (bytes): " << memory << endl;
    cout << "----------dividit entre----------" << endl;
    cout << "> guardar les paraules: N/A" << endl;
    cout << "> guardar posicions en text: " << stats.posMemory << endl;
    cout << "> altres: " << stats.staticMemory << endl;
    cout << "=================================\n" << endl;
}