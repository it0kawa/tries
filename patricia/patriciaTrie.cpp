#include "patriciaTrie.hh"
#include <stdexcept>

///////////////////// Node 
PTrie::Node::Node(int bit, const string &key = "") 
    : bit(bit),     
    key(key)  
{   
    child[0] = this; child[1] = this;
}

bool PTrie::Node::isTerminal() const { return bit == TERMINALNODE; }

int PTrie::Node::getBitPos() const { return bit; }

const string& PTrie::Node::getKey() const { return key; }

void PTrie::Node::setKey(const string &key) { this->key = key; }

PTrie::Node* PTrie::Node::getChild(bool branch) const { return child[branch]; }

// sobreescriu la branca si ja existeix
void PTrie::Node::setChild(bool branch, PTrie::Node* node) {
    child[branch] = node;
}

///////////////////// Trie
/*
Veiem que buscar un string buit seria problematic ja que tots els nodes no
terminals el contenen, la solucio que he pensat ha estat tractar l'string buit
com un cas especial que es guardara a root, per tant, ha de ser inicialitzat
amb un valor diferent a string buit inicialment per a poder comprovar si ha 
estat afegit o no
*/
PTrie::PTrie() : root(new Node(-1, "~")) {}

// si el trie te algun element, sempre sera root[0] accessible, si root[0] 
// apunta a ell mateix el trie es buit.
bool PTrie::isEmpty() const {
   return root->getChild(0) == root;
}


/*
Hi ha unja restriccio que s'ha de cumplir sempre:
    `No hi ha cap paraula que pugui ser prefix d'una altra`

Una manera d'assegurar aquesta premisa necessaria es afegir un centinela al
final de cada paraula. Un punt en contra d'aixo es que no podem guardar cap
paraula la versio original de la qual contingui aquest centinela.
*/
string PTrie::preventPrefix(string &key) {
    key.push_back('\0');
    return key;
}

// esta per decidir si "" es acceptat o no
void PTrie::verify(const string &key) {
    // if (key == "") {
    //     throw invalid_argument("no s'accepta string buit (\"\") al trie");
    // }
    if (key.find('\0') != string::npos) {
        throw invalid_argument("no s'accepten strings que continguin el caracter \\0");
    }
}

/*
Big endian. la part dels ASCII que acostumem a trobar en textos comparteixen
un prefix igual molt mes llarg que el sufix, per tant, el path compression
es lleugerament mes eficient en memoria amb big endian que amb little endian
ja que requereix menys nodes no terminals.
*/ 
bool PTrie::getBit(const string &key, int i) {
    // agafem el byte
    size_t index = i / 8;
    if (index >= key.length() || i < 0) return false;
    // agafem el bit
    size_t bit = 7 - (i & 7);
    return ((unsigned char)key[index] >> bit) & 1;
}

void PTrie::insert(string key) {
    verify(key);
    // verifica els casos especials "" i trie buit
    if (key == "") {
        if (root->getKey() != "") root->setKey("");
        return;
    }

    key = preventPrefix(key);
    if (isEmpty()) {
        root->setChild(0, new Node(TERMINALNODE, key));
        return;
    }
    
    // root[1] mai tindra elements
    Node* parent = root;
    Node* child = root->getChild(0);

    int keylen = key.length() * 8;
    
    // identic a contains, consultar contains per explicacio del early exit
    while (parent->getBitPos() < child->getBitPos() &&
        parent->getBitPos() < keylen)
    {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }

    // si la paraula ja pertany al trie retornem sense inserir
    const string &childKey = child->getKey();
    if (childKey == key) return;
    
    int diffBit = 0;
    int overlappedPath = max(key.length(), childKey.length()) * 8;

    parent = root;
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

    newNode->setChild(branch, new Node(TERMINALNODE, key));
    newNode->setChild(!branch, child);
    // penjem aquest nou node diferencia al pare del node diferencia que hem
    // substituit
    parent->setChild(getBit(key, parent->getBitPos()), newNode);
}

bool PTrie::contains(string key) const {
    verify(key);
    if (key == "") return root->getKey() == "";

    key = preventPrefix(key);
    if (isEmpty()) return false;

    Node* parent = root;
    Node* child = parent->getChild(0);

    int keyBitLen = key.length() * 8;
    
    /*
    Veiem perque podem fer un early exit:
    - Trie garanteix que per tot node, bit parent < bit child.
    Veiem que si entrem a la condicio < keylen, aixo vol dir que parent no es
    terminal, ja que si ho fos el seu fill apuntaria a ell mateix trencant la
    invariant. Per tant, si pare no es terminal i te una diferencia a un bit
    major que keylen, aleshores key no esta al trie

    Per que parent.bit < keylen i no child.bi < keylen? si child terminal i 
    parent no terminal, child.bit = TERMINALNODE > keylen pero encara pot 
    contenir key 
    */
    while (parent->getBitPos() < child->getBitPos() &&
        parent->getBitPos() < keyBitLen)
    {
        parent = child;
        child = child->getChild(getBit(key, child->getBitPos()));
    }

    //cout << "key: " << key << ", childKey: " << child->getKey() << endl;
    return child->getKey() == key;
}


/*
Al contrari que amb contains aqui no podem fer early return ja que hem 
d'arribar a un node terminal per comprobar si el seu prefix es el que busquem
*/
bool PTrie::isPrefix(string prefix) const {
    verify(prefix);
    if (prefix == "") return root->getKey() == "";

    prefix = preventPrefix(prefix);
    
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
void PTrie::getPrefixed(const Node* node, set<string> &prefixed) {
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

set<string> PTrie::getPrefixed(string prefix) const {
    verify(prefix);
    if (prefix == "") {
        return root->getKey() == "" ? set<string>{""} : set<string>();
    }
    prefix = preventPrefix(prefix);

    if (isEmpty()) return set<string>();

    Node* parent = root;
    Node* child = parent->getChild(0);
    Node* endPrefix = child;
    
    int prefixBitLen = prefix.length() * 8;
    /*
    comportament identic a prefix pero guarda l'ultim node del que, en cas
    d'existir el prefix que busquem al trie, penjaran totes les paraules amb
    aquell prefix.  
    */
    while (parent->getBitPos() < child->getBitPos()) {
        if (parent->getBitPos() < prefixBitLen) endPrefix = parent;
        parent = child;
        child = child->getChild(getBit(prefix, child->getBitPos()));
        // if (child->getBitPos() < prefixBitLen) endPrefix = child;
    }
    
    // verifica la precondicio getPrefixed() privat
    if (child->getKey().substr(0, prefix.length()) != prefix) {
        return set<string>();
    }

    set<string> prefixed;
    getPrefixed(endPrefix, prefixed);

    return prefixed;
}