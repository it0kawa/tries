#include "naive.hh"

/////////////// node
Naive::Node::Node() : is_terminal(false), textPos(), childs() {}

Naive::Node::Node(const vector<size_t> &vals): is_terminal(false), textPos(vals), childs() {}

vector<size_t> Naive::Node::getMemoryUsage() const {
    // no inclou la memoria dinamica
    size_t staticMem = sizeof(*this);
    // afegim la memoria dinamica del vector
    size_t posMem = sizeof(size_t) * textPos.size();
    size_t valMem = 0;
    return {staticMem, posMem, valMem};
}

vector<size_t> Naive::Node::getTextPos() const { return textPos; }

Naive::Node* Naive::Node::getChild(unsigned char c) const { 
    return childs[c]; 
}

void Naive::Node::putTextPos(const size_t &val) { 
    textPos.push_back(val);
}

void Naive::Node::setChild(unsigned char c, Naive::Node* node) { childs[c] = node; }

void Naive::Node::setTerminal(bool b) { is_terminal = b; }

bool Naive::Node::isTerminal() const { return is_terminal; }

// Naive private
Naive::Naive() : root(new Node()) {}

Naive::Node* Naive::put(
    Naive::Node* node, const string &key, const size_t &val,size_t d) {

    if (d == key.length()) { node->putTextPos(val); return node; }
    unsigned char c = key[d];
    node->setTerminal(false); // si o si es posa un nou node a partir d'aquest
    if (node->getChild(c) == nullptr) { node->setChild(c, new Node()); node->getChild(c)->setTerminal(true); }
    return put(node->getChild(c), key, val, d + 1);
}    

Naive::Node* Naive::get(
    Naive::Node* node, const string &key, size_t d) const {

    if (node == nullptr) return nullptr;
    if (d == key.length()) return node;
    unsigned char c = key[d];
    return get(node->getChild(c), key, d + 1);
}

void Naive::calculateStats(Node *node, Stats &stats, size_t height) {
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
        stats.totalWordlen += height;
        ++stats.numWords;
    }
    for (size_t i = 0; i < R; ++i) {
        Node* child = node->getChild(i);
        if (child == nullptr) continue;
        
        vector<size_t> mem = child->getMemoryUsage();
        stats.staticMemory += mem[0];
        stats.posMemory += mem[1];
        stats.valMemory += mem[2];
        
        calculateStats(child, stats, height + 1);
    }
}

Stats Naive::calculateStats() {
    Stats stats;
    calculateStats(root, stats, 0);

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

// Naive public:
void Naive::insert(const string key, const size_t &val) { 
    put(root, key, val, 0);
}

vector<size_t> Naive::getPositions(string key) {
    Naive::Node* node = get(root, key, 0);
    return (node == nullptr) ? vector<size_t>() : node->getTextPos();
}

bool Naive::contains(const string key)  {
    return getPositions(key).size() > 0;
}

Stats Naive::getStats() {
    return calculateStats();
}

void Naive::printStats() {
    Stats stats = calculateStats();

    cout << "\n=================================" << endl;
    cout << "       Naive trie stats" << endl;
    cout << "---------------------------------" << endl;
    cout << "> numNodes: " << stats.numNodes << "\n";
    cout << "> numWords(terminals): " << stats.numNodesTerminals << "\n";
    cout << "> maxHeight: " << stats.maxHeight << "\n";
    cout << "> avgHeight: " << stats.avgHeight << "\n";
    cout << "> avg word length (chars): " << stats.avgWordLen << "\n";
    cout << "> avg height/wordLen: " << stats.avgHeightRatioWordLen << "\n";
    cout << "> avg Node/wordLen: " << stats.avgNodeRatioWordLen << endl;
    cout << "\n---------------------------------" << endl;
    cout << "MemoryUsage (bytes): " << stats.totalMemory << endl;
    cout << "----------dividit entre----------" << endl;
    cout << "> guardar les paraules en el trie: " << stats.valMemory << endl;
    cout << "> guardar les posicions en el trie: " << stats.posMemory << endl;
    cout << "> nodes del trie: " << stats.nodesMemory << endl;
    cout << "> static memory del trie: " << stats.staticMemory << endl;
    cout << "=================================\n" << endl;
}