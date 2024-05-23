//grafos.h
#ifndef GRAFOS_H
#define GRAFOS_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

void construirGrafo(const string& diccionario);
vector<string> generarPosiblesPalabras(const string& letras, const unordered_map<string, vector<string>>& grafo);
void posiblesPalabras(const string& letras);
bool difiereEnUnaLetra(const string& palabra1, const string& palabra2);
bool contieneSimbolosInvalidos(const string& cadena);
// Función para encontrar las posibles palabras a partir de un conjunto de letras
void posiblesPalabras(const std::string& letras, const std::string& diccionario);

#endif // GRAFOS_H