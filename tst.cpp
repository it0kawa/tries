#include "tst.hh"

//Node:
Tst::Node::Node() : is_terminal(false), textPos(), c(), left(), mid(), right() {}

Tst::Node::Node(const vector<size_t> &valuesP, const string &cP) : is_terminal(false), textPos(valuesP), c(cP), left(), mid(), right() {} 

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

vector<size_t> Tst::Node::getMemoryUsage() const {
    size_t staticMem = sizeof(*this);
    size_t posMem = sizeof(size_t) * textPos.size();
    size_t valMem = sizeof(char) * c.size();
    return {staticMem, posMem, valMem};
}

void Tst::Node::setTerminal(bool b) { is_terminal = b; }

bool Tst::Node::isTerminal() const { return is_terminal; }

//Tst private:
Tst::Tst() : root(), clength(1) {}

Tst::Tst(const size_t &clengthP) : root(), clength(clengthP) {}



typename Tst::Node* Tst::put(
    Node* node, const string &key, const size_t &val, size_t d) {
    
    // caracter c per la posicio d:
    string c = key.substr(d, clength);
    // en cas de fer put d'una key nova, acabarem afegint nodes per aquells caracters que no estiguin al tst
    // en cas de fer put d'una key que ja esta, acabarem insertant un value al vector del node del final de la paraula 
    if (node == nullptr) { node = new Node(); node->putCharacter(c); node->setTerminal(true);}
    // per un caracter c donat i el caracter c' de node,
    //      si c < c' acabem possant c al arbre esquerre
    //      si c > c', acabem possant c al arbre dret
    //      si c = c', llavors c esta a node => no cal afegir res, pero entrem a la branca del mig si encara no sabem que la resta de caracters de la paraula estan al arbre
    //      si c = c', finalment entra i posa un value
    if (c.compare(node->getCharacter()) < 0) node->putLeft(put(node->getLeft(), key, val, d)); 
    else if (c.compare(node->getCharacter()) > 0) node->putRight(put(node->getRight(), key, val, d)); 
    else if (d < key.length() - 1) { node->setTerminal(false); node->putMid(put(node->getMid(), key, val, d + clength)); }
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

typename Tst::Node* Tst::getPositionsAndPathLen(Node* node, const string &key, size_t d, size_t &pathLen) {
    // Si el node inicial es buit, o be anem recorrent nodes i trobem que acaba sent buit, llavors no existeix la paraula sencera al arbre
    if (node == nullptr) return nullptr;
    ++pathLen;
    // caracter c per la posicio d:
    string c = key.substr(d, clength);
    // per un caracter c donat i el caracter c' de node,
    //      si c < c', llavors c nomes pot estar al arbre esquerre
    //      si c > c', llavors c nomes pot estar al arbre dret
    //      si c = c', llavors esta al node del mig, pero entrem a la branca del mig si encara no sabem que la resta de caracters de la paraula estan al arbre
    //      si c = c', finalment sabem que l'ultim caracter, i que tots els anteriors tambe, llavors podem retornar el node (que aquell mateix contindra el vector amb les posicions on esta aquella paraula al text)
    if (c.compare(node->getCharacter()) < 0) return getPositionsAndPathLen(node->getLeft(), key, d, pathLen);
    else if (c.compare(node->getCharacter()) > 0) return getPositionsAndPathLen(node->getRight(), key, d, pathLen);
    else if (d < key.length() - 1) return getPositionsAndPathLen(node -> getMid(), key, d + clength, pathLen);
    else return node;
}

void Tst::calculateStats(Node *node, Stats &stats, size_t height, size_t wordCount) {
    ++stats.numNodes;

    // si un node es fulla segur que la paraula que representa la fulla esta al dataset
    if (node->isTerminal()) {
        ++stats.numNodesTerminals;
        stats.maxHeight = max(stats.maxHeight, height);
        stats.totalHeight += height;
    }
    // si el vector te algun valor llavors aquella paraula que representa el node esta al dataset
    // pero poden haber nodes que estan al dataset pero no son fulles
    if (node->getTextPos().size() > 0) {
        stats.totalWordlen += (wordCount + 1) * node->getTextPos().size();
        stats.numWords += node->getTextPos().size();
    }
    
    vector<size_t> mem = node->getMemoryUsage();
    stats.staticMemory += mem[0];
    stats.posMemory += mem[1];
    stats.valMemory += mem[2];

    if (node->getMid() != nullptr) calculateStats(node->getMid(), stats, height + 1, wordCount + 1);
    if (node->getLeft() != nullptr) calculateStats(node->getLeft(), stats, height + 1, wordCount);
    if (node->getRight() != nullptr) calculateStats(node->getRight(), stats, height + 1, wordCount);
}

Stats Tst::calculateStats() {
    Stats stats;
    if (root == nullptr) {
        stats.totalHeight = -1;
        stats.avgHeight = -1;
        stats.staticMemory = 4;
        stats.totalMemory = stats.staticMemory + stats.nodesMemory;
        return stats; // el cas del Tst buit
    }
    calculateStats(root, stats, 0, 0);

    if (stats.numWords > 0) {
        stats.avgHeight = (float)stats.totalHeight / stats.numNodesTerminals;
        stats.avgWordLen = (float)stats.totalWordlen / stats.numWords;
    }
    if (stats.avgWordLen > 0) {
        stats.avgHeightRatioWordLen = stats.avgHeight / stats.avgWordLen;
        stats.avgNodeRatioWordLen = stats.numNodes / stats.avgWordLen;
    }

    stats.nodesMemory = stats.posMemory + stats.valMemory;
    stats.totalMemory = stats.staticMemory + stats.nodesMemory;

    return stats;
}

// Tst public:

void Tst::insert(string key, const size_t &val) {
    root = put(root, key, val, 0); // retorna l'arbre amb la key insertada
}

vector<size_t> Tst::getPositions(string key) {
    // comprobem si esta la key al arbre (root), si no esta retornem valor default. Si esta, retornem el vector de values (vector amb les posicions on esta aquella paraula al text)
    Node* node = get(root, key, 0);
    if (node == nullptr) return vector<size_t>();
    return node->getTextPos();
}

bool Tst::contains(string key) {
    return getPositions(key).size() > 0;
}

pair<vector<size_t>, size_t> Tst::getPositionsAndPathLen(string key) {
    size_t pathLen = 0;
    Node* node = getPositionsAndPathLen(root, key, 0, pathLen);
    if (node == nullptr) return {vector<size_t>(), pathLen};
    return {node->getTextPos(), pathLen};
}

pair<bool, size_t> Tst::containsAndPathLen(string key) {
    pair<vector<size_t>, size_t> p = getPositionsAndPathLen(key);
    return {p.first.size() > 0, p.second};
}

Stats Tst::getStats() {
    return calculateStats();
}

void Tst::printStats() {
    Stats stats = calculateStats();

    cout << "\n=================================" << endl;
    cout << "       Tst trie stats" << endl;
    cout << "---------------------------------" << endl;
    cout << "> numNodes: " << stats.numNodes << "\n";
    cout << "> numWords(terminals): " << stats.numNodesTerminals << "\n";
    cout << "> maxHeight: " << stats.maxHeight << "\n";
    cout << "> avgHeight: " << stats.avgHeight << "\n";
    cout << "> avg word length: " << stats.avgWordLen << "\n";
    cout << "> avg height/wordLen: " << stats.avgHeightRatioWordLen << "\n";
    cout << "> avg Node/wordLen: " << stats.avgNodeRatioWordLen << endl;
    cout << "\n---------------------------------" << endl;
    cout << "MemoryUsage (bytes): " << stats.totalMemory << endl;
    cout << "----------dividit entre----------" << endl;
    cout << "> guardar les paraules en el trie: " << stats.valMemory << endl;
    cout << "> guardar les posicions en el trie: " << stats.posMemory << endl;
    cout << "> nodes del trie: " << stats.nodesMemory << endl;
    cout << "> altres: " << stats.staticMemory << endl;
    cout << "=================================\n" << endl;
}