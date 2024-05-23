#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <map>

// Funciones existentes
void mostrarPuntajes(const std::vector<std::string>& nombres, const std::vector<int>& puntajes);
void inicializarDiccionario(const std::string& archivo);
void iniciarDiccionarioInverso(const std::string& archivo, const std::string& salida, std::queue<std::string>& mensajes);
void toLowerCase(std::string& str);
bool esPalabraValida(const std::string& palabra, const std::string& diccionario);
bool contieneSimbolosInvalidos(const std::string& palabra);
int calcularPuntaje(const std::string& palabra);
void obtenerPuntaje(const std::string& palabra, const std::string& diccionario, const std::string& diccionarioInverso, bool simulacion);

int leerVariablePersistente();
void escribirVariablePersistente(int valor);

void mostrarAyuda();

void construirGrafoDePalabras(const std::set<std::string>& palabras, std::map<std::string, std::set<std::string>>& grafo);
void mostrarGrafoDePalabras(const std::map<std::string, std::set<std::string>>& grafo);
void mostrarGrafoPalabras(const std::string& letras);
// Función para generar todas las permutaciones de un conjunto de letras
// Función para mostrar el grafo de palabras
void grafoDePalabras(const std::string& diccionario);
void posiblesPalabras(const std::string& letras, const std::string& diccionario);
std::set<std::string> generarPermutaciones(const std::string& letras);
#endif // SCRABBLE_H
