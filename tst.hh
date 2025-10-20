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

class Tst {
private:
    class Node {
    private:
        vector<size_t> textPos;
        string c; // mida de c per tot node de l'arbre es com a molt clength, l'ultim node d'una paraula pot tindre mida de c = 1 sii la paraula te mida senar
        Node* left;
        Node* mid;
        Node* right;
    public:
        Node();
        Node(const vector<size_t> &valuesP, const string &cP);
        // funcions publiques per accedir des de les funcions put i get de la classe Tst. son funcions basiques de getters i setters:
        vector<size_t> getTextPos() const;
        string getCharacter() const;
        Node* getLeft();
        Node* getMid();
        Node* getRight();
        void putTextPos(const size_t &val);
        void putCharacter(const string &c);
        void putLeft(Node* node);
        void putMid(Node* node);
        void putRight(Node* node);
    };

    // Comença els atributs i metodes de la classe:
    Node* root;
    size_t clength;
    Node* put(Node* node, const string &key, const size_t &val, size_t d);
    Node* get(Node* node, const string &key, size_t d) const;

public:
    Tst();
    Tst(const size_t &clengthP);
    // funcions publiques per limitar comportament des de l'exterior. Cada funcio crida a una privada
    void put(const string &key, const size_t &val);
    vector<size_t> get(const string &key) const;
};

#endif