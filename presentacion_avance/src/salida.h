#ifndef salida_H
#define salida_H
#include <string>
#include "parser.h"
#include "solucion.h"
#include "evaluacion.h"
using namespace std;

void escribirSalida(const string& ruta, const Instance& inst, const Solucion& sol,
                    const Evaluacion& ev, double tiempo);

#endif
