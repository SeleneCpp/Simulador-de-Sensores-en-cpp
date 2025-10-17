#include "Sensor.h"

using namespace std;

// SensorData

SensorData::SensorData()
    : medicion("0"), tiempo(system_clock::now())
{}

SensorData::SensorData(const string& m, time_point<system_clock> t)
    : medicion(m), tiempo(t)
{}

string SensorData::getMedicion() const { return medicion; }

time_point<system_clock> SensorData::getTiempo() const { return tiempo; }

void SensorData::escribirValor() {
    cout << "Ingrese valor de la medicion: ";
    cin >> medicion;
    tiempo = system_clock::now();
}

void SensorData::mostrar() const {
    time_t t_c = system_clock::to_time_t(tiempo);
    cout << "  Valor: " << medicion << ", Tiempo: " << ctime(&t_c);
}

// Sensor

Sensor::Sensor()
    : RangoSup(0.0f), RangoInf(0.0f), ID("NULL"), TipoMedicion('0'), Zona('0') {}

Sensor::Sensor(const string& id, char tipo, char zona, float inf, float sup)
    : ID(id), TipoMedicion(tipo), Zona(zona), RangoInf(inf), RangoSup(sup)
{}

void Sensor::setDatos(const string& id, char tipo, char zona, float inf, float sup) {
    ID = id;
    TipoMedicion = tipo;
    Zona = zona;
    RangoInf = inf;
    RangoSup = sup;
}

string Sensor::getID() const {
    return ID;
}

void Sensor::agregarMedicion(const SensorData& data) {
    mediciones.push_back(data);
}

void Sensor::mostrarMediciones() const {
    for (const auto& m : mediciones) {
        m.mostrar();
    }
}

void Sensor::mostrarMediciones(ostream& os) const {
    for (const auto& m : mediciones) {
        time_t t_c = system_clock::to_time_t(m.getTiempo());
        os << "  Valor: " << m.getMedicion() << ", Tiempo: " << ctime(&t_c);
    }
}

void Sensor::mostrarInfo() const {
    cout << "ID: " << ID
         << ", Tipo: " << TipoMedicion
         << ", Zona: " << Zona
         << ", Rango: " << RangoInf << " - " << RangoSup << "\n";
}

void Sensor::mostrarInfo(ostream& os) const {
    os << "ID: " << ID
       << ", Tipo: " << TipoMedicion
       << ", Zona: " << Zona
       << ", Rango: " << RangoInf << " - " << RangoSup << "\n";
}

// SensorManager

void SensorManager::cargarSensor() {
    char opcion = 's';

    while (opcion == 's' || opcion == 'S') {
        char tipo, zona;
        float inf, sup;

        cout << "Ingrese tipo de medicion (ej: T para temperatura): ";
        cin >> tipo;
        cout << "Ingrese zona (A, B, C...): ";
        cin >> zona;
        cout << "Ingrese su rango inferior: ";
        cin >> inf;
        cout << "Ingrese su rango superior: ";
        cin >> sup;

        contadorSensores++;

        string numero = (contadorSensores < 10 ? "0" : "") + to_string(contadorSensores);
        string id = string(1, zona) + string(1, tipo) + numero;

        Sensor s(id, tipo, zona, inf, sup);
        sensores[id] = s;

        cout << "ID generado: " << id << endl;
        cout << "Sensor registrado exitosamente.\n";

        cout << "\n¿Desea agregar otro sensor? (s/n): ";
        cin >> opcion;
        cout << "\n";
    }
}

void SensorManager::mostrarTodo() const {
    mostrarTodo(cout);  // reutiliza la versión con ostream
}

void SensorManager::mostrarTodo(ostream& os) const {
    if (sensores.empty()) {
        os << "No hay sensores registrados.\n";
        return;
    }

    for (const auto& par : sensores) {
        const Sensor& s = par.second;
        s.mostrarInfo(os);
        s.mostrarMediciones(os);
        os << "-------------------------\n";
    }
}

void SensorManager::cargarMedicion() {
    string id;
    cout << "Ingrese ID del sensor: ";
    cin >> id;

    auto it = sensores.find(id);
    if (it == sensores.end()) {
        cout << "Sensor no encontrado.\n";
        return;
    }

    Sensor& sensor = it->second;
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        SensorData nueva;
        nueva.escribirValor();

        float valor = std::stof(nueva.getMedicion());

        if (valor >= sensor.RangoInf && valor <= sensor.RangoSup) {
            sensor.agregarMedicion(nueva);
        } else {
            SensorData errorData("error", system_clock::now());
            sensor.agregarMedicion(errorData);
        }

        cout << "\n¿Desea ingresar otra medicion para este sensor? (s/n): ";
        cin >> continuar;
        cout << "\n";
    }
}

void SensorManager::estadisticasPorTipo(char tipo) const {
    vector<float> valores;

    for (const auto& par : sensores) {
        const Sensor& sensor = par.second;

        if (sensor.ID.size() > 1 && sensor.ID[1] == tipo) {
            for (const auto& medicion : sensor.mediciones) {
                if (medicion.getMedicion() != "error") {
                    valores.push_back(stof(medicion.getMedicion()));
                }
            }
        }
    }

    if (valores.empty()) {
        cout << "No hay mediciones validas para el tipo '" << tipo << "'.\n";
        return;
    }

    auto maxVal = *max_element(valores.begin(), valores.end());
    auto minVal = *min_element(valores.begin(), valores.end());
    float suma = accumulate(valores.begin(), valores.end(), 0.0f);
    float promedio = suma / valores.size();

    cout << "Estadisticas para tipo '" << tipo << "':\n";
    cout << "  Maximo: " << maxVal << "\n";
    cout << "  Minimo: " << minVal << "\n";
    cout << "  Promedio: " << promedio << "\n";
}

//Nuevo

void SensorManager::mostrarErrores() const {
    bool hayErrores = false;

    for (const auto& par : sensores) {
        const Sensor& s = par.second;
        vector<SensorData> errores;

        for (const auto& m : s.mediciones) {
            if (m.getMedicion() == "error") {
                errores.push_back(m);
            }
        }

        if (!errores.empty()) {
            hayErrores = true;
            cout << "Sensor " << s.ID << " - Mediciones fuera de rango:\n";
            for (const auto& e : errores) {
                time_t t_c = system_clock::to_time_t(e.getTiempo());
                cout << "  Valor: ERROR, Tiempo: " << ctime(&t_c);
            }
            cout << "-------------------------\n";
        }
    }

    if (!hayErrores) {
        cout << "No se encontraron mediciones fuera de rango.\n";
    }
}


void SensorManager::consultarPorZona(char zona) const {
    bool encontrados = false;

    for (const auto& par : sensores) {
        const Sensor& s = par.second;

        if (s.Zona == zona) {
            s.mostrarInfo();
            s.mostrarMediciones();
            cout << "-------------------------\n";
            encontrados = true;
        }
    }

    if (!encontrados) {
        cout << "No se encontraron sensores en la zona '" << zona << "'.\n";
    }
}

