#include "patriciaTrie.hh"

using namespace std;

int main() {
    Trie t;
    // molt tonto i cutre, dema ho arreglo una mica amb asserts
    t.insert("aaaaa");
    t.insert("aa");
    t.insert("aabb");
    t.insert("bbb");
    
    cout << "haria de ser: 11110000" << endl;
    cout << t.contains("aaaaa") << endl;
    cout << t.contains("aa") << endl;
    cout << t.contains("aabb") << endl;
    cout << t.contains("bbb") << endl;
    cout << t.contains("aabbb") << endl;
    cout << t.contains("d") << endl;
    cout << t.contains("aaa") << endl;
    cout << t.contains("aaaaaaaaaaaaaaaa") << endl;

    cout << "hauria de ser: 111000" << endl;      
    cout << t.isPrefix("a") << endl;
    cout << t.isPrefix("bbb") << endl;  
    cout << t.isPrefix("aab") << endl;
    cout << t.isPrefix("aaab") << endl;
    cout << t.isPrefix("ab") << endl;
    cout << t.isPrefix("e") << endl;
  
    for (auto s : t.getPrefixed("a")) cout << s << ' ';
    cout << endl;
    return 0;
}