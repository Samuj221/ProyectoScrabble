#include "scrabble.h"
#include "arboles.h"
#include "grafos.h" 
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

unordered_map<char, int> tablaPuntuacion = {
    {'E', 1}, {'A', 1}, {'I', 1}, {'O', 1}, {'N', 1}, {'R', 1}, {'T', 1}, {'L', 1}, {'S', 1}, {'U', 1},
    {'D', 2}, {'G', 2},
    {'B', 3}, {'C', 3}, {'M', 3}, {'P', 3},
    {'F', 4}, {'H', 4}, {'V', 4}, {'W', 4}, {'Y', 4},
    {'K', 5},
    {'J', 8}, {'X', 8},
    {'Q', 10}, {'Z', 10}
};

// Función para mostrar los puntajes
void mostrarPuntajes(const vector<string>& nombres, const vector<int>& puntajes) {
    cout << "=== Puntajes ===" << endl;
    for (size_t i = 0; i < nombres.size(); ++i) {
        cout << nombres[i] << ": " << puntajes[i] << " puntos" << endl;
    }
}

// Función para inicializar el diccionario
void inicializarDiccionario(const string& archivo) {
    try {
        ifstream inputFile(archivo);
        if (!inputFile.is_open()) {
            throw runtime_error("(Archivo no existe) El archivo " + archivo + " no existe o no puede ser leído.");
        }

        vector<string> palabrasValidas;
        string palabra;
        while (getline(inputFile, palabra)) {
            if (all_of(palabra.begin(), palabra.end(), [](char c) { return isalpha(c) || isspace(c); })) {
                palabrasValidas.push_back(palabra);
            }
        }

        inputFile.close();

        ofstream outputFile("diccionario.txt");
        for (const auto& palabra : palabrasValidas) {
            outputFile << palabra << '\n';
        }
        outputFile.close();

        cout << "(Resultado exitoso) El diccionario se ha inicializado correctamente.\n";
    } catch (const exception& e) {
        cerr << "(Error) " << e.what() << endl;
    }
}

// Función para inicializar el diccionario inverso
void iniciarDiccionarioInverso(const string& archivo, const string& salida, queue<string>& mensajes) {
    try {
        ifstream inputFile(archivo);
        if (!inputFile.is_open()) {
            throw runtime_error("(Archivo no existe) El archivo " + archivo + " no existe o no puede ser leído.");
        }

        vector<string> palabrasValidas;
        string palabra;
        while (getline(inputFile, palabra)) {
            if (all_of(palabra.begin(), palabra.end(), [](char c) { return isalpha(c) || isspace(c); })) {
                reverse(palabra.begin(), palabra.end());
                palabrasValidas.push_back(palabra);
            }
        }

        inputFile.close();

        // Verificar si la salida estándar está asociada a una consola
        if (!isatty(fileno(stdout))) {
            mensajes.push("(Resultado exitoso) El diccionario inverso no se crea en la consola.\n");
            return;
        }

        ofstream outputFile(salida);
        for (const auto& palabra : palabrasValidas) {
            outputFile << palabra << '\n';
        }
        outputFile.close();

        cout << "(Resultado exitoso) El diccionario inverso se ha inicializado correctamente.\n";
    } catch (const exception& e) {
        cerr << "(Error) " << e.what() << endl;
        mensajes.push("(Error) " + string(e.what()) + "\n");
    }
}

// Función para convertir una cadena a minúsculas
void toLowerCase(string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

// Función para verificar si una palabra es válida en el diccionario
bool esPalabraValida(const string& palabra, const string& diccionario) {
    ifstream diccionarioFile(diccionario);
    if (!diccionarioFile.is_open()) {
        throw runtime_error("(Archivo no existe) El archivo " + diccionario + " no existe o no puede ser leído.");
    }

    string palabraDic;
    string palabraLower = palabra;  // Crear una copia de la palabra original

    toLowerCase(palabraLower);  // Convertir la palabra a minúsculas

    while (diccionarioFile >> palabraDic) {
        toLowerCase(palabraDic);  // Convertir la palabra del diccionario a minúsculas
        if (palabraLower == palabraDic) {
            diccionarioFile.close();
            return true;
        }
    }

    return false;
}

// Función para verificar si una palabra contiene símbolos inválidos
bool contieneSimbolosInvalidos(const string& palabra) {
    try {
        if (!all_of(palabra.begin(), palabra.end(), [](char c) { return isalpha(c) || isspace(c); })) {
            throw invalid_argument("(Símbolos inválidos) La palabra contiene caracteres no permitidos.");
        }
    } catch (const exception& e) {
        cerr << "(Error) " << e.what() << endl;
        return true;
    }
    return false;
}

// Función para calcular el puntaje de una palabra
int calcularPuntaje(const string& palabra) {
    int puntaje = 0;
    for (char letra : palabra) {
        puntaje += tablaPuntuacion[toupper(letra)];
    }
    return puntaje;
}

// Función para obtener el puntaje de una palabra y verificar su validez
void obtenerPuntaje(const string& palabra, const string& diccionario, const string& diccionarioInverso, bool simulacion) {
    if (contieneSimbolosInvalidos(palabra)) {
        cout << "(Letras inválidas) La palabra contiene símbolos inválidos.\n";
        return;
    }

    if (esPalabraValida(palabra, diccionario) || esPalabraValida(palabra, diccionarioInverso)) {
        int puntaje = calcularPuntaje(palabra);
        int tamaño = palabra.size();
        cout << "(Resultado exitoso) La palabra '" << palabra << "' tiene un puntaje de " << puntaje << " y un tamaño de " << tamaño << " caracteres.\n";
    } else {
        cout << "(Palabra no existe) La palabra no existe en el diccionario.\n";
    }
}

// Función para leer una variable persistente de un archivo
int leerVariablePersistente() {
    int valor;
    ifstream archivo("variable.txt");
    if (archivo.is_open()) {
        archivo >> valor;
        archivo.close();
    } else {
        valor = 0;
    }
    return valor;
}

// Función para escribir una variable persistente en un archivo
void escribirVariablePersistente(int valor) {
    ofstream archivo("variable.txt");
    if (archivo.is_open()) {
        archivo << valor;
        archivo.close();
    } else {
        cerr << "Error: No se puede abrir el archivo para escribir la variable persistente." << endl;
    }
}

// Función para mostrar la ayuda del programa
void mostrarAyuda() {
    cout << "=== ./main ayuda ===" << endl;
    cout << "Este comando de ayuda al jugador, es para ejecutar el juego en consola y que sepa cual comando debe digitar en concola:" << endl;
    cout << endl << "Comandos disponibles:" << endl;
    cout << endl << "1: ./main inicializar_diccionario" << endl;
    cout << "-> Inicializa el diccionario con las palabras del archivo especificado." << endl;
    cout << "   Este comando carga un archivo de texto que contiene un diccionario de palabras" << endl;
    cout << "   en el idioma original del juego (ingles). Las palabras deben estar escritas" << endl;
    cout << "   en un formato valido y se almacenan para su posterior uso." << endl << endl;
    cout << "2: ./main inicializar_diccionario_inverso" << endl;
    cout << "-> Inicializa el diccionario inverso con las palabras del archivo especificado." << endl;
    cout << "   Similar al comando 'inicializar', pero las palabras se almacenan en sentido" << endl;
    cout << "   inverso (leidas de derecha a izquierda), facilitando ciertas operaciones." << endl;
    cout << "-> Tambien se presenta la opcion de visualizar el diccionario inverso" << endl;
    cout << "   Esta opcion solo se puede usar despues de inicializar el diccionario inverso" << endl;
    cout << "   Lo cual permite visualizar las palabras en orden inverso" << endl << endl;
    cout << "3: ./main calcular_puntaje" << endl;
    cout << "-> Calcula el puntaje de una palabra dada y muestra el resultado en pantalla." << endl;
    cout << "   Este comando recibe una palabra como entrada, verifica su validez" << endl;
    cout << "   en el diccionario y calcula el puntaje basado en las reglas del juego." << endl << endl;
    cout << "4: ./main persistencia" << endl;
    cout << "-> Lee y muestra el valor de la variable persistente almacenada en un archivo." << endl;
    cout << "   Este comando permite recuperar el valor de una variable almacenada en" << endl;
    cout << "   un archivo de texto, facilitando la persistencia de datos entre sesiones." << endl << endl;
    cout << "5: ./main mostrar_grafo_palabras" << endl;
    cout << "-> Muestra el grafo de palabras posibles a partir de las letras dadas." << endl;
    cout << "   Este comando genera todas las posibles palabras a partir de un conjunto" << endl;
    cout << "   de letras y construye un grafo donde las palabras estan conectadas si" << endl;
    cout << "   difieren en una sola letra. Esto es util para visualizar relaciones" << endl;
    cout << "   entre palabras y encontrar posibles jugadas." << endl << endl;
}


// Función principal
int main(int argc, char* argv[]) {
    if (argc < 2) {
        mostrarAyuda();
        return 1;
    }

    string comando = argv[1];

    if (comando == "inicializar_diccionario") {
        if (argc != 3) {
            cout << "Uso: ./main inicializar_diccionario <archivo>" << endl;
            return 1;
        }
        inicializarDiccionario(argv[2]);
    } else if (comando == "inicializar_diccionario_inverso") {
        if (argc != 4) {
            cout << "Uso: ./main inicializar_diccionario_inverso <archivo> <salida>" << endl;
            return 1;
        }
        queue<string> mensajes;
        iniciarDiccionarioInverso(argv[2], argv[3], mensajes);
        while (!mensajes.empty()) {
            cout << mensajes.front();
            mensajes.pop();
        }
    } else if (comando == "calcular_puntaje") {
        if (argc != 3) {
            cout << "Uso: ./main calcular_puntaje <palabra>" << endl;
            return 1;
        }
        string palabra = argv[2];
        int puntaje = calcularPuntaje(palabra);
        cout << "La palabra '" << palabra << "' tiene un puntaje de " << puntaje << endl;
    } else if (comando == "obtener_puntaje") {
        if (argc != 5) {
            cout << "Uso: ./main obtener_puntaje <palabra> <diccionario> <diccionarioInverso>" << endl;
            return 1;
        }
        string palabra = argv[2];
        string diccionario = argv[3];
        string diccionarioInverso = argv[4];
        obtenerPuntaje(palabra, diccionario, diccionarioInverso, false);
    } else if (comando == "leer_variable_persistente") {
        int valor = leerVariablePersistente();
        cout << "Valor de la variable persistente: " << valor << endl;
    } else if (comando == "escribir_variable_persistente") {
        if (argc != 3) {
            cout << "Uso: ./main escribir_variable_persistente <valor>" << endl;
            return 1;
        }
        int valor = stoi(argv[2]);
        escribirVariablePersistente(valor);
    } else if (comando == "to_lowercase") {
        if (argc != 3) {
            cout << "Uso: ./main to_lowercase <cadena>" << endl;
            return 1;
        }
        string cadena = argv[2];
        toLowerCase(cadena);
        cout << "Cadena en minúsculas: " << cadena << endl;
    } else if (comando == "grafo_de_palabras") {
        if (argc != 3) {
            cout << "Uso: ./main grafo_de_palabras <diccionario>" << endl;
            return 1;
        }
        grafoDePalabras(argv[2]);
    } else if (comando == "posibles_palabras") {
        if (argc != 4) {
            cout << "Uso: ./main posibles_palabras <letras> <diccionario>" << endl;
            return 1;
        }
        posiblesPalabras(argv[2], argv[3]);
    } else if (comando == "ayuda") {
        mostrarAyuda();
    } else if (comando == "salir") {
        cout << "Saliendo del programa..." << endl;
        return 0;
    } else {
        cout << "Comando no reconocido. Usa './main ayuda' para ver la lista de comandos disponibles." << endl;
        return 1;
    }

    return 0;
}