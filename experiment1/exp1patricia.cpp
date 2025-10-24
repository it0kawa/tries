#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm> 
#include <chrono>
#include <iomanip>
#include <utility>     
#include <random>      
#include "../patricia/patriciaTrie.hh"

using namespace std;


string clean_word(const string& word) {
    string clean_word;
    for (char c : word) {
        if (isalpha(c)) {
            clean_word += tolower(static_cast<unsigned char>(c));
        }
    }
    return clean_word;
}

int main(int argc, char *argv[]) {
    // --- 1. Validar argumentos de entrada ---
    if (argc != 4) {
        cerr << "Error: Uso incorrecto." << endl;
        cerr << "Uso: " << argv[0] << " <ruta_fichero> <m_base_size> <num_steps>" << endl;
        return 1;
    }

    string filename = argv[1];
    size_t m_base_size = 0;
    size_t num_steps = 0;
    try {
        m_base_size = stoul(argv[2]);
        num_steps = stoul(argv[3]);
        if (m_base_size == 0 || num_steps == 0) throw std::invalid_argument("m y num_steps deben ser > 0");
    } catch (const std::exception& e) {
        cerr << "Error: m_base_size y num_steps deben ser números enteros positivos." << endl;
        return 1;
    }

    // --- 2. Leer TODAS las palabras del fichero a memoria ---
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el fichero " << filename << endl;
        return 1;
    }

    vector<pair<string, size_t>> all_words;
    string raw_word;
    size_t word_position = 1;
    
    while (file >> raw_word) {
        string word = clean_word(raw_word);
        size_t current_pos = word_position++;
        if (!word.empty()) {
            all_words.push_back({word, current_pos});
        }
    }
    file.close();
    cout << "Fichero leído. Total de palabras limpias: " << all_words.size() << endl;

    if (all_words.empty()) {
        cerr << "Error: No se encontraron palabras válidas en el fichero." << endl;
        return 1;
    }

    // --- 3. Barajar (shuffle) el vector para aleatoriedad ---
    std::random_device rd;
    std::mt19937 g(rd()); // Motor de aleatoriedad
    std::shuffle(all_words.begin(), all_words.end(), g);
    cout << "Palabras barajadas aleatoriamente." << endl;


    // --- 4. Imprimir la cabecera del CSV ---
    // N = número total de palabras en ESTE trie
    // TotalInsertionTime_ms = Tiempo para construir el trie de N palabras
    // AvgQueryTime_ns = Tiempo medio de consulta para esas N palabras
    cout << "SourceFile,m_base_size,N,TotalInsertionTime_ms,AvgQueryTime_ns,TotalSpace_bytes" << endl;


    for (int p = 0; p < num_steps; ++p) {
        size_t N = m_base_size * (1 << p);

        if (N > all_words.size()) {
            break;
        }

        // --- A. Crear NUEVO trie e insertar N palabras ---
        PTrie trie; 
        
        auto start_insert = chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < N; ++i) {
            trie.insert(all_words[i].first, all_words[i].second);
        }
        
        auto end_insert = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> total_insert_time_ms = end_insert - start_insert;


        // --- B. Medir Tiempo de Consulta Exitosa (de las N palabras) ---
        auto start_query = chrono::high_resolution_clock::now();

        for (size_t i = 0; i < N; ++i) {
            volatile auto pos = trie.getPositions(all_words[i].first);
        }

        auto end_query = chrono::high_resolution_clock::now();
        chrono::duration<double, nano> query_time_total = end_query - start_query;
        double avg_query_time_ns = query_time_total.count() / N;


        // --- C. Medir Espacio Ocupado ---
        Stats stats = trie.getStats();
        size_t space = stats.totalMemory;


        // --- D. Imprimir resultados (formato CSV) ---
        cout << filename << ","
             << m_base_size << ","
             << N << "," // El número de palabras total en este trie
             << fixed << setprecision(4) << total_insert_time_ms.count() << "," // T. inserción TOTAL
             << fixed << setprecision(2) << avg_query_time_ns << ","           // T. consulta MEDIO
             << space << endl;
    }
    return 0;
}