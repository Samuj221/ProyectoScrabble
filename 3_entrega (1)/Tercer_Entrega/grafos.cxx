//grafos.cxx
#include "grafos.h"
#include "scrabble.h"
#include <fstream>
#include <algorithm>
#include <unordered_set>

unordered_map<string, vector<string>> grafoPalabras;

void construirGrafo(const string& diccionario) {
    ifstream inputFile(diccionario);
    if (!inputFile.is_open()) {
        cerr << "(Archivo no existe) El archivo " << diccionario << " no existe o no puede ser leído." << endl;
        return;
    }

    vector<string> palabras;
    string palabra;
    while (getline(inputFile, palabra)) {
        palabras.push_back(palabra);
    }
    inputFile.close();

    for (const auto& palabra1 : palabras) {
        for (const auto& palabra2 : palabras) {
            if (difiereEnUnaLetra(palabra1, palabra2)) {
                grafoPalabras[palabra1].push_back(palabra2);
            }
        }
    }

    cout << "(Resultado exitoso) Grafo construido correctamente." << endl;
}

bool difiereEnUnaLetra(const string& palabra1, const string& palabra2) {
    if (palabra1.size() != palabra2.size()) {
        return false;
    }

    int diferencias = 0;
    for (size_t i = 0; i < palabra1.size(); ++i) {
        if (palabra1[i] != palabra2[i]) {
            ++diferencias;
            if (diferencias > 1) {
                return false;
            }
        }
    }

    return diferencias == 1;
}

vector<string> generarPosiblesPalabras(const string& letras, const unordered_map<string, vector<string>>& grafo) {
    unordered_set<string> palabrasValidas;
    string letrasSorted = letras;
    sort(letrasSorted.begin(), letrasSorted.end());

    do {
        string palabra;
        for (char c : letrasSorted) {
            if (c != '?') {
                palabra += c;
            }
        }

        for (const auto& par : grafo) {
            if (is_permutation(par.first.begin(), par.first.end(), palabra.begin())) {
                palabrasValidas.insert(par.first);
            }
        }
    } while (next_permutation(letrasSorted.begin(), letrasSorted.end()));

    vector<string> resultado(palabrasValidas.begin(), palabrasValidas.end());
    return resultado;
}

void posiblesPalabras(const string& letras) {
    if (contieneSimbolosInvalidos(letras)) {
        cout << "(Letras inválidas) La cadena letras contiene símbolos inválidos." << endl;
        return;
    }

    vector<string> palabras = generarPosiblesPalabras(letras, grafoPalabras);

    cout << "(Resultado exitoso) Las posibles palabras a construir con las letras " << letras << " son:" << endl;
    for (const auto& palabra : palabras) {
        int puntaje = calcularPuntaje(palabra);
        cout << palabra << " (longitud: " << palabra.size() << ", puntaje: " << puntaje << ")" << endl;
    }
}

bool contieneSimbolosInvalidos(const string& cadena) {
    return !all_of(cadena.begin(), cadena.end(), [](char c) { return isalpha(c) || c == '?'; });
}