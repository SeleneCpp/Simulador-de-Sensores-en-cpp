#ifndef SENSOR_H_
#define SENSOR_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <ctime>
#include <fstream>

using namespace std;
using namespace std::chrono;

class SensorData {
    friend class SensorManager;

private:
    string medicion;
    time_point<system_clock> tiempo;

public:
    SensorData();
    SensorData(const string& m, time_point<system_clock> t);

    string getMedicion() const;
    time_point<system_clock> getTiempo() const;
    void escribirValor();
    void mostrar() const;
};

class Sensor {
    friend class SensorManager;

private:
    string ID;
    char TipoMedicion;
    char Zona;
    float RangoSup;
    float RangoInf;
    vector<SensorData> mediciones;

public:
    Sensor();
    Sensor(const string& id, char tipo, char zona, float inf, float sup);

    void setDatos(const string& id, char tipo, char zona, float inf, float sup);
    string getID() const;

    void agregarMedicion(const SensorData& data);
    void mostrarMediciones() const;
    void mostrarMediciones(ostream& os) const;  // NUEVO

    void mostrarInfo() const;
    void mostrarInfo(ostream& os) const;        // NUEVO
};

class SensorManager {
private:
    map<string, Sensor> sensores;
    vector<Sensor> vectorSensores;
    int contadorSensores = 0;

public:
    void cargarSensor();
    void cargarMedicion();
    void mostrarTodo() const;
    void mostrarTodo(ostream& os) const;
    void estadisticasPorTipo(char tipo) const;
    void mostrarErrores() const;  // NUEVO
    void consultarPorZona(char zona) const;  // NUEVO


};

#endif /* SENSOR_H_ */
