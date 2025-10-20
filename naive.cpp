#include "naive.hh"

/////////////// node
Naive::Node::Node() : is_terminal(false), textPos(), childs() {}

Naive::Node::Node(const vector<size_t> &vals): textPos(vals), childs() {}

pair<size_t, size_t> Naive::Node::getMemoryUsage() const {
    // no inclou la memoria dinamica
    size_t staticMem = sizeof(*this);
    // afegim la memoria dinamica del vector
    size_t posMem = sizeof(size_t) * textPos.size();
    return {staticMem, posMem};
}

vector<size_t> Naive::Node::getTextPos() const { return textPos; }

Naive::Node* Naive::Node::getChild(unsigned char c) const { 
    return childs[c]; 
}

void Naive::Node::putTextPos(const size_t &val) { 
    textPos.push_back(val); 
    is_terminal = true;
}

void Naive::Node::setChild(unsigned char c, Naive::Node* node) { childs[c] = node; }

Naive::Node* Naive::put(
    Naive::Node* node, const string &key, const size_t &val,size_t d) {

    if (d == key.length()) { node->putTextPos(val); return node; }
    unsigned char c = key[d];
    if (node->getChild(c) == nullptr) node->setChild(c, new Node());
    return put(node->getChild(c), key, val, d + 1);
}    

bool Naive::Node::isTerminal() const { return is_terminal; }


/////////////// trie
Naive::Naive() : root(new Node()) {}

Naive::Node* Naive::get(
    Naive::Node* node, const string &key, size_t d) const {

    if (node == nullptr) return nullptr;
    if (d == key.length()) return node;
    unsigned char c = key[d];
    return get(node->getChild(c), key, d + 1);
}

void Naive::put(const string &key, const size_t &val) { 
    put(root, key, val, 0);
}

vector<size_t> Naive::get(const string &key) const {
    Naive::Node* node = get(root, key, 0);
    return (node == nullptr) ? vector<size_t>() : node->getTextPos();
}

bool Naive::contains(const string &key) const {
    Node* node = get(root, key, 0);
    if (node == nullptr || !node->isTerminal()) return false;
    return true;
}

void Naive::calculateStats(const Node *node, Stats &stats, size_t height) const {
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

void Naive::printStats() const {
    Stats stats;

    calculateStats(root, stats, 0);
    
    float avgHeight = 0, avgWordLen = 0;
    float avgHeightRatioWordLen = 0, avgNodeRatioWordLen = 0;
    if (stats.numWords > 0) {
        avgHeight = (float)stats.totalHeight / stats.numWords;
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
    cout << "> numWords(terminals): " << stats.numWords << "\n";
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