#include "solucion.h"
#include <algorithm>
#include <stdexcept>
using namespace std;

Solucion::Solucion(const Instance& inst) {
    matriz.assign(inst.nRooms, vector<int>(inst.totalPeriodos(), -1));
}

void Solucion::generarAleatoria(const Instance& inst, mt19937& rng) {
    int P = inst.totalPeriodos();

    //Lista con todas las celdas (sala, periodo) de la matriz
    vector<pair<int,int>> celdas;
    for (int r = 0; r < inst.nRooms; r++)
        for (int p = 0; p < P; p++)
            celdas.push_back({r, p});

    //Lista con una entrada por cada clase a programar (el curso c aparece bloques veces)
    vector<int> clases;
    for (int c = 0; c < inst.nCourses; c++)
        for (int k = 0; k < inst.courses[c].bloques; k++)
            clases.push_back(c);

    if (clases.size() > celdas.size())
        throw runtime_error("Hay mas clases que celdas (sala, periodo) disponibles");

    //Se mezclan las celdas y cada clase toma una celda distinta
    shuffle(celdas.begin(), celdas.end(), rng);
    for (size_t i = 0; i < clases.size(); i++)
        matriz[celdas[i].first][celdas[i].second] = clases[i];
}
