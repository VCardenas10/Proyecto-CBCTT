#ifndef parser_H
#define parser_H
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Course {
    string id;
    string profesor;
    int bloques;      
    int minDias;      
    int estudiantes; 
};


struct Room {
    string id;
    int capacidad;  
};


struct Curriculum {
    string id;
    vector<int> courseIdx;   
};

class Instance {
public:
    string name;
    int nCourses = 0;
    int nRooms = 0;
    int nDays = 0;
    int periodsPerDay = 0;
    int nCurricula = 0;
    int nConstraints = 0;

    vector<Course> courses;         // C
    vector<Room> rooms;             // R
    vector<Curriculum> curricula;   // Q

    unordered_map<string, int> courseIndex; 
    unordered_map<string, int> roomIndex;

    //Son de tipo Char porque al usar HillCLimbing es más barato que bool
    vector<vector<char>> unavailable;
    vector<vector<char>> conflict;

    int totalPeriodos() const { return nDays * periodsPerDay; }

    int periodoDe(int dia, int slot) const { return dia * periodsPerDay + slot; }
    int diaDePeriodo(int periodo) const { return periodo / periodsPerDay; }
    int slotDePeriodo(int periodo) const { return periodo % periodsPerDay; }

    void cargar(const string& ruta);

    void imprimirResumen() const;

private:
    void construirConflictos();
};

#endif 
