#include "parser.h"
#include "solucion.h"
#include "evaluacion.h"
#include "salida.h"
#include <iostream>
#include <string>
#include <chrono>
#include <random>
using namespace std;

//Uso: cbctt <instancia.ctt> [archivo_salida] [semilla]
int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <instancia.ctt> [archivo_salida] [semilla]\n";
        return 1;
    }
    string rutaSalida = (argc >= 3) ? argv[2] : "";
    unsigned semilla = (argc >= 4) ? stoul(argv[3]) : (unsigned)chrono::system_clock::now().time_since_epoch().count();

    try {
        auto inicio = chrono::steady_clock::now();

        Instance inst;
        inst.cargar(argv[1]);

        mt19937 rng(semilla);
        Solucion sol(inst);
        sol.generarAleatoria(inst, rng);

        Evaluacion ev = evaluar(inst, sol);

        double tiempo = chrono::duration<double>(chrono::steady_clock::now() - inicio).count();
        escribirSalida(rutaSalida, inst, sol, ev, tiempo);
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
