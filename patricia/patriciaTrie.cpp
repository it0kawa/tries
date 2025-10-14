#include "patriciaTrie.hh"


Trie::Node::Node(int ithBit) : ithBit(ithBit), key("") {
    childs[0] = this; childs[1] = this;
}

Trie::Node::Node(int ithBit, const string &key) 
    : ithBit(ithBit),     
    key(key)  
{   
    childs[0] = this; childs[1] = this;
}

int Trie::Node::getBitPos() const { return ithBit; }

bool Trie::Node::isTerminal() const { 
    return childs[0] == this || childs[1] == this;
}

const string& Trie::Node::getKey() const { return key; }

void Trie::Node::setKey(const string &key) { this->key = key; }

Trie::Node* Trie::Node::getChild(bool branch) const { return childs[branch]; }

// sobreescriu la branca si ja existeix
void Trie::Node::setChild(bool branch, Trie::Node* node) {
    childs[branch] = node;
}

/*
Veiem que buscar un string buit seria problematic ja que tots els nodes no
terminals el contenen, la solucio que he pensat ha estat tractar l'string buit
com un cas especial que es guardara a root, per tant, ha de ser inicialitzat
amb un valor diferent a string buit inicialment per a poder comprovar si ha 
estat afegit o no
*/
Trie::Trie() : root(new Node(-1, "~")) {}

/*
Big endian. la part dels ASCII que acostumem a trobar en textos comparteixen
un prefix igual molt mes llarg que el sufix, per tant, el path compression
es lleugerament mes eficient en memoria amb big endian que amb little endian
ja que requereix menys nodes no terminals.
*/ 
bool Trie::getBit(const string &key, int i) const {
    // agafem el byte
    size_t index = i / 8;
    if (index >= key.length()) return false;
    // agafem el bit
    size_t bit = 7 - (i % 8);
    return ((unsigned char)key[index] >> bit) & 1;
}

void Trie::insert(const string &key) {
    if (key == "") {
        if (root->getKey() != "") root->setKey("");
        return;
    }
    if (isEmpty()) {
        root->setChild(0, new Node(TERMINALPOS, key));
        return;
    }
    // root[1] mai tindra elements
    Node* parent = root;
    Node* child = root->getChild(0);

    int pos, keyBitLen = key.length() * 8;
    
    // busquem el primer link upstream == node terminal
    while ((pos = child->getBitPos()) < keyBitLen && parent->getBitPos() < pos)
    {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }

    const string &childKey = child->getKey();
    if (childKey == key) return;

    // busquem el punt on es diferencien
    int diffBit = 0, overlappedPath = min(keyBitLen, (int)childKey.length()*8);

    parent = root; child = root->getChild(0);

    while (diffBit <= overlappedPath &&
        getBit(key, diffBit) == getBit(childKey, diffBit))
    {
        ++diffBit;
        if ((pos = child->getBitPos()) < diffBit) {
            parent = child;
            child = child->getChild(getBit(key, child->getBitPos()));
        }
    }

    Node* newNode = new Node(diffBit, key);
    bool branch = getBit(key, diffBit);

    // fulla amb la paraula
    newNode->setChild(branch, newNode);
    newNode->setChild(!branch, child);

    parent->setChild(getBit(key, parent->getBitPos()), newNode);
}

bool Trie::contains(const string &key) const {
    if (key == "") return root->getKey() == "";
    if (isEmpty()) return false;

    Node* parent = root;
    Node* child = parent->getChild(0);

    while (parent->getBitPos() < child->getBitPos()) {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }

    return child->getKey() == key;
}

bool Trie::isPrefix(const std::string &prefix) const {
    if (prefix == "") return root->getKey() == "";
    if (isEmpty()) return false;

    Node* parent = root;
    Node* child = parent->getChild(0);

    while (parent->getBitPos() < child->getBitPos()) {
        parent = child;
        child = child->getChild(getBit(prefix, child->getBitPos()));
    }

    return child->getKey().substr(0, prefix.length()) == prefix;
}

/*
    Precondicio: tots els descendents de Node van prefixats per prefix
    ja ve donada per getPrefixed() public
*/
void Trie::getPrefixed(Node* node, set<string> &prefixed, const std::string &prefix) const
{
    if (node->isTerminal()) prefixed.insert(node->getKey());
    
    // anem recollint recursivament per cada branca
    for (int branch = 0; branch < 2; ++branch) {
        Node* child = node->getChild(branch);
        if (node->getBitPos() < child->getBitPos()) {
            getPrefixed(child, prefixed, prefix);
        }
    }
}

set<string> Trie::getPrefixed(const std::string &prefix) const
{
    if (prefix == "") {
        return root->getKey() == "" ? set<string>{""} : set<string>();
    }
    if (isEmpty()) return set<string>();

    Node* parent = root;
    Node* child = parent->getChild(0);
    Node* endPrefix = child;
    
    int pos, prefixBitLen = prefix.length() * 8;

    while (parent->getBitPos() < (pos = child->getBitPos())) {
        parent = child;
        child = child->getChild(getBit(prefix, child->getBitPos()));
        if (pos < prefixBitLen) endPrefix = child;
    }
    
    // verifica la precondicio getPrefixed() privat
    if (child->getKey().substr(0, prefix.length()) != prefix) {
        return set<string>();
    }

    set<string> prefixed;
    getPrefixed(endPrefix, prefixed, prefix);

    return prefixed;
}

bool Trie::isEmpty() const {
   return root->getChild(0) == root;
}
