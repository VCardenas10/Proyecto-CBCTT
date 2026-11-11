#include "evaluacion.h"
using namespace std;

Evaluacion evaluar(const Instance& inst, const Solucion& sol) {
    Evaluacion ev;
    int P = inst.totalPeriodos();

    vector<vector<int>> clasesEnPeriodo(inst.nCourses, vector<int>(P, 0));       
    vector<vector<int>> clasesEnDia(inst.nCourses, vector<int>(inst.nDays, 0));  
    vector<vector<char>> salaUsada(inst.nCourses, vector<char>(inst.nRooms, 0));  
    vector<int> programadas(inst.nCourses, 0);                                   

    for (int r = 0; r < inst.nRooms; r++) {
        for (int p = 0; p < P; p++) {
            int c = sol.matriz[r][p];
            if (c < 0) continue;

            clasesEnPeriodo[c][p]++;
            clasesEnDia[c][inst.diaDePeriodo(p)]++;
            salaUsada[c][r] = 1;
            programadas[c]++;

            //Availabilities: clase en un periodo prohibido para el curso
            if (inst.unavailable[c][p]) ev.availabilities++;

            //RoomCapacity: estudiantes que no caben en la sala
            int exceso = inst.courses[c].estudiantes - inst.rooms[r].capacidad;
            if (exceso > 0) ev.roomCapacity += exceso;
        }
    }
    //RoomOccupancy queda en 0: la grilla guarda un solo curso por celda

    //Lectures: clases faltantes (o sobrantes) y clases del mismo curso en un mismo periodo
    for (int c = 0; c < inst.nCourses; c++) {
        ev.lectures += abs(inst.courses[c].bloques - programadas[c]);
        for (int p = 0; p < P; p++)
            if (clasesEnPeriodo[c][p] > 1) ev.lectures += clasesEnPeriodo[c][p] - 1;
    }

    //Conflicts: por cada periodo, pares de clases de cursos distintos que chocan
    for (int p = 0; p < P; p++) {
        for (int r1 = 0; r1 < inst.nRooms; r1++) {
            int a = sol.matriz[r1][p];
            if (a < 0) continue;
            for (int r2 = r1 + 1; r2 < inst.nRooms; r2++) {
                int b = sol.matriz[r2][p];
                if (b >= 0 && a != b && inst.conflict[a][b]) ev.conflicts++;
            }
        }
    }

    //MinWorkingDays y RoomStability, por curso
    for (int c = 0; c < inst.nCourses; c++) {
        int dias = 0;
        for (int d = 0; d < inst.nDays; d++)
            if (clasesEnDia[c][d] > 0) dias++;
        if (dias < inst.courses[c].minDias) ev.minWorkingDays += inst.courses[c].minDias - dias;

        int salas = 0;
        for (int r = 0; r < inst.nRooms; r++)
            if (salaUsada[c][r]) salas++;
        if (salas > 1) ev.roomStability += salas - 1;
    }

    //CurrCompactness: una clase del curriculo esta aislada si no tiene otra clase
    //del mismo curriculo en el slot anterior ni en el siguiente del mismo dia
    for (const auto& q : inst.curricula) {
        vector<int> clasesQ(P, 0);   
        for (int c : q.courseIdx)
            for (int p = 0; p < P; p++)
                clasesQ[p] += clasesEnPeriodo[c][p];

        for (int p = 0; p < P; p++) {
            if (clasesQ[p] == 0) continue;
            int slot = inst.slotDePeriodo(p);
            bool anterior  = slot > 0 && clasesQ[p - 1] > 0;
            bool siguiente = slot < inst.periodsPerDay - 1 && clasesQ[p + 1] > 0;
            if (!anterior && !siguiente) ev.currCompactness += clasesQ[p];
        }
    }

    return ev;
}
