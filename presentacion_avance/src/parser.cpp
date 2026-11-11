#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

static vector<string> separarTokens(const string& linea) {
    vector<string> tokens;
    istringstream ss(linea);
    string tok;
    while (ss >> tok) tokens.push_back(tok);
    return tokens;
}

static int enteroCabecera(const string& linea) {
    istringstream ss(linea);
    string clave;
    int valor = 0;
    ss >> clave >> valor;
    return valor;
}

void Instance::cargar(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open())
        throw runtime_error("No se pudo abrir la instancia: " + ruta);

    string linea;

    while (getline(archivo, linea)) {
        if (linea.find("COURSES:") != string::npos) break;
        if (linea.rfind("Name:", 0) == 0) {
            name = linea.substr(linea.find(':') + 1);
            size_t s = name.find_first_not_of(" \t");
            name = (s == string::npos) ? "" : name.substr(s);
        }
        else if (linea.rfind("Courses:", 0) == 0)          nCourses = enteroCabecera(linea);
        else if (linea.rfind("Rooms:", 0) == 0)            nRooms = enteroCabecera(linea);
        else if (linea.rfind("Days:", 0) == 0)             nDays = enteroCabecera(linea);
        else if (linea.rfind("Periods_per_day:", 0) == 0)  periodsPerDay = enteroCabecera(linea);
        else if (linea.rfind("Curricula:", 0) == 0)        nCurricula = enteroCabecera(linea);
        else if (linea.rfind("Constraints:", 0) == 0)      nConstraints = enteroCabecera(linea);
    }

    courses.reserve(nCourses);
    int leidos = 0;
    while (leidos < nCourses && getline(archivo, linea)) {
        auto t = separarTokens(linea);
        if (t.empty()) continue;
        Course c;
        c.id          = t[0];
        c.profesor    = t[1];
        c.bloques     = stoi(t[2]);
        c.minDias     = stoi(t[3]);
        c.estudiantes = stoi(t[4]);
        courseIndex[c.id] = (int)courses.size();
        courses.push_back(c);
        leidos++;
    }

    while (getline(archivo, linea))
        if (linea.find("ROOMS:") != string::npos) break;
    rooms.reserve(nRooms);
    leidos = 0;
    while (leidos < nRooms && getline(archivo, linea)) {
        auto t = separarTokens(linea);
        if (t.empty()) continue;
        Room r;
        r.id        = t[0];
        r.capacidad = stoi(t[1]);
        roomIndex[r.id] = (int)rooms.size();
        rooms.push_back(r);
        leidos++;
    }

    while (getline(archivo, linea))
        if (linea.find("CURRICULA:") != string::npos) break;
    curricula.reserve(nCurricula);
    leidos = 0;
    while (leidos < nCurricula && getline(archivo, linea)) {
        auto t = separarTokens(linea);
        if (t.empty()) continue;
        Curriculum q;
        q.id = t[0];
        int cantidad = stoi(t[1]);
        for (int i = 0; i < cantidad; i++) {
            auto it = courseIndex.find(t[2 + i]);
            if (it == courseIndex.end())
                throw runtime_error("Curriculo " + q.id + " referencia curso inexistente: " + t[2 + i]);
            q.courseIdx.push_back(it->second);
        }
        curricula.push_back(q);
        leidos++;
    }

    unavailable.assign(nCourses, vector<char>(totalPeriodos(), 0));
    while (getline(archivo, linea))
        if (linea.find("UNAVAILABILITY_CONSTRAINTS:") != string::npos) break;
    leidos = 0;
    while (leidos < nConstraints && getline(archivo, linea)) {
        auto t = separarTokens(linea);
        if (t.empty()) continue;
        if (t[0] == "END.") break;
        auto it = courseIndex.find(t[0]);
        if (it == courseIndex.end())
            throw runtime_error("Restriccion referencia curso inexistente: " + t[0]);
        int c    = it->second;
        int dia  = stoi(t[1]);
        int slot = stoi(t[2]);
        int p    = periodoDe(dia, slot);
        if (p >= 0 && p < totalPeriodos())
            unavailable[c][p] = 1;
        leidos++;
    }
    construirConflictos();
}

void Instance::construirConflictos() {
    conflict.assign(nCourses, vector<char>(nCourses, 0));

    for (int i = 0; i < nCourses; i++)
        for (int j = i + 1; j < nCourses; j++)
            if (courses[i].profesor == courses[j].profesor)
                conflict[i][j] = conflict[j][i] = 1;

    for (const auto& q : curricula) {
        const auto& cs = q.courseIdx;
        for (size_t a = 0; a < cs.size(); a++)
            for (size_t b = a + 1; b < cs.size(); b++)
                conflict[cs[a]][cs[b]] = conflict[cs[b]][cs[a]] = 1;
    }
}

void Instance::imprimirResumen() const {
    int paresConflicto = 0;
    for (int i = 0; i < nCourses; i++)
        for (int j = i + 1; j < nCourses; j++)
            if (conflict[i][j]) paresConflicto++;

    int totalBloques = 0;
    for (const auto& c : courses) totalBloques += c.bloques;

    int celdasNoDisp = 0;
    for (const auto& fila : unavailable)
        for (char v : fila) if (v) celdasNoDisp++;

    cout << "==================== RESUMEN DE INSTANCIA ====================\n";
    cout << "Nombre            : " << name << "\n";
    cout << "Cursos (|C|)      : " << nCourses << "\n";
    cout << "Salas  (|R|)      : " << nRooms << "\n";
    cout << "Dias   (|D|)      : " << nDays << "\n";
    cout << "Periodos/dia      : " << periodsPerDay << "\n";
    cout << "Periodos totales  : " << totalPeriodos() << "\n";
    cout << "Curriculos (|Q|)  : " << nCurricula << "\n";
    cout << "Total de clases   : " << totalBloques << " (suma de L_c)\n";
    cout << "Pares en conflicto: " << paresConflicto << "\n";
    cout << "Celdas no disp.   : " << celdasNoDisp << " (suma de |U_c|)\n";
    cout << "==============================================================\n";
}
