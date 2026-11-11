#ifndef solucion_H
#define solucion_H
#include <vector>
#include <random>
#include "parser.h"
using namespace std;

class Solucion {
public:
    vector<vector<int>> matriz;

    Solucion(const Instance& inst);
    void generarAleatoria(const Instance& inst, mt19937& rng);
};

#endif
