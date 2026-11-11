#include "salida.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

//Escribe el contenido en cualquier flujo (archivo o consola)
static void escribirEn(ostream& out, const Instance& inst, const Solucion& sol,
                       const Evaluacion& ev, double tiempo) {
    out << fixed << setprecision(3) << tiempo << "\n";
    out << ev.infactibilidad() << " " << ev.penalizacion() << "\n\n";

    out << "# Conteo restricciones duras\n";
    out << ev.lectures << " " << ev.conflicts << " " << ev.roomOccupancy << " " << ev.availabilities << "\n\n";

    out << "# Conteo restricciones blandas\n";
    out << ev.roomCapacity << " " << ev.minWorkingDays << " " << ev.currCompactness << " " << ev.roomStability << "\n\n";

    out << "# Lista de asignaciones (una por leccion)\n";
    for (int p = 0; p < inst.totalPeriodos(); p++)
        for (int r = 0; r < inst.nRooms; r++) {
            int c = sol.matriz[r][p];
            if (c < 0) continue;
            out << inst.courses[c].id << " " << inst.rooms[r].id << " "
                << inst.diaDePeriodo(p) << " " << inst.slotDePeriodo(p) << "\n";
        }
}

void escribirSalida(const string& ruta, const Instance& inst, const Solucion& sol,
                    const Evaluacion& ev, double tiempo) {
    if (ruta.empty()) {
        escribirEn(cout, inst, sol, ev, tiempo);
        return;
    }
    ofstream archivo(ruta);
    if (!archivo.is_open())
        throw runtime_error("No se pudo escribir la salida: " + ruta);
    escribirEn(archivo, inst, sol, ev, tiempo);
}
