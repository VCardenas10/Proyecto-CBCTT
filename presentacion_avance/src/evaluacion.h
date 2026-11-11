#ifndef evaluacion_H
#define evaluacion_H
#include "parser.h"
#include "solucion.h"
using namespace std;

//Pesos de las restricciones blandas (formulacion UD2 / ITC-2007)
const int PESO_CAPACIDAD   = 1;
const int PESO_DIAS        = 5;
const int PESO_COMPACIDAD  = 2;
const int PESO_ESTABILIDAD = 1;

const long long PESO_DURA = 100000;

struct Evaluacion {
    //Restricciones duras:
    int lectures = 0;       
    int conflicts = 0;      
    int roomOccupancy = 0;  
    int availabilities = 0; 

    //Restricciones blandas:
    int roomCapacity = 0;    
    int minWorkingDays = 0; 
    int currCompactness = 0;
    int roomStability = 0;  

    int infactibilidad() const { return lectures + conflicts + roomOccupancy + availabilities; }

    int penalizacion() const {
        return PESO_CAPACIDAD * roomCapacity + PESO_DIAS * minWorkingDays +
               PESO_COMPACIDAD * currCompactness + PESO_ESTABILIDAD * roomStability;
    }

    //Funcion objetivo:
    long long costo() const { return PESO_DURA * infactibilidad() + penalizacion(); }
};
 
Evaluacion evaluar(const Instance& inst, const Solucion& sol);

#endif
