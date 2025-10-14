#include "patriciaTrie.hh"

// els fills d'un node sense fills apunten al mateix node 
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

/*
els tries patricia guarden els bits on hi ha diferencies, podem veure que si 
alguna de les branques (0 o 1) apunta cap a si mateix el node es terminal
*/
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
    if (i < 0) return false;
    // agafem el byte
    size_t index = i / 8;
    if (index >= key.length()) return false;
    // agafem el bit
    size_t bit = 7 - (i & 7);
    return ((unsigned char)key[index] >> bit) & 1;
}

void Trie::insert(const string &key) {
    // verifica els casos especials "" i trie buit
    if (key == "") {
        if (root->getKey() != "") root->setKey("");
        return;
    }
    if (isEmpty()) {
        root->setChild(0, new Node(TERMINALPOS, key));
        //root->setChild(0, new Node(key.length(), key));
        return;
    }
    // root[1] mai tindra elements
    Node* parent = root;
    Node* child = root->getChild(0);

    int pos, keyBitLen = key.length() * 8;
    
    //funcionament identic a Trie::contains(key)
    while ((pos = child->getBitPos()) < keyBitLen && parent->getBitPos() < pos)
    {
        parent = child;
        child = child->getChild(getBit(key, pos));
    }

    // si la paraula ja pertany al trie retornem sense inserir
    const string &childKey = child->getKey();
    if (childKey == key) return;

    int diffBit = 0, overlappedPath = min(keyBitLen, (int)childKey.length()*8);
    parent = root; child = root->getChild(0);

    /*
    busquem el bit on es diferencien la paraula amb les branques actuals
    - A la vegada comprovem que si el bit de diferencia del fill es menor que
      el # de bits iguals aleshores el fill passa a ser node candidat a partir
      del qual penjara la nova branca del trie 
    */
    // cout << "key: " << key << endl;
    while (diffBit <= overlappedPath &&
        getBit(key, diffBit) == getBit(childKey, diffBit))
    {
        ++diffBit;
        // els increments de diffBit son de 1 en 1, per tant, amb un if per 
        // iteracio assegurem que seleccionem el node correcte
        if ((pos = child->getBitPos()) < diffBit) {
            parent = child;
            child = child->getChild(getBit(key, pos));
        }
    }

    Node* newNode = new Node(diffBit, key);
    bool branch = getBit(key, diffBit);

    // fem el node terminal per la branca del bit de diferencia de la paraula
    // inserida
    newNode->setChild(branch, newNode);
    // penjem la part que ja teniem abans a l'altra branca
    newNode->setChild(!branch, child);
    // penjem aquest nou node diferencia al pare del node diferencia que hem
    // substituit
    parent->setChild(getBit(key, parent->getBitPos()), newNode);
}

/*
Comprovem si la paraula ja pertany al trie.
- Busquem el primer link upstream == node terminal. 
- A la vegada comprovem que el bit de diferencia que guarda el child sigui
    menor que el tamany, en bits, de la paraula que volem inserir. En cas
    contrari, veiem que es impossible que la paraula pertanyi al trie.
*/
bool Trie::contains(const string &key) const {
    if (key == "") return root->getKey() == "";
    if (isEmpty()) return false;

    Node* parent = root;
    Node* child = parent->getChild(0);

    // int pos, keyBitLen = key.length() * 8;
    
    // while ((pos = child->getBitPos()) < keyBitLen && parent->getBitPos() < pos)
    // {
    //     parent = child;
    //     child = child->getChild(getBit(key, pos));
    // }
    while (parent->getBitPos() < child->getBitPos()) {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }
    cout << "key: " << key << ", childKey: " << child->getKey() << endl;
    return child->getKey() == key;
}

/*
Al contrari que amb contains aqui no podem fer early return ja que hem 
d'arribar a un node terminal per comprobar si el seu prefix es el que busquem
*/
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
void Trie::getPrefixed(Node* node, set<string> &prefixed) const
{
    // anem recollint recursivament els terminals per cada branca
    //if (node->isTerminal()) prefixed.insert(node->getKey());
    if (node->getKey() != "") prefixed.insert(node->getKey());
    for (int branch = 0; branch < 2; ++branch) {
        Node* child = node->getChild(branch);
        if (node->getBitPos() < child->getBitPos()) {
            getPrefixed(child, prefixed);
        }
    }
}

set<string> Trie::getPrefixed(const std::string &prefix) const {
    if (prefix == "") {
        return root->getKey() == "" ? set<string>{""} : set<string>();
    }
    if (isEmpty()) return set<string>();

    Node* parent = root;
    Node* child = parent->getChild(0);
    Node* endPrefix = child;
    
    int pos, prefixBitLen = prefix.length() * 8;
    /*
    comportament identic a prefix pero guarda l'ultim node del que, en cas
    d'existir el prefix que busquem al trie, penjaran totes les paraules amb
    aquell prefix.  
    */
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
    getPrefixed(endPrefix, prefixed);

    return prefixed;
}

// si el trie te algun element, sempre sera root[0] accessible, si root[0] 
// apunta a ell mateix el trie es buit.
bool Trie::isEmpty() const {
   return root->getChild(0) == root;
}

// unicament per debugar insert/contains
void Trie::printTrie() const {
    if (isEmpty()) {
        cout << "empty trie" << endl;
        return;
    }

    set<string> keys;
    // no hi hauria d'haver alguna cosa guardad aqui??
    cout << root->getChild(0)->getKey() << endl;
    getPrefixed(root->getChild(0), keys);

    cout << "#################" << endl;
    for (const string &s : keys) {
        cout << s << endl;
    }
    cout << "#################" << endl;
}