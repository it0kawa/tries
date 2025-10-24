#include "optimizedPatriciaTrie.hh"
#include <stdexcept>


///////////////////// Node
OPTrie::Node::Node(size_t bit, size_t index)
    : bit(bit),
    index(index)
{
    child[0] = this; child[1] = this;
}

bool OPTrie::Node::isTerminal() const {
    //return bit == TERMINALNODE;
    return (child[0] == this) && (child[1] == this);
}

size_t OPTrie::Node::getBitPos() const { return bit; }

size_t OPTrie::Node::getIndex() const { return index; }

void OPTrie::Node::setIndex(size_t index) { this->index = index; }

OPTrie::Node* OPTrie::Node::getChild(bool branch) const { return child[branch]; }

// sobreescriu la branca si ja existeix
void OPTrie::Node::setChild(bool branch, OPTrie::Node* node) {
    child[branch] = node;
}

size_t OPTrie::Node::getMemoryUsage() const {
    // els nodes optimitzats no tenen memoria dinamica
    size_t staticMem = sizeof(*this);
    return staticMem;
}

///////////////////// Trie
OPTrie::OPTrie(int numwords) : root(new Node(0)) {
    if (numwords > 0) infoNodes.reserve(numwords);
}

// si el trie te algun element, sempre sera root[0] accessible, si root[0]
// apunta a ell mateix el trie es buit.
bool OPTrie::isEmpty() const {
   return root->getChild(0) == root;
}

/*
Hi ha unja restriccio que s'ha de cumplir sempre:
    `No hi ha cap paraula que pugui ser prefix d'una altra`

Una manera d'assegurar aquesta premisa necessaria es afegir un centinela al
final de cada paraula. Un punt en contra d'aixo es que no podem guardar cap
paraula la versio original de la qual contingui aquest centinela.
*/
void OPTrie::preventPrefix(string &key) {
    key.push_back('\0');
}

bool OPTrie::verify(const string &key) {
    try {
        if (key == "") {
            throw invalid_argument("no s'accepta string buit (\"\")");
        }
        if (key.find('\0') != string::npos) {
            throw invalid_argument("no s'accepten strings que continguin el caracter nul ('\\0')");
        }
    }
    catch (const exception &e) {
        cerr << "[Error]: no s'ha acceptat l'input -> " << e.what() << endl;
        return false;
    }
    return true;
}

/*
Big endian. la part dels ASCII que acostumem a trobar en textos comparteixen
un prefix igual molt mes llarg que el sufix, per tant, el path compression
es lleugerament mes eficient en memoria amb big endian que amb little endian
ja que requereix menys nodes interemedis.
*/
bool OPTrie::getBit(const string &key, size_t i) {
    // agafem el byte
    size_t index = i / 8;
    if (index >= key.length()) return false;
    // agafem el bit
    size_t bit = 7 - (i & 7);
    return ((unsigned char)key[index] >> bit) & 1;
}

void OPTrie::addTextPos(const Node* node, size_t pos) {
    infoNodes[node->getIndex()]->textPos.push_back(pos);
}

void OPTrie::setKey(const Node *node, const string key) {
    infoNodes[node->getIndex()]->key = key;
}
vector<size_t> OPTrie::getTextPos(const Node* node) const {
    return infoNodes[node->getIndex()]->textPos;
}

const string& OPTrie::getKey(const Node* node) const {
    return infoNodes[node->getIndex()]->key;
}

// precondicio: key ja verificada i conte continela
// postcondicio: crea un nou node amb index `index`
OPTrie::Node* OPTrie::makeTerminalNode(const string key, size_t pos) {
    InfoNode* info = new InfoNode;
    info->textPos.push_back(pos);
    info->key = key;
    infoNodes.push_back(info);
    size_t index = infoNodes.size() - 1;
    return new Node(TERMINALNODE, index);
}

/*
busca un node donada una paraula `key`
precondicio:
    - `key` es correcte
    - s'ha afegit centinela a `key`
    - Trie not empty
postcondicio:
    no necessariament torna un node que guarda `key`.
    + Si existeix un node que guarda `key`, el retorna
    + Si no existeix retorna un node `candidat` que ha trobat pero que no es el
      que originalment buscavem, aquest node pot guardar una paraula totalment
      different a `key`.
*/
OPTrie::Node *OPTrie::findNode(const string &key) const
{
    /*
    // No funciona un early exit pero no se per que
    
    Veiem per que podem fer un early exit:
    - Trie garanteix que per tot node, bit parent < bit child.
    Veiem que si entrem a la condicio < keylen, aixo vol dir que parent es
    terminal o la paraula es mes curta que el path en el que estem. 
    Si == TERMINALNODE es terminal i es el candidat,
    Si no, veiem que aleshores veiem que les paraules en el path de `key` son
    mes llargues que `key` i per tant `key` no pertany al trie 
    */

    Node* parent = root;
    // root[1] mai tindra elements
    Node* child = parent->getChild(0);

    //size_t keyBitLen = key.length() * 8;
    while (parent->getBitPos() < child->getBitPos()) {
        // if (parent->getBitPos() > keyBitLen && !parent->isTerminal()) {
        //     // `key` no esta al trie
        //     return parent;
        // }
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }
    return child;
}

void OPTrie::insert(string key, size_t pos) {
    if (!verify(key)) return;
    preventPrefix(key);

    if (isEmpty()) {
        root->setChild(0, makeTerminalNode(key, pos));
        return;
    }

    Node* child = findNode(key);

    // si la paraula ja pertany al trie retornem sense inserir pero afegim pos
    const string &childKey = getKey(child);
    if (childKey == key) {
        addTextPos(child, pos);
        return;
    }
    size_t diffBit = 0;
    size_t overlappedPath = max(key.length(), childKey.length()) * 8;

    Node* parent = root;
    child = root->getChild(0);

    while (getBit(key, diffBit) == getBit(childKey, diffBit) &&
        diffBit <= overlappedPath)
    {
        ++diffBit;
        // els increments de diffBit son de 1 en 1, per tant, amb un if per
        // iteracio assegurem que seleccionem el node correcte
        if (child->getBitPos() < diffBit) {
            parent = child;
            child = child->getChild(getBit(key, child->getBitPos()));
        }
    }

    Node* newNode = new Node(diffBit);
    bool branch = getBit(key, diffBit);
    newNode->setChild(branch, makeTerminalNode(key, pos));
    newNode->setChild(!branch, child);
    // penjem aquest nou node diferencia al pare del node diferencia que hem
    // substituit
    parent->setChild(getBit(key, parent->getBitPos()), newNode);
}

bool OPTrie::contains(string key) const {
    if (!verify(key) || isEmpty()) return false;
    preventPrefix(key);

    Node* node = findNode(key);
    return getKey(node) == key;
}

// veiem que es identica a findNode pero retorna el vector de posicions
// en comptes d'un boolea
vector<size_t> OPTrie::getPositions(string key) const
{
    vector<size_t> noMatch = vector<size_t>();
    if (!verify(key) || isEmpty()) return noMatch;
    preventPrefix(key);

    Node* node = findNode(key);

    if (getKey(node) == key) return getTextPos(node);
    return noMatch;
}

/*
Al contrari que amb findNode aqui no podem fer early return ja que hem
d'arribar a un node terminal per comprobar si el seu prefix es el que busquem
*/
bool OPTrie::isPrefix(string prefix) const {
    if (!verify(prefix) || isEmpty()) return false;

    string prefixNoCentinela = prefix;
    preventPrefix(prefix);

    Node* parent = root;
    Node* child = parent->getChild(0);

    while (parent->getBitPos() < child->getBitPos()) {
        parent = child;
        child = child->getChild(getBit(prefix, child->getBitPos()));
    }

    string childKey = getKey(child);
    // `prefix` no pot ser prefix d'una paraula mes petita que ell
    if (childKey.length() < prefix.length()) return false;

    // verifica la precondicio getPrefixed()
    return childKey.substr(0, prefix.length() - 1) == prefixNoCentinela;
}

/*
    Precondicio: tots els descendents de Node van prefixats per prefix
    ja ve donada per autocompleta()
*/
// s'ha de treure el centinela
void OPTrie::getPrefixed(const Node* node, set<string> &prefixed) const {
    // anem recollint recursivament els terminals per cada branca
    if (node->isTerminal()) {
        string key = getKey(node); key.pop_back();
        prefixed.insert(key);
    }
    for (size_t branch = 0; branch < 2; ++branch) {
        Node* child = node->getChild(branch);
        if (node->getBitPos() < child->getBitPos()) {
            getPrefixed(child, prefixed);
        }
    }
}

// s'ha de retornar el set de paraules pero sense el centinela
set<string> OPTrie::autocompleta(string prefix) const {
    set<string> noMatch = set<string>();
    if (!verify(prefix)) return noMatch;
    if (isEmpty()) return noMatch;

    /*
    no hem de comptar el centinela jaque per definicio aquest no permet trobar
    si existeixen paraules que comparteixin prefix
    */
    string prefixNoCentinela = prefix;
    preventPrefix(prefix);

    Node* parent = root;
    Node* child = parent->getChild(0);
    Node* endPrefix = child;

    size_t prefixBitLen = prefixNoCentinela.length() * 8;
    /*
    comportament identic a prefix pero guarda l'ultim node del que, en cas
    d'existir el prefix que busquem al trie, penjaran totes les paraules amb
    aquell prefix.
    */
    while (parent->getBitPos() < child->getBitPos()) {
        if (parent->getBitPos() < prefixBitLen) {
            //endPrefix = parent;
            endPrefix = child;
        }
        parent = child;
        child = child->getChild(getBit(prefix, child->getBitPos()));
    }

    string childKey = getKey(child);
    // `prefix` no pot ser prefix d'una paraula mes petita que ell
    if (childKey.length() < prefix.length()) return noMatch;

    // verifica la precondicio getPrefixed()
    if (childKey.substr(0, prefix.length() - 1) != prefixNoCentinela) {
        return noMatch;
    }

    set<string> prefixed;
    getPrefixed(endPrefix, prefixed);

    return prefixed;
}

//// expermients

void OPTrie::calculateStatsNode(const Node *node, Stats &stats, size_t height) const {
    ++stats.numNodes;
    // static, words, pos
    stats.staticMemory = node->getMemoryUsage();

    if (node->isTerminal()) {
        stats.maxHeight = max(stats.maxHeight, height);
        stats.totalHeight += height;
        ++stats.numWords;
        // no es comtpa el centinela
        stats.totalWordlen += getKey(node).length() - 1;
        return;
    }
    else {
        Node* child0 = node->getChild(0);
        Node* child1 = node->getChild(1);
        height += 1;
        calculateStatsNode(child0, stats, height);
        calculateStatsNode(child1, stats, height);
    }
}

void OPTrie::calculateStatsTrie(Stats &stats) const {
    stats.staticMemory += sizeof(InfoNode*) * infoNodes.capacity();
    for (const auto node : infoNodes) {
        stats.staticMemory += sizeof(InfoNode);
        stats.wordsMemory += node->key.capacity() * sizeof(char);
        stats.posMemory += node->textPos.capacity() * sizeof(size_t);
    }
}

void OPTrie::calculateStats(Stats &stats) const {
    calculateStatsNode(root->getChild(0), stats, 1);
    calculateStatsTrie(stats);
}

pair<OPTrie::Node *, size_t> OPTrie::findNodeAndPathLen(const string &key) const {
    Node* parent = root;
    // root[1] mai tindra elements
    Node* child = parent->getChild(0);

    size_t path = 1;
    while (parent->getBitPos() < child->getBitPos()) {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
        ++path;
    }
    return {child, path};
}

pair<vector<size_t>, size_t> OPTrie::getPositionsAndPathLen(string key) const {
    vector<size_t> noMatch = vector<size_t>();
    if (!verify(key) || isEmpty()) return {noMatch, 0};
    preventPrefix(key);

    pair<Node*, size_t> p = findNodeAndPathLen(key);
    
    if (getKey(p.first) == key) return {getTextPos(p.first), p.second};
    return {noMatch, 0};
}

pair<bool, size_t> OPTrie::containsAndPathLen(string key) const {
    if (!verify(key) || isEmpty()) return {false, 0};
    preventPrefix(key);

    pair<Node*, size_t> p = findNodeAndPathLen(key);
    return {getKey(p.first) == key, p.second};
}

OPTrie::Stats OPTrie::getStats() const {
    Stats stats;

    // root es un dummy
    if (!isEmpty()) calculateStats(stats);

    if (stats.numWords > 0) {
        stats.avgHeight = (float)stats.totalHeight / stats.numWords;
        stats.avgWordLen = (float)stats.totalWordlen / stats.numWords;
        stats.avgNodeRatioWords = (float)stats.numNodes / stats.numWords;
    }
    if (stats.avgWordLen > 0) {
        stats.avgHeightRatioWordLen = stats.avgHeight / stats.avgWordLen;
        stats.avgNodeRatioWordLen = stats.numNodes / stats.avgWordLen;
    }
    stats.totalMemory = stats.staticMemory + stats.wordsMemory + stats.posMemory;
    
    return stats;
}

void OPTrie::printStats() const {
    Stats stats = getStats();

    cout << "\n=================================" << endl;
    cout << "    OPT Patricia trie stats" << endl;
    cout << "---------------------------------" << endl;
    cout << "> numNodes: " << stats.numNodes << "\n";
    cout << "> numWords(terminals): " << stats.numWords << "\n";
    cout << "> maxHeight (bits): " << stats.maxHeight << "\n";
    cout << "> avgHeight (bits): " << stats.avgHeight << "\n";
    cout << "> avg word length (bytes): " << stats.avgWordLen << "\n";
    cout << "> avg height/wordLen (bytes): " << stats.avgHeightRatioWordLen << "\n";
    cout << "> avg Nodes/wordLen: " << stats.avgNodeRatioWordLen << "\n";
    cout << "> avg Nodes/#words: " << stats.avgNodeRatioWords << endl;
    cout << "\n---------------------------------" << endl;
    cout << "MemoryUsage (bytes): " << stats.totalMemory << endl;
    cout << "----------dividit entre----------" << endl;
    cout << "> guardar les paraules: " << stats.wordsMemory << endl;
    cout << "> guardar posicions en text: " << stats.posMemory << endl;
    cout << "> static node memory: " << stats.staticMemory << endl;
    cout << "=================================\n" << endl;
}